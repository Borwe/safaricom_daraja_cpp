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
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#define BOOST_TEST_MODULE TOKEN_API
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <cstring>
#include <daraja/tokens.hpp>


BOOST_AUTO_TEST_CASE(create_access_token_non_async){
    std::string testLocation(TEST_BUILD_DIR);
    std::string confFileLoc=testLocation+"/test.properties";
    Daraja::tokens::ConsumerValues conf=
            Daraja::tokens::ConsumerBuilder().getConsumerValuesFromFile(confFileLoc);

    Daraja::tokens::AccessGenerator access_async(conf,false);
    const std::string access_token=access_async.getAccessToken();
    //sleep for 3 seconds and retry getting token which should be different
    const std::string access_token2=access_async.getAccessToken();
    BOOST_TEST(access_token.empty()==false);
    BOOST_TEST(access_token2.empty()==false);
    BOOST_TEST(access_token!=access_token2);

    BOOST_REQUIRE_THROW(access_async.start(),std::runtime_error);
}


BOOST_AUTO_TEST_CASE(create_access_token_async){
    std::string testLocation(TEST_BUILD_DIR);
    std::string confFileLoc=testLocation+"/test.properties";
    Daraja::tokens::ConsumerValues conf=
            Daraja::tokens::ConsumerBuilder().getConsumerValuesFromFile(confFileLoc);

    Daraja::tokens::AccessGenerator access_async(conf,true);
    access_async.start();
    const std::string access_token=access_async.getAccessToken();
    std::string access_token2=access_async.getAccessToken();
    std::cout<<"TK1: "<<access_token<<"\n";
    std::cout<<"TK2: "<<access_token2<<"\n";
    //sleep for 5 seconds and retry getting token which should be different
    //5 seconds is the maximum wait time 
    std::this_thread::sleep_for(std::chrono::seconds(5));
    access_token2=access_async.getAccessToken();
    BOOST_TEST(access_token.empty()==false);
    BOOST_TEST(access_token2.empty()==false);
    BOOST_TEST(access_token!=access_token2);
}
