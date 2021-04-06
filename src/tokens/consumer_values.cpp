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

#include <daraja/tokens/consumer_values.hpp>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/AutoPtr.h>
#include <Poco/Base64Encoder.h>
#include <sstream>
#include <string>

using Poco::AutoPtr;
using Poco::Util::PropertyFileConfiguration;
using Poco::Base64Encoder;

namespace Daraja{
    namespace tokens{
        ConsumerValues::ConsumerValues(std::string key,std::string secret)
            :m_key(key),m_secret(secret){
            std::stringstream chained;
            Base64Encoder base64encoder(chained);
            std::string toEncode=key+":"+secret;
            base64encoder<<toEncode.c_str();
            this->base64d_keys_and_secret=chained.str();
        }

        ConsumerValues ConsumerValues::getConsumerValuesFromFile(std::string fileName){
            AutoPtr<PropertyFileConfiguration> prop(new PropertyFileConfiguration);
            prop->load(fileName);
            std::string key=prop->getString("consumer_key");
            std::string secret=prop->getString("consumer_secret");
            return ConsumerValues(key,secret);
            
        }

        const std::string ConsumerValues::getKey()const{
            return m_key;
        }

        const std::string ConsumerValues::getSecret()const{
            return m_secret;
        }
    }
}
