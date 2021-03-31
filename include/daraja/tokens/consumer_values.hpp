#ifndef DARAJA_CONSUMER_VALUES_CUSTOM
#define DARAJA_CONSUMER_VALUES_CUSTOM

#include <string>

namespace Daraja{
    namespace tokens{

        class ConsumerValues{
        private:
            //private variables
            std::string m_key;
            std::string m_secret;
            std::string base64d_keys_and_secret;
        public:

            ConsumerValues(std::string key,std::string secret);
            ConsumerValues(const ConsumerValues &copy)=default;
            ConsumerValues(ConsumerValues &&move)=default;
            ConsumerValues &operator=(const ConsumerValues &copy)=default;
            ConsumerValues &operator=(ConsumerValues &&move)=default;
            ~ConsumerValues()=default;

            /**
             * Read consumer information from @file, secret and key
             */
            static ConsumerValues getConsumerValuesFromFile(std::string file);
            const std::string getKey()const;
            const std::string getSecret()const;
        };
    }
}

#endif
