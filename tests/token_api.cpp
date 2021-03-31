//Copyright 2021 Brian Orwe
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS, //WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  //See the License for the specific language governing permissions and //limitations under the License.  #include <cstdio> #define CATCH_CONFIG_MAIN
#include "daraja/tokens/consumer_values.hpp"
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <daraja/tokens.hpp>
#ifndef TEST_BUILD_DIR
#define TEST_BUILD_DIR "PLACE_HOLDER_FOR_CMAKE"
#endif
#include <fstream>

TEST_CASE("Generate tokens","[tokens]"){
    std::string testLocation(TEST_BUILD_DIR);
    std::string confFileLoc=testLocation+"/test.properties";
    int result=std::ifstream(confFileLoc.c_str()).good();
    REQUIRE(result==1);

    GIVEN("Conf file at: "+confFileLoc){

        Daraja::tokens::ConsumerValues conf=
                Daraja::tokens::
                    ConsumerValues::
                    getConsumerValuesFromFile(confFileLoc);
    }
    
    GIVEN("ConsumerValues Object"){

    //const std::string key=conf.getKey();
    //const std::string secret=conf.getSecret();
    //std::cout<<"KEY: "<<key<<std::endl;
    //std::cout<<"SECRET: "<<secret<<std::endl;
    }

}
