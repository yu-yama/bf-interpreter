#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>

namespace BFInterpreter {
    template<bool opt = true>
    struct BF {
        using cnt_type = int;
        using debug_type = int;
    private:
        struct BFNumber {
            using num_type = int;
            constexpr static num_type num_lim = 256;
            num_type n;
            BFNumber() : n(0) {};
            void adj() {
                if ((n %= num_lim) < 0) n += num_lim;
            }
            operator bool() const {
                return n != 0;
            }
            void add() {
                if (++n >= num_lim) n -= num_lim;
            }
            void add(num_type a) {
                n += a;
                adj();
            }
            void sub() {
                if (--n < 0) n += num_lim;
            }
            void sub(num_type a) {
                n -= a;
                adj();
            }
            void put() const {
                std::cout.put(n);
                if (debug >= 0) std::cerr << "(O) " << n << '\n';
            }
            void get() {
                n = std::cin.get();
                if (debug >= 0) std::cerr << "(I) " << n << '\n';
            }
        };
        struct BFMemory {
            using mem_type = std::vector<BFNumber>;
            using pt_type = int;
            constexpr static pt_type pt_lim = 30000;
            mem_type m;
            pt_type pt;
            BFMemory() : m(pt_lim), pt(0) {};
            const auto size() const {
                return m.size();
            }
            const auto& cur() const {
                return m[pt];
            }
            auto&& cur() {
                return m[pt];
            }
            void nxt() {
                nxt(1);
            }
            void nxt(pt_type a) {
                if ((pt += a) >= pt_lim) throw std::out_of_range("Pointer exceeded the right boundary of the memory.");
            }
            void prv() {
                prv(1);
            }
            void prv(pt_type a) {
                if ((pt -= a) < 0) throw std::out_of_range("Pointer exceeded the left boundary of the memory.");
            }
        };
        BFMemory m;
        std::istream& is;
        std::vector< std::pair<char, int> > ops;
        cnt_type op_pos;
        template<typename... Ts>
        void nxt(Ts... as) {
            m.nxt(as...);
        }
        template<typename... Ts>
        void prv(Ts... as) {
            m.prv(as...);
        }
        template<typename... Ts>
        void add(Ts... as) {
            m.cur().add(as...);
        }
        template<typename... Ts>
        void sub(Ts... as) {
            m.cur().sub(as...);
        }
        void put() const {
            m.cur().put();
        }
        void get() {
            m.cur().get();
        }
        void str() {
            // Accepting mismatched brackets as long as no jump is attempted is an expected behaviour
            constexpr static auto err_msg = "Mismatched brackets found: at least one more [ than ]";
            if (m.cur()) return;
            cnt_type d = 1;
            while (d > 0) {
                if constexpr (opt) {
                    if (op_pos >= static_cast<cnt_type>(ops.size())) throw std::runtime_error(err_msg);
                    else switch (ops[op_pos++].first) {
                        case '[': ++d; break;
                        case ']': --d; break;
                    }
                } else {
                    switch (is.get()) {
                        case EOF: throw std::runtime_error(err_msg); break;
                        case '[': ++d; break;
                        case ']': --d; break;
                    }
                }
            }
        }
        void end() {
            // Accepting mismatched brackets as long as no jump is attempted is an expected behaviour
            constexpr static auto err_msg = "Mismatched brackets found: at least one more ] than [";
            if (!m.cur()) return;
            cnt_type d = -1;
            while (d < 0) {
                if constexpr (opt) {
                    if (op_pos < 0) throw std::runtime_error(err_msg);
                    else {
                        switch (ops[op_pos -= 2].first) {
                            case '[': ++d; break;
                            case ']': --d; break;
                        }
                        ++op_pos;
                    }
                } else {
                    switch (is.unget(), is.unget(), is.get()) {
                        case EOF: throw std::runtime_error(err_msg); break;
                        case '[': ++d; break;
                        case ']': --d; break;
                    }
                }
            }
        }
    public:
        constexpr static char c_nxt = '>', c_prv = '<', c_add = '+', c_sub = '-', c_put = '.', c_get = ',', c_str = '[', c_end = ']';
        static debug_type debug;
        BF(std::istream& s = std::cin) : m(), is(s), ops(), op_pos(0) {
            if constexpr (opt) {
                ops.push_back({0, 0});
                for (char c; (c = is.get()) != EOF;) {
                    switch (c) {
                        case c_nxt:
                        case c_prv:
                        case c_add:
                        case c_sub: {
                            if (ops.back().first == c) ++ops.back().second;
                            else ops.push_back({c, 1});
                            break;
                        }
                        case c_put:
                        case c_get:
                        case c_str:
                        case c_end: ops.push_back({c, 1}); break;
                    }
                }
            }
        };
        bool step() {
            for (int i = 0, l = std::min(static_cast<debug_type>(m.size()), debug); i < l; ++i) std::cerr << m.m.at(i).n << (i + 1 == l ? '\n' : ' ');
            if constexpr (opt) {
                if (op_pos >= static_cast<cnt_type>(ops.size())) return false;
                auto [op, cnt] = ops[op_pos++];
                switch (op) {
                    case c_nxt: nxt(cnt); break;
                    case c_prv: prv(cnt); break;
                    case c_add: add(cnt); break;
                    case c_sub: sub(cnt); break;
                    case c_put: put(); break;
                    case c_get: get(); break;
                    case c_str: str(); break;
                    case c_end: end(); break;
                }
                return true;
            } else {
                switch (is.get()) {
                    case EOF: is.unget(); return false; break;
                    case c_nxt: nxt(); break;
                    case c_prv: prv(); break;
                    case c_add: add(); break;
                    case c_sub: sub(); break;
                    case c_put: put(); break;
                    case c_get: get(); break;
                    case c_str: str(); break;
                    case c_end: end(); break;
                }
                return true;
            }
        }
        void run() {
            while (step()) ;
        }
    };
    template<bool opt>
    int BF<opt>::debug = -1;
}

int main(int argc, char *argv[]) {
    using namespace std;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            ifstream ifs(argv[i]);
            BFInterpreter::BF b(ifs);
            b.run();
        }
    } else {
        BFInterpreter::BF().run();
    }
    return 0;
}
