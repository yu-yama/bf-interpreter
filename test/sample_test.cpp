// test/sample_test.cpp
//
// Copyright 2021 yu-yama
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "bf/bf.hpp"
#include <gtest/gtest.h>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <sstream>

template <typename opt>
struct SampleTest : public ::testing::Test{};

using SampleTest_types = ::testing::Types< std::integral_constant<bool, false>, std::integral_constant<bool, true> >;

TYPED_TEST_SUITE(SampleTest, SampleTest_types);

TYPED_TEST(SampleTest, file) {
    using namespace std;
    constexpr static bool opt = TypeParam::value;
    cerr << "opt: " << boolalpha << opt << '\n';
    #ifdef BF_SAMPLE_SRC_DIR
        string fp = BF_SAMPLE_SRC_DIR;
    #else
        string fp = "../sample";
    #endif
    filesystem::directory_iterator f(fp);
    for (const auto& s : f) {
        auto sp = s.path();
        if (sp.extension() != ".bf") continue;
        cerr << "testing: " << sp << '\n';
        ifstream ss(sp);
        auto ip = sp, oep = sp, dep = sp;
        ip.replace_extension(".in.txt");
        oep.replace_extension(".out.txt");
        dep.replace_extension(string(".debug") + static_cast<char>('0' + opt) + ".txt");
        ifstream ifs(ip), oefs(oep), defs(dep);
        if (defs) {
            defs >> BFInterpreter::BF<opt>::debug;
            defs.get(); // remove delimiter
        } else BFInterpreter::BF<opt>::debug = BFInterpreter::BF<opt>::no_debug;
        auto to_string = [](istream& str) {
            return str ? string(istreambuf_iterator<char>(str), {}) : "";
        };
        istringstream is(to_string(ifs));
        ostringstream os, ds;
        BFInterpreter::BF<opt> b(ss, is, os, ds);
        b.run();
        EXPECT_EQ(to_string(oefs), os.str());
        EXPECT_EQ(to_string(defs), ds.str());
        ss.close();
        ifs.close();
        oefs.close();
        defs.close();
    }
}
