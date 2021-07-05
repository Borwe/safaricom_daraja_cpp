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

#define BOOST_TEST_MODULE CONFIGURATION_API
#include "daraja/configuration/consumer_values.hpp"
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/test/tools/interface.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <cstring>

BOOST_AUTO_TEST_CASE(read_config_file){
    //Test if file exists
    std::string testLocation(TEST_BUILD_DIR);
    std::string confFileLoc=testLocation+"/test.properties";
    BOOST_TEST(boost::filesystem::exists(confFileLoc.c_str()));

    Daraja::tokens::ConsumerValues conf=
            Daraja::tokens::ConsumerBuilder().getConsumerValuesFromFile(confFileLoc);
                
    BOOST_TEST(conf.getKey()=="nBArpApiGCMoSzRjk9afRy1VJXfmZPfw");
    BOOST_TEST(conf.getSecret()=="cCpW0YQZ8gIC6yGJ");
    BOOST_TEST(conf.getEndpoint()=="https://sandbox.safaricom.co.ke/oauth/v1/generate?grant_type=client_credentials");
    BOOST_TEST(conf.getbase64KeysAndSecret()
            =="bkJBcnBBcGlHQ01vU3pSams5YWZSeTFWSlhmbVpQZnc6Y0NwVzBZUVo4Z0lDNnlHSg==");
}
