#ifndef WINDOWMANAGER_HPP_INCLUDED
#define WINDOWMANAGER_HPP_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "input.hpp"

namespace ch69{
    class WindowManager{
        public:
            WindowManager(const std::string& title, int window_width, int window_height)
            :
            title_(title),
            window_width_(window_width),
            window_height_(window_height),
            window_(*glfwCreateWindow(window_width, window_height, title.c_str(), NULL, NULL))
            {
                glfwSetWindowUserPointer( &window_, this );
                glfwMakeContextCurrent(&window_);

                glewInit();

                glfwSetMouseButtonCallback(&window_, mouse_button_callback);
                glfwSetKeyCallback(&window_, key_callback);
                glfwSetScrollCallback(&window_, scroll_callback);
            }

            GLFWwindow& get_window(){
                return window_;
            }

            Input& get_input(){
                return input_;
            }



        private:

            static void mouse_button_callback( GLFWwindow* window, int button, int action, int mods) {
                static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->input_.mouse_button_callback(*window, button, action, mods);
            };

            static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods) {
                static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->input_.key_callback(*window, key, scancode, action, mods);
            };

            static void scroll_callback(GLFWwindow* window, double mouse_x, double mouse_y) {
                static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->input_.scroll_callback(*window, mouse_x, mouse_y);
            };

            GLFWwindow& window_;
            std::string title_;
            int window_width_, window_height_;
            Input input_;

    };
}

#endif //WINDOWMANAGER_HPP_INCLUDED