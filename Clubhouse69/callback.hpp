#ifndef CALLBACK_HPP_DEFINED
#define CALLBACK_HPP_DEFINED

#include "cstdlib"

namespace ch69 {
    union Callback {
        NullCallback;
        ExecutableCallback;
    }

    class NullCallback {
        public:
            NullCallback() {}

        bool operator()() {}
    };

    class ExecutableCallback {
        public:
            ExecutableCallback(std::string command)
            :
            command_(command)
            {}

            bool operator()() {
                return std::system(command_);
            }

        private:
            std::string command_;
    };
}

#endif CALLBACK_HPP_DEFINED