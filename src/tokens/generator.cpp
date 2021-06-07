#include "boost/asio/deadline_timer.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/post.hpp"
#include "boost/asio/ssl/context.hpp"
#include "boost/asio/ssl/stream_base.hpp"
#include "boost/asio/ssl/verify_mode.hpp"
#include "boost/asio/steady_timer.hpp"
#include "boost/asio/strand.hpp"
#include "boost/beast/core/bind_handler.hpp"
#include "boost/beast/core/error.hpp"
#include "boost/beast/core/stream_traits.hpp"
#include "boost/beast/core/tcp_stream.hpp"
#include "boost/beast/http/empty_body.hpp"
#include "boost/beast/http/field.hpp"
#include "boost/beast/http/fields.hpp"
#include "boost/beast/http/message.hpp"
#include "boost/beast/http/read.hpp"
#include "boost/beast/http/string_body.hpp"
#include "boost/beast/ssl/ssl_stream.hpp"
#include "boost/date_time/posix_time/posix_time_config.hpp"
#include "boost/date_time/posix_time/posix_time_duration.hpp"
#include "boost/date_time/posix_time/ptime.hpp"
#include "boost/system/error_code.hpp"
#include <boost/asio.hpp>
#include <boost/utility/string_view_fwd.hpp>
#include <chrono>
#include <cstddef>
#include <daraja/tokens/generator.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include <thread>
#include <vcruntime.h>

namespace Daraja{
    namespace tokens{

        namespace beast = boost::beast;
        namespace http = beast::http;
        namespace net = boost::asio;
        using tcp = net::ip::tcp;

        template<typename T>
        using shared = std::shared_ptr<T>;

        class safaricom_tokens_getter:
            public std::enable_shared_from_this<safaricom_tokens_getter>{
                shared<net::ssl::context> m_ctx;
                shared<net::io_context> m_io;
                shared<tcp::resolver> m_resolver;
                shared<beast::ssl_stream<beast::tcp_stream>> m_stream;
                beast::flat_buffer m_buffer;
                shared<http::request<http::empty_body>> m_request;
                shared<http::response<http::string_body>> m_response;
                const ConsumerValues m_conf;
                std::string m_address;
                std::string m_port;
            public:

                explicit safaricom_tokens_getter(const ConsumerValues &conf)
                    :m_conf(conf),m_port("443"){
                    m_ctx = std::make_shared<net::ssl::context>(
                                boost::asio::ssl::context::sslv23_client);
                    m_ctx->set_verify_mode(net::ssl::verify_none);
                    m_io = std::make_shared<net::io_context>();
                    m_stream=std::make_shared<beast::ssl_stream<beast::tcp_stream>>(
                            net::make_strand(*m_io),*m_ctx);
                    m_resolver=std::make_shared<tcp::resolver>(*m_io);
                }

                void run(){
                    //get url from endpoint
                    std::string host=m_conf.getEndpoint().substr(8);
                    int end_of_host=host.find_first_of("/");
                    auto tmp=host;
                    host=host.substr(0,end_of_host);//host address
                    auto query=tmp.substr(end_of_host);// beginning of query

                    m_address = host;

                    int http_version=11;

                    //setup the request object
                    m_request = std::make_shared<http::request<http::empty_body>>();
                    m_request->version(http_version);
                    m_request->method(http::verb::get);
                    m_request->target(query);
                    m_request->set(http::field::host,m_address);


                    m_resolver->async_resolve(m_address,m_port,
                            beast::bind_front_handler(
                                &safaricom_tokens_getter::on_resolve,
                                shared_from_this()));
                    m_io->run();
                    std::thread([m_io=this->m_io](){m_io->run();}).detach();
                }

                void on_resolve(beast::error_code ec,
                        tcp::resolver::results_type results){
                    if(ec){
                        fail(ec,"On resolve");
                        return;
                    }
                    m_stream->next_layer()
                        .expires_after(std::chrono::seconds(50));
                    m_stream->next_layer().async_connect(results,
                            beast::bind_front_handler( 
                                &safaricom_tokens_getter::on_connect,
                                shared_from_this()));
                }

                void on_connect(beast::error_code ec,
                        tcp::resolver::results_type::endpoint_type endpoint_type){
                    if (ec){
                        fail(ec,"On connect");
                        //try reconnect after 5 seconds
                        net::deadline_timer t(
                                *m_io, boost::posix_time::seconds(1));
                        t.async_wait([self=shared_from_this()]
                                (boost::system::error_code ec){
                                self->m_resolver->async_resolve(self->m_address,
                                        self->m_port, beast::bind_front_handler(
                                            &safaricom_tokens_getter::on_resolve,
                                            self->shared_from_this()));
                        });
                        return;
                    }

                    m_stream->async_handshake(net::ssl::stream_base::client,
                            beast::bind_front_handler(
                                &safaricom_tokens_getter::on_handshake,
                                shared_from_this()));
                }

                void on_handshake(beast::error_code ec){
                    if(ec){
                        fail(ec,"On Handshake");
                        return;
                    }

                    std::string authorization = "Basic " + m_conf.getbase64KeysAndSecret();
                    m_request->set(http::field::authorization, authorization);
                    m_request->set(beast::http::field::content_type,
                            "application/json");

                    http::async_write(*m_stream, *m_request,
                            beast::bind_front_handler(&safaricom_tokens_getter::on_write,
                                shared_from_this()));
                }

                void on_write(beast::error_code ec,std::size_t bytes_transferred){
                    if(ec){
                        fail(ec,"On Write");
                        run();
                        return;
                    }

                    m_buffer.clear();
                    m_response = std::make_shared<http::response<http::string_body>>();

                    http::async_read(*m_stream, m_buffer, *m_response,
                            beast::bind_front_handler(&safaricom_tokens_getter::on_read,
                                shared_from_this()));
                }

                void on_read(beast::error_code ec, size_t bytes_transferred){
                    if(ec){
                        fail(ec,"On Read");
                        //meaning error occured while waiting for reply..
                        //so redo request
                        m_buffer.clear();
                        run();
                        return;
                    }

                    //meaning we didn't get a success from daraja
                    //so retry again
                    if(m_response->result_int()!=200){

                        shared<net::deadline_timer> t=
                            std::make_shared<net::deadline_timer>(
                                    net::make_strand(*m_io),
                                    boost::posix_time::seconds(2));
                        t->expires_from_now( );

                        t->async_wait([self=shared_from_this(),tm=t](beast::error_code ec){


                            std::string body = self->m_response->body();
                            std::cout<<body<<"\n";
                            self->m_buffer.clear();

                            self->m_stream = 
                                std::make_shared<beast::ssl_stream<beast::tcp_stream>>(
                                net::make_strand(*(self->m_io)),*(self->m_ctx));

                            self->run();
                        });

                        return;
                    }
                    std::string body = m_response->body();
                    std::cout<<body<<"\n";
                }

                /***
                 * Print error message
                 */
                void fail(beast::error_code &code, const char *err){
                    std::cerr << err << ": "<< code.message() << std::endl;
                }
        };

        AccessGenerator::AccessGenerator(const ConsumerValues &conf,
                bool asyncGenerate):
            conf(conf),doAsync(asyncGenerate){
        }

        const std::string AccessGenerator::getAccessToken()const{
            return "";
        }

        //TODO this is just for tests, clean this function to do appropriate thing
        void AccessGenerator::start()const{
            std::make_shared<safaricom_tokens_getter>(this->conf)->run();
        }
    }
}
