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
            ExecutableCallback(const std::string& command)
            :
            command_(command)
            {}

            bool operator()() {
                // & causes the command to run asynchronously in the background, preventing the main program from locking
                return std::system((command_ + " &").c_str());
            }

        private:
            std::string command_;
    };
}

#endif // CALLBACK_HPP_DEFINED