#ifndef DARA_TOKENS_GENERATOR_CUSTOM
#define DARA_TOKENS_GENERATOR_CUSTOM

#include <daraja/tokens/consumer_values.hpp>
#include <string>

namespace Daraja{
    namespace tokens{

        class AccessGenerator{
        private:
            //private variables
            ConsumerValues conf;
            bool doAsync;
        public:
            //public variables
        
            AccessGenerator(const ConsumerValues &conf,bool asyncGenerate=false);
            AccessGenerator(const AccessGenerator &copy)=default;
            AccessGenerator(AccessGenerator &&move)=default;
            AccessGenerator &operator=(const AccessGenerator &copy)=default;
            AccessGenerator &operator=(AccessGenerator &&move)=default;

            //TODO finish implementing this
            const std::string getAccessToken() const;
            // TODO finish it
            /**
             * If doAsync is set to true, then this initialize a loop
             * that keeps updating the access token information
             * every 3.5 seconds, or the expiry time resieved from token request
             * <br>
             * If doAsync is set to false, then this function call does nothing
             * as all the request will be done to get the access token when calling the 
             * AccessGenerator::getAccessToken() member function
             */
            void start()const;
        };
    }
}

#endif

