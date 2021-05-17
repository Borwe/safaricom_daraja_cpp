//Copyright (C) 2021  Brian Orwe
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/beast/core/detail/base64.hpp>
#include <daraja/tokens/consumer_values.hpp>
#include <string>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/beast.hpp>


namespace Daraja{
    namespace tokens{
        ConsumerValues::ConsumerValues(std::string key,std::string secret,std::string endpoint)
            :m_key(key),m_secret(secret),m_endpoint(endpoint){
            std::string toEncode=m_key+":"+m_secret;
            size_t encodeSize=boost::beast::detail::base64::encoded_size(toEncode.size());

            char chained [encodeSize];
            boost::beast::detail::base64::encode(chained,toEncode.c_str(),toEncode.size());
            chained[encodeSize]='\0';
            this->base64d_keys_and_secret=chained;
        }

        ConsumerValues ConsumerValues::getConsumerValuesFromFile(std::string fileName){
            boost::property_tree::ptree config;
            boost::property_tree::ini_parser::read_ini(fileName,config);
            std::string key=config.get<std::string>("consumer_key");
            std::string secret=config.get<std::string>("consumer_secret");
            std::string endpoint=config.get<std::string>("token_endpoint");
            return ConsumerValues(key,secret,endpoint);
        }

        const std::string ConsumerValues::getKey()const{
            return m_key;
        }

        const std::string ConsumerValues::getSecret()const{
            return m_secret;
        }

        const std::string ConsumerValues::getbase64KeysAndSecret()const{
            return base64d_keys_and_secret;
        }

        const std::string ConsumerValues::getEndpoint()const{
            return m_endpoint;
        }
    }
}
