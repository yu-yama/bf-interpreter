// test/instructions_test.cpp
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
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>

constexpr bool opt = false;
constexpr int nl = BFInterpreter::BF<opt>::num_lim();
constexpr int pl = BFInterpreter::BF<opt>::pt_lim();
constexpr int small = 1000;
constexpr int big = 30000;

TEST(InstructionsTest, nxt) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt> b(string(small, '>'), is, os, ds);
    static_assert(small < pl);
    b.run();
    EXPECT_EQ(small, b.mem_pt());
}

TEST(InstructionsTest, nxt_err) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt> b(string(big, '>'), is, os, ds);
    static_assert(big >= pl);
    EXPECT_THROW({
        try {
            b.run();
        } catch (const BFInterpreter::out_of_range& e) {
            EXPECT_STREQ("Pointer exceeded the right boundary of the memory.", e.what());
            throw;
        }
    }, BFInterpreter::out_of_range);
}

TEST(InstructionsTest, prv) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt> b(string(small, '>') + string(small, '<'), is, os, ds);
    static_assert(small < pl);
    b.run();
    EXPECT_EQ(0, b.mem_pt());
}

TEST(InstructionsTest, prv_err) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt> b("<", is, os, ds);
    EXPECT_THROW({
        try {
            b.run();
        } catch (const BFInterpreter::out_of_range& e) {
            EXPECT_STREQ("Pointer exceeded the left boundary of the memory.", e.what());
            throw;
        }
    }, BFInterpreter::out_of_range);
}

TEST(InstructionsTest, inc) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt>::debug = 1;
    BFInterpreter::BF<opt> b(string(small, '+'), is, os, ds);
    b.run();
    string expected = "0\n";
    for (int i = 0; i <= small; ++i) expected += to_string((i % nl)) + '\n';
    EXPECT_EQ(expected, ds.str());
}

TEST(InstructionsTest, dec) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt>::debug = 1;
    BFInterpreter::BF<opt> b(string(small, '-'), is, os, ds);
    b.run();
    string expected = "0\n";
    for (int i = 0; i <= small; ++i) expected += to_string(((-i % nl) + nl) % nl) + '\n';
    EXPECT_EQ(expected, ds.str());
}

TEST(InstructionsTest, put) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    string s;
    for (int i = 1; i <= small; ++i) s += "+.";
    BFInterpreter::BF<opt> b(s, is, os, ds);
    b.run();
    string expected;
    for (int i = 1; i <= small; ++i) expected += static_cast<char>(i % nl);
    EXPECT_EQ(expected, os.str());
}

TEST(InstructionsTest, get) {
    using namespace std;
    string in;
    for (int i = 1; i <= small; ++i) in += static_cast<char>(i % nl);
    istringstream is(in);
    ostringstream os, ds;
    BFInterpreter::BF<opt>::debug = 0;
    BFInterpreter::BF<opt> b(string(small, ','), is, os, ds);
    b.run();
    string expected;
    for (int i = 1; i <= small; ++i) expected += "(I) " + to_string(i % nl) + '\n';
    EXPECT_EQ(expected, ds.str());
}

TEST(InstructionsTest, lp) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt>::debug = 2;
    BFInterpreter::BF<opt> b(string(small, '+') + "[>+<-]", is, os, ds);
    b.run();
    string expected = "0 0\n";
    for (int i = 0; i < small; ++i) expected += to_string(i % nl) + " 0\n";
    for (int i = small % nl; i > 0; --i) for (int j = 0; j < 5; ++j) expected += to_string(i % nl) + " " + to_string((small - i + (j > 2)) % nl) + '\n';
    for (int i = 0; i < 2; ++i) expected += "0 " + to_string(small % nl) + '\n';
    EXPECT_EQ(expected, ds.str());
}

TEST(InstructionsTest, lpb_ok) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt>::debug = 1;
    BFInterpreter::BF<opt> b("+[", is, os, ds);
    b.run();
    string expected;
    for (int i = 0; i < 4; ++i) expected += to_string(i >> 1) + '\n';
    EXPECT_EQ(expected, ds.str());
}

TEST(InstructionsTest, lpb_ng) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt> b("[", is, os, ds);
    EXPECT_THROW({
        try {
            b.run();
        } catch (const BFInterpreter::syntax_error& e) {
            EXPECT_STREQ("Mismatched brackets found: at least one more [ than ]", e.what());
            throw;
        }
    }, BFInterpreter::syntax_error);
}

TEST(InstructionsTest, lpe_ok) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt>::debug = 1;
    BFInterpreter::BF<opt> b("]", is, os, ds);
    b.run();
    string expected;
    for (int i = 0; i < 3; ++i) expected += "0\n";
    EXPECT_EQ(expected, ds.str());
}

TEST(InstructionsTest, lpe_ng) {
    using namespace std;
    istringstream is;
    ostringstream os, ds;
    BFInterpreter::BF<opt> b("+]", is, os, ds);
    EXPECT_THROW({
        try {
            b.run();
        } catch (const BFInterpreter::syntax_error& e) {
            EXPECT_STREQ("Mismatched brackets found: at least one more ] than [", e.what());
            throw;
        }
    }, BFInterpreter::syntax_error);
}
