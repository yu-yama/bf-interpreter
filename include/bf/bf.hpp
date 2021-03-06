// include/bf/bf.hpp
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

#pragma once

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>

namespace BFInterpreter {
    struct out_of_range : public std::out_of_range {
        using std::out_of_range::out_of_range;
    };
    struct syntax_error : public std::logic_error {
        using std::logic_error::logic_error;
    };
    template<bool opt = true>
    struct BF {
        using cnt_type = int;
        using debug_type = int;
    private:
        struct BFNumber {
            using num_type = int;
            constexpr static num_type num_lim = 256;
            static_assert(num_lim > 0);
            num_type n;
            constexpr BFNumber() noexcept : n(0) {};
            constexpr void adj() noexcept {
                if constexpr ((num_lim & (num_lim - 1)) == 0) n &= num_lim - 1;
                else if ((n %= num_lim) < 0) n += num_lim;
            }
            constexpr operator bool() const noexcept {
                return n != 0;
            }
            constexpr void inc(num_type a) noexcept {
                n += a;
                adj();
            }
            constexpr void dec(num_type a) noexcept {
                n -= a;
                adj();
            }
            void put(std::ostream& o, std::ostream& d) const {
                o.put(static_cast<char>(n));
                if (debug >= 0) d << "(O) " << n << '\n';
            }
            void get(std::istream& i, std::ostream& d) {
                n = i.get();
                if (debug >= 0) d << "(I) " << n << '\n';
            }
        };
        struct BFMemory {
            using mem_type = std::vector<BFNumber>;
            using pt_type = int;
            constexpr static pt_type pt_lim = 30000;
            static_assert(pt_lim > 0);
            mem_type m;
            pt_type pt;
            BFMemory() : m(pt_lim), pt(0) {};
            const auto& mem() const {
                return m;
            }
            const auto& mem_pt() const {
                return pt;
            }
            auto size() const noexcept {
                return m.size();
            }
            const auto& cur() const {
                return m[pt];
            }
            auto&& cur() {
                return m[pt];
            }
            void nxt(pt_type a) {
                if ((pt += a) >= pt_lim) throw out_of_range("Pointer exceeded the right boundary of the memory.");
            }
            void prv(pt_type a) {
                if ((pt -= a) < 0) throw out_of_range("Pointer exceeded the left boundary of the memory.");
            }
        };
        BFMemory m;
        std::istream& is;
        std::ostream& os;
        std::ostream& ds;
        std::vector< std::pair<char, cnt_type> > ops;
        cnt_type op_pos;
        void nxt(cnt_type a) {
            m.nxt(a);
        }
        void prv(cnt_type a) {
            m.prv(a);
        }
        void inc(cnt_type a) noexcept {
            m.cur().inc(a);
        }
        void dec(cnt_type a) noexcept {
            m.cur().dec(a);
        }
        void put() const {
            m.cur().put(os, ds);
        }
        void get() {
            m.cur().get(is, ds);
        }
        void lpb() {
            // Accepting mismatched brackets as long as no jump is attempted is an expected behaviour
            constexpr static auto err_msg = "Mismatched brackets found: at least one more [ than ]";
            if (m.cur()) return;
            cnt_type d = 1;
            while (d > 0) {
                if (op_pos >= static_cast<cnt_type>(ops.size())) throw syntax_error(err_msg);
                else switch (ops[op_pos++].first) {
                    case '[': ++d; break;
                    case ']': --d; break;
                }
            }
        }
        void lpe() {
            // Accepting mismatched brackets as long as no jump is attempted is an expected behaviour
            constexpr static auto err_msg = "Mismatched brackets found: at least one more ] than [";
            if (!m.cur()) return;
            cnt_type d = -1;
            while (d < 0) {
                if (op_pos < 0) throw syntax_error(err_msg);
                else {
                    switch (ops[op_pos -= 2].first) {
                        case '[': ++d; break;
                        case ']': --d; break;
                    }
                    ++op_pos;
                }
            }
        }
    public:
        constexpr static char c_nxt = '>', c_prv = '<', c_inc = '+', c_dec = '-', c_put = '.', c_get = ',', c_lpb = '[', c_lpe = ']';
        constexpr static debug_type no_debug = -1;
        static_assert(no_debug < 0);
        static debug_type debug;
        BF(const std::string& s, std::istream& i = std::cin, std::ostream& o = std::cout, std::ostream& d = std::cerr) : m(), is(i), os(o), ds(d), ops({{0, 0}}), op_pos(0) {
            for (const auto& c : s) switch (c) {
                case c_nxt:
                case c_prv:
                case c_inc:
                case c_dec: {
                    if constexpr (opt) if (ops.back().first == c) {
                        ++ops.back().second;
                        break;
                    }
                    [[fallthrough]];
                }
                case c_put:
                case c_get:
                case c_lpb:
                case c_lpe: ops.push_back({c, 1}); break;
                default: if constexpr (!opt) ops.push_back({c, 0}); break;
            }
        };
        BF(std::istream& s = std::cin, std::istream& i = std::cin, std::ostream& o = std::cout, std::ostream& d = std::cerr) : BF(std::string(std::istreambuf_iterator<char>(s), {}), i, o, d) {}
        constexpr static auto num_lim() noexcept {
            return BFNumber::num_lim;
        }
        constexpr static auto pt_lim() noexcept {
            return BFMemory::pt_lim;
        }
        const auto& mem() const {
            return m.mem();
        }
        const auto& mem_pt() const {
            return m.mem_pt();
        }
        const auto& mem_size() const {
            return m.size();
        }
        const auto& mem_cur() const {
            return m.cur();
        }
        bool step() {
            for (debug_type i = 0, l = std::min(static_cast<debug_type>(m.size()), debug); i < l; ++i) ds << m.m.at(i).n << (i + 1 == l ? '\n' : ' ');
            if (op_pos >= static_cast<cnt_type>(ops.size())) return false;
            auto [op, cnt] = ops[op_pos++];
            switch (op) {
                case c_nxt: nxt(cnt); break;
                case c_prv: prv(cnt); break;
                case c_inc: inc(cnt); break;
                case c_dec: dec(cnt); break;
                case c_put: put(); break;
                case c_get: get(); break;
                case c_lpb: lpb(); break;
                case c_lpe: lpe(); break;
            }
            return true;
        }
        void run() {
            while (step()) ;
        }
    };
    template<bool opt>
    typename BF<opt>::debug_type BF<opt>::debug = BF<opt>::no_debug;
}
