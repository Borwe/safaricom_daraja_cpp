#include "boost/asio/deadline_timer.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/post.hpp"
#include "boost/asio/ssl/context.hpp"
#include "boost/asio/ssl/stream_base.hpp"
#include "boost/asio/ssl/verify_mode.hpp"
#include "boost/asio/steady_timer.hpp"
#include "boost/beast/core/bind_handler.hpp"
#include "boost/beast/core/error.hpp"
#include "boost/beast/core/stream_traits.hpp"
#include "boost/beast/core/tcp_stream.hpp"
#include "boost/date_time/posix_time/posix_time_duration.hpp"
#include "boost/system/error_code.hpp"
#include <boost/asio.hpp>
#include <boost/utility/string_view_fwd.hpp>
#include <chrono>
#include <daraja/tokens/generator.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <iostream>
#include <memory>

namespace Daraja{
    namespace tokens{

        namespace beast = boost::beast;
        namespace http = beast::http;
        namespace net = boost::asio;
        using tcp = net::ip::tcp;

        class safaricom_tokens_getter:
            public std::enable_shared_from_this<safaricom_tokens_getter>{
                std::shared_ptr<net::ssl::context> m_ctx;
                tcp::resolver m_resolver;
                beast::ssl_stream<beast::tcp_stream> m_stream;
                beast::flat_buffer m_buffer;
                http::request<http::empty_body> m_request;
                http::response<http::string_body> m_response;
                const std::string m_server_query_endpoint;
                const std::string m_key;
                const std::string m_secret;
                std::string m_address;
            public:


                
                explicit safaricom_tokens_getter(net::io_context &io
                        ,std::shared_ptr<net::ssl::context> ctx
                        ,const ConsumerValues &conf)
                    :m_ctx(ctx),
                    m_resolver(net::make_strand(io)),
                    m_stream(net::make_strand(io),*m_ctx), 
                    m_server_query_endpoint(conf.getEndpoint()),
                    m_key(conf.getKey()),
                    m_secret(conf.getSecret()){}

                void run(){
                    //get url from endpoint
                    std::string host=m_server_query_endpoint.substr(8);
                    int end_of_host=host.find_first_of("/");
                    auto tmp=host;
                    host=host.substr(0,end_of_host);//host address
                    auto query=tmp.substr(end_of_host);// beginning of query

                    m_address = host;

                    int http_version=11;
                    std::string port="443";

                    //m_address="google.com";
                    std::cout<<m_address<<"\n";

                    //setup the request object
                    m_request.version(http_version);
                    m_request.method(http::verb::get);
                    m_request.target(query);
                    m_request.set(http::field::host,m_address);

                    m_resolver.async_resolve(m_address,port,
                            beast::bind_front_handler(
                                &safaricom_tokens_getter::on_resolve,
                                shared_from_this()));
                }

                void on_resolve(beast::error_code ec,
                        tcp::resolver::results_type results){
                    if(ec){
                        fail(ec,"On resolve");
                        return;
                    }
                    m_stream.next_layer()
                        .expires_after(std::chrono::seconds(5));
                    m_stream.next_layer().async_connect(results,
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
                                m_stream.get_executor(), boost::posix_time::seconds(1));
                        t.async_wait([self=shared_from_this()]
                                (boost::system::error_code ec){
                                self->m_resolver.async_resolve(self->m_address,"443",
                                        beast::bind_front_handler(
                                            &safaricom_tokens_getter::on_resolve,
                                            self->shared_from_this()));
                        });
                        return;
                    }

                    m_stream.async_handshake(net::ssl::stream_base::client,
                            beast::bind_front_handler(
                                &safaricom_tokens_getter::on_handshake,
                                shared_from_this()));
                }

                void on_handshake(beast::error_code ec){
                    if(ec){
                        fail(ec,"On Handshake");
                        return;
                    }
                    std::cout<<"YEAH!!!!!!\n";
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
            net::io_context ios;
            auto ctx=
                std::make_shared<net::ssl::context>(
                        boost::asio::ssl::context::sslv23_client);
            ctx->set_verify_mode(net::ssl::verify_none);
            std::make_shared<safaricom_tokens_getter>(ios,ctx,this->conf)->run();
            ios.run();
        }
    }
}
