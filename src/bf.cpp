// src/bf.cpp
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
#include <fstream>

int main(int argc, char *argv[]) {
    using namespace std;
    int err_cnt = 0;
    if (argc > 1) for (int i = 1; i < argc; ++i) {
        ifstream ifs(argv[i]);
        if (ifs) {
            BFInterpreter::BF<> b(ifs);
            b.run();
        } else ++err_cnt, std::cerr << "An error occurred while reading file " << argv[i] << "; the file possibly does not exist.\n"; // Reporting, but not immediately throwing an exception in case of an input error is an expected behaviour
        ifs.close();
    } else BFInterpreter::BF<>().run();
    if (err_cnt) throw ios_base::failure(to_string(err_cnt) + " input error(s) occurred.");
    return 0;
}
