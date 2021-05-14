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
        public:
            //public variables
        
            AccessGenerator(const ConsumerValues &conf);
            AccessGenerator(const AccessGenerator &copy)=default;
            AccessGenerator(AccessGenerator &&move)=default;
            AccessGenerator &operator=(const AccessGenerator &copy)=default;
            AccessGenerator &operator=(AccessGenerator &&move)=default;

            //TODO finish implementing this
            std::string getAccessToken();
        };
    }
}

#endif

