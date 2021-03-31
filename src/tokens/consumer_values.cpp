#include <daraja/tokens/consumer_values.hpp>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/AutoPtr.h>
#include <string>

using Poco::AutoPtr;
using Poco::Util::PropertyFileConfiguration;

namespace Daraja{
    namespace tokens{
        ConsumerValues::ConsumerValues(std::string key,std::string secret)
            :m_key(key),m_secret(secret){
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
