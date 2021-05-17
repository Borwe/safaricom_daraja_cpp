#include <boost/asio.hpp>
#include <boost/utility/string_view_fwd.hpp>
#include <daraja/tokens/generator.hpp>
#include <boost/beast.hpp>
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
                tcp::resolver m_resolver;
                beast::tcp_stream m_stream;
                beast::flat_buffer m_buffer;
                http::request<http::empty_body> m_request;
                http::response<http::string_body> m_response;
                const std::string m_server_query_endpoint;
                const std::string m_key;
                const std::string m_secret;
            public:


                
                explicit safaricom_tokens_getter(net::io_context &io
                        ,const ConsumerValues &conf)
                    :m_resolver(net::make_strand(io)),
                    m_stream(net::make_strand(io)),m_key(conf.getKey()),
                    m_secret(conf.getSecret()),m_server_query_endpoint(conf.getEndpoint()){
                }

                void run(){
                    //get url from endpoint
                    bool hasHttp =
                        boost::string_view(m_server_query_endpoint)
                            .starts_with("htpp://");
                    bool hasHttps =
                        boost::string_view(m_server_query_endpoint)
                            .starts_with("htpps://");
                    boost::string_view host="";
                    if(hasHttp || hasHttps){
                        host=m_server_query_endpoint.substr(6);
                    }else if(hasHttps){
                        host=m_server_query_endpoint.substr(7);
                    }else{
                        host=m_server_query_endpoint;
                    }
                    int end_of_host=host.find_first_of("/");
                    auto tmp=host;
                    host=host.substr(0,end_of_host);//host address
                    auto query=tmp.substr(end_of_host);// beginning of query
                    std::string host_to_use="localhost";


                    int http_version=11;
                    std::string port="80";

                    //setup the request object
                    m_request.version(http_version);
                    m_request.method(http::verb::get);
                    m_request.target(query);
                    m_request.set(http::field::host,host);

                    m_resolver.async_resolve(host_to_use,port,
                            beast::bind_front_handler(
                                &safaricom_tokens_getter::on_resolve,
                                shared_from_this()));
                }

                void on_resolve(beast::error_code ec,
                        tcp::resolver::results_type results){
                    if(ec){
                        std::cout<<"COME ON FUCK"<<std::endl;
                    }
                    std::cout<<"YEAHHHH: "<<std::endl;
                }
        };

        AccessGenerator::AccessGenerator(const ConsumerValues &conf,
                bool asyncGenerate):doAsync(asyncGenerate),
            conf(conf){
        }

        const std::string AccessGenerator::getAccessToken()const{
            return "";
        }

        //TODO this is just for tests, clean this function to do appropriate thing
        void AccessGenerator::start()const{
            net::io_context ios;
            std::make_shared<safaricom_tokens_getter>(ios,this->conf)->run();
            ios.run();
        }
    }
}
