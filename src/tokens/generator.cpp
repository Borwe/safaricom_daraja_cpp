#include "boost/date_time.hpp"
#include <boost/asio.hpp>
#include <boost/json/src.hpp>
#include <boost/utility/string_view_fwd.hpp>
#include <chrono>
#include <cstddef>
#include <daraja/tokens/generator.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/coroutine2/all.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <thread>

template<typename T>
using shared = std::shared_ptr<T>;

namespace Daraja{
    namespace tokens{

        namespace beast = boost::beast;
        namespace http = beast::http;
        namespace net = boost::asio;
        using tcp = net::ip::tcp;
        namespace cor2 = boost::coroutines2;

        template<typename T>
        using cor_t = cor2::coroutine<T>;


        //for handling pull requests
        std::vector<std::string> sh_tokens;

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
                shared<cor_t<std::string>::push_type> m_pusher;
                const bool m_is_async;
                bool &m_dont_ever_edit_keep_running;
            public:

                explicit safaricom_tokens_getter(const bool async,
                         bool &keep_running,
                        shared< cor_t<std::string>::push_type > pusher
                        ,const ConsumerValues &conf)
                    :m_conf(conf),m_port("443"),m_is_async(async),
                    m_dont_ever_edit_keep_running(keep_running){
                    m_pusher=pusher;
                    m_ctx = std::make_shared<net::ssl::context>(
                                boost::asio::ssl::context::sslv23_client);
                    m_ctx->set_verify_mode(net::ssl::verify_none);
                    m_io = std::make_shared<net::io_context>();
                    m_stream=std::make_shared<beast::ssl_stream<beast::tcp_stream>>(
                            net::make_strand(*m_io),*m_ctx);
                    m_resolver=std::make_shared<tcp::resolver>(*m_io);
                }


                void run(){
                    //don't keep running if reached end of turn
                    if(m_dont_ever_edit_keep_running==false){
                        beast::error_code ec;
                        m_stream->next_layer().socket()
                            .shutdown(tcp::socket::shutdown_both,ec);
                        if(m_io!=nullptr && !m_io->stopped()){
                            m_io->stop();
                        }
                        return;
                    }
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
                }

                void on_resolve(beast::error_code ec,
                        tcp::resolver::results_type results){
                    if(ec){
                        fail(ec,"On resolve");
                        run();
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
                                self->run();
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
                    boost::json::value val = boost::json::parse(body);
                    std::stringstream reader;
                    reader << val.at("expires_in").as_string().c_str();
                    uint64_t wait_period = 0;
                    reader >> wait_period;

                    std::string token_key = val.at("access_token").as_string()
                        .c_str();
                    if( m_is_async==false ){
                        (*m_pusher)(token_key);
                        m_pusher = nullptr;
                    }else{
                        (*m_pusher)(token_key);
                        shared<net::deadline_timer> t=
                            std::make_shared<net::deadline_timer>(
                                    net::make_strand(*m_io),
                                    boost::posix_time::milliseconds(wait_period));
                        t->expires_from_now( );
                        t->async_wait([self=shared_from_this(),tm=t]
                                (beast::error_code ec){

                            self->m_buffer.clear();
                            //clear vector
                            sh_tokens.clear();

                            self->m_stream = 
                                std::make_shared<beast::ssl_stream<beast::tcp_stream>>(
                                net::make_strand(*(self->m_io)),*(self->m_ctx));

                            self->run();
                        });
                    }
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
            conf(conf),doAsync(asyncGenerate),keep_running_async(true){
        }

        void AccessGenerator::setAccessToken(std::string &token){
            this->access_token=token;
        }

        const std::string AccessGenerator::getAccessToken() {
            if(this->doAsync==false){

                std::make_shared<safaricom_tokens_getter>(doAsync,keep_running_async,
                    std::make_shared<cor_t<std::string>::push_type >
                            ([self=this](cor_t<std::string>::pull_type &puller){

                        std::string access_token=puller.get();
                        self->setAccessToken(access_token);
                        puller();
                    })
                ,
                conf)->run();
            }else{
                while(sh_tokens.empty()){
                }
                auto val=sh_tokens.at(0);
                return val;
            }
            return access_token;
        }

        //TODO this is just for tests, clean this function to do appropriate thing
        void AccessGenerator::start(){
            if(this->doAsync==true){
                cor_t<std::string>::push_type pusher( []
                        (cor_t<std::string>::pull_type &puller){

                        while(true){
                            if(sh_tokens.empty() == false){
                                sh_tokens.clear();
                            }
                            auto val=puller.get();
                            sh_tokens.push_back(val);
                            puller();
                        }
                });
                shared< cor_t<std::string>::push_type > sh_pusher = 
                    std::make_shared<cor_t<std::string>::push_type >
                        (std::move(pusher));

                for_async_running=std::make_shared<std::thread>([this,sh_pusher](){
                        std::make_shared<safaricom_tokens_getter>(this->doAsync
                                ,this->keep_running_async,sh_pusher,this->conf)->run();
                });
            }else{
                throw std::runtime_error("Not allowed to call AccessGenerator::start() if doAsync is false");
            }
        }

        AccessGenerator::~AccessGenerator(){
            keep_running_async=false;
            if(for_async_running!=nullptr){
                if(for_async_running->joinable())
                    for_async_running->join();
            }
        }
    }
}
