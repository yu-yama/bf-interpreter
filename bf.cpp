#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>

namespace BFInterpreter {
    class BF {
        struct BFNumber {
            using num_type = int;
            constexpr static num_type num_lim = 256;
            num_type n;
            BFNumber() : n(0) {};
            operator bool() const {
                return n != 0;
            }
            void add() {
                if (++n >= num_lim) n -= num_lim;
            }
            void sub() {
                if (--n < 0) n += num_lim;
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
                if (++pt >= pt_lim) throw std::out_of_range("Pointer exceeded the right boundary of the memory.");
            }
            void prv() {
                if (--pt < 0) throw std::out_of_range("Pointer exceeded the left boundary of the memory.");
            }
        };
        BFMemory m;
        std::istream& ops;
        void nxt() {
            m.nxt();
        }
        void prv() {
            m.prv();
        }
        void add() {
            m.cur().add();
        }
        void sub() {
            m.cur().sub();
        }
        void put() const {
            m.cur().put();
        }
        void get() {
            m.cur().get();
        }
        void str() {
            // Accepting mismatched brackets as long as no jump is attempted is an expected behaviour
            if (m.cur()) return;
            dep_type d = 1;
            while (d > 0) {
                switch (ops.get()) {
                    case EOF: throw std::runtime_error("Mismatched brackets found: at least one more [ than ]"); break;
                    case '[': ++d; break;
                    case ']': --d; break;
                }
            }
        }
        void end() {
            // Accepting mismatched brackets as long as no jump is attempted is an expected behaviour
            if (!m.cur()) return;
            dep_type d = -1;
            while (d < 0) {
                switch (ops.unget(), ops.unget(), ops.get()) {
                    case EOF: throw std::runtime_error("Mismatched brackets found: at least one more ] than ["); break;
                    case '[': ++d; break;
                    case ']': --d; break;
                }
            }
        }
    public:
        using dep_type = int;
        using debug_type = int;
        static debug_type debug;
        BF(std::istream& s = std::cin) : m(), ops(s) {};
        bool step() {
            for (int i = 0, l = std::min(static_cast<debug_type>(m.size()), debug); i < l; ++i) std::cerr << m.m.at(i).n << (i + 1 == l ? '\n' : ' ');
            switch (ops.get()) {
                case EOF: ops.unget(); return false; break;
                case '>': nxt(); break;
                case '<': prv(); break;
                case '+': add(); break;
                case '-': sub(); break;
                case '.': put(); break;
                case ',': get(); break;
                case '[': str(); break;
                case ']': end(); break;
            }
            return true;
        }
        void run() {
            while (step()) ;
        }
    };
    int BF::debug = -1;
}

int main(int argc, char *argv[]) {
    using namespace std;
    if (argc > 1) {
        ifstream ifs(argv[1]);
        BFInterpreter::BF b(ifs);
        b.run();
    } else {
        BFInterpreter::BF().run();
    }
    return 0;
}
