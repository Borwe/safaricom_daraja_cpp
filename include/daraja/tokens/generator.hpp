#ifndef DARA_TOKENS_GENERATOR_CUSTOM
#define DARA_TOKENS_GENERATOR_CUSTOM

#include <daraja/tokens/consumer_values.hpp>
#include <mutex>
#include <string>
#include <daraja_export.h>

namespace Daraja{
    namespace tokens{

        class DARAJA_EXPORT AccessGenerator{
        private:
            //private variables
            ConsumerValues conf;
            bool doAsync;
            std::string access_token;
        public:
            //public variables
            std::mutex access_lock;
        
            AccessGenerator(const ConsumerValues &conf,bool asyncGenerate=false);

            const std::string getAccessToken() ;
            void setAccessToken(std::string &token);
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
            void start();
        };
    }
}

#endif

