#ifndef INPUT_HPP_INCLUDED
#define INPUT_HPP_INCLUDED

#include <GLFW/glfw3.h>
#include <functional>

namespace ch69 {
    class Input{
        public:
            void reset_inputs() {
                // trigger once per loop
                // sets all lists of inputs back to 0
                for(std::size_t i = 0; i < sizeof(pressed_keyboard_buttons_) / sizeof(*pressed_keyboard_buttons_); i++) {
                    pressed_keyboard_buttons_[i] = 0;
                }
                for(std::size_t i = 0; i < sizeof(released_keyboard_buttons_) / sizeof(*released_keyboard_buttons_); i++) {
                    released_keyboard_buttons_[i] = 0;
                }
                for(std::size_t i = 0; i < sizeof(pressed_mouse_buttons_) / sizeof(*pressed_mouse_buttons_); i++) {
                    pressed_mouse_buttons_[i] = 0;
                }
                for(std::size_t i = 0; i < sizeof(scroll_distance_) / sizeof(*scroll_distance_); i++) {
                    scroll_distance_[i] = 0;
                }
            }

            bool mouse_button_pressed(int button) {
                return pressed_mouse_buttons_[button] == 1;
            }

            bool key_pressed(int button) {
                const auto pressed = std::find(std::begin(pressed_keyboard_buttons_), std::end(pressed_keyboard_buttons_), button) != std::end(pressed_keyboard_buttons_);
                return pressed;
            }

            bool key_released(int button){
                const auto pressed = std::find(std::begin(released_keyboard_buttons_), std::end(released_keyboard_buttons_), button) != std::end(released_keyboard_buttons_);
                return pressed;
            }

            void get_mouse_pos(GLFWwindow& window, double mouse_x, double mouse_y) {
                glfwGetCursorPos(&window, &mouse_x, &mouse_y);
            }

            const double* get_scroll_distance() const {
                return scroll_distance_;
            }

            void key_callback(GLFWwindow& window, int key, int scancode, int action, int mods) {
                if(glfwGetWindowAttrib(&window, GLFW_FOCUSED)){
                    if(action == GLFW_PRESS){
                        // add to first empty element
                        for(std::size_t i = 0; i < sizeof(pressed_keyboard_buttons_) / sizeof(*pressed_keyboard_buttons_); i++) {
                            if(pressed_keyboard_buttons_[i] == 0) {
                                pressed_keyboard_buttons_[i] = key;
                                break;
                            }
                        }
                    } else if(action == GLFW_RELEASE) {
                        // add to first empty element
                        for(std::size_t i = 0; i < sizeof(released_keyboard_buttons_) / sizeof(*released_keyboard_buttons_); i++) {
                            if(released_keyboard_buttons_[i] == 0) {
                                released_keyboard_buttons_[i] = key;
                                break;
                            }
                        }
                    }
                }
            }

            void mouse_button_callback(GLFWwindow& window, int button, int action, int mods) {
                if(glfwGetWindowAttrib(&window, GLFW_FOCUSED)) {
                    if(action == GLFW_PRESS) {
                        // write to sorted element
                        pressed_mouse_buttons_[button] = 1;
                    } else if(action == GLFW_RELEASE) {
                        // write to sorted element
                        pressed_mouse_buttons_[button] = -1;
                    }
                }
            }

            void scroll_callback(GLFWwindow& window, double x_offset, double y_offset){
                if(glfwGetWindowAttrib(&window, GLFW_FOCUSED)){
                    scroll_distance_[0] += x_offset;
                    scroll_distance_[1] += y_offset;
                }
            }


        private:
            // https://www.glfw.org/docs/3.3/group__keys.html defines 121 unique keys (including GLFW_KEY_UNKNOWN)
            int pressed_keyboard_buttons_[121];
            int released_keyboard_buttons_[121];
            // https://www.glfw.org/docs/latest/group__buttons.html has 8 unique buttons 0-7
            // this means that mouse_buttons is sorted for more efficiency, and only uses 0 and 1 (indicator is index)
            int pressed_mouse_buttons_[8];

            double scroll_distance_[2];

            // at the moment modifier keys are ignored
            // glfw also supports joysticks
    };
};

#endif // INPUT_HPP_INCLUDED