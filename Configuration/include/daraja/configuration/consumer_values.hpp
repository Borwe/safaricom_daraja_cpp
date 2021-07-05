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

#ifndef DARAJA_CONSUMER_VALUES_CUSTOM
#define DARAJA_CONSUMER_VALUES_CUSTOM

#include <string>
#include <daraja_configuration_export.h>

namespace Daraja{
    namespace tokens{

        class DARAJA__CONFIGURATION_EXPORT ConsumerValues{
        private:
            //private variables
            std::string m_key;
            std::string m_secret;
            std::string m_endpoint;
            std::string base64d_keys_and_secret;
        public:

            ConsumerValues(std::string key,std::string secret
                    ,std::string endpoint);
            ConsumerValues(const ConsumerValues &copy)=default;
            ConsumerValues(ConsumerValues &&move)=default;
            ConsumerValues &operator=(const ConsumerValues &copy)=default;
            ConsumerValues &operator=(ConsumerValues &&move)=default;
            ~ConsumerValues()=default;

            /**
             * Read consumer information from @file, secret and key
             */
            const std::string getKey()const;
            const std::string getSecret()const;
            const std::string getbase64KeysAndSecret()const;
            const std::string getEndpoint()const;
        };

        class DARAJA__CONFIGURATION_EXPORT ConsumerBuilder{
        private:
            //private variables
        public:
            //public variables
            ConsumerValues getConsumerValuesFromFile(std::string file) const;
        
            ConsumerBuilder()=default;
            ConsumerBuilder(const ConsumerBuilder &copy)=default;
            ConsumerBuilder(ConsumerBuilder &&move)=default;
            ConsumerBuilder &operator=(ConsumerBuilder &&move)=default;
            ~ConsumerBuilder()=default;
        };

    }
}

#endif
