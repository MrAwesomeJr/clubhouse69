#ifndef CALLBACK_HPP_DEFINED
#define CALLBACK_HPP_DEFINED

#include "cstdlib"

namespace ch69 {
    class NullCallback {
        public:
            bool operator()() {return 0;}
    };

    class HelloCallback {
        public:
            bool operator()() {
                std::cout << "hello" << std::endl;
                return 0;
            }
    };

    class ExecutableCallback {
        public:
            ExecutableCallback(std::string command)
            :
            command_(command)
            {}

            bool operator()() {
                return std::system(command_.c_str());
            }

        private:
            std::string command_;
    };
}

#endif // CALLBACK_HPP_DEFINED