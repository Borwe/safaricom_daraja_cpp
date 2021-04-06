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
