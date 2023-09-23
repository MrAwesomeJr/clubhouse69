#ifndef BUTTON_HPP_INCLUDED
#define BUTTON_HPP_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "input.hpp"

namespace ch69 {
    class ButtonTexture {
        public:
            ButtonTexture(idle_texture, hover_texture, click_texture)
            :
            idle_texture_(idle_texture),
            hover_texture_(hover_texture),
            click_texture_(click_texture)
            {}

            const ImageTexture& get_idle_texture() {
                return idle_texture_;
            }

            const ImageTexture& get_hover_texture() {
                return hover_texture_;
            }

            const ImageTexture& get_click_texture() {
                return click_texture_;
            }

        private:
            ImageTexture idle_texture_, hover_texture_, click_texture_;

    }

    class Button {
        public:
            Button(const ImageTexture& idle_texture, const ImageTexture& hover_texture, const ImageTexture& click_texture, const Input& input, void (*click_callback)())
            :
            input_(input),
            idle_texture_(idle_texture),
            hover_texture_(hover_texture),
            click_texture_(click_texture),
            click_callback_(click_callback)
            {}

            Button(const ButtonTexture& button_texture, const Input& input, void (*click_callback)())
            :
            input_(input),
            idle_texture_(button_texture.get_idle_texture()),
            hover_texture_(button_texture.get_hover_texture()),
            click_texture_(button_texture.get_click_texture()),
            click_callback_(click_callback)
            {}

            void draw(const GLFWwindow& window, bool blend_flag = true){
                double mouse_x, mouse_y;
                input_.get_mouse_pos(mouse_x, mouse_y);
                // questioning use of directly accessing elements
                if (mouse_x >= idle_texture_.get_params().get_coords()[0] &&
                    mouse_x <= idle_texture_.get_params().get_coords()[0] + idle_texture_.get_params().get_size()[0] &&
                    mouse_y >= idle_texture_.get_params().get_coords()[1] &&
                    mouse_y <= idle_texture_.get_params().get_coords()[1] + idle_texture_.get_params().get_size()[1]) {

                    if (input_.mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
                        click_texture_.draw(window, blend_flag);
                        click_callback_();
                    } else {
                        hover_texture_.draw(window, blend_flag);
                    }
                } else {
                    idle_texture_.draw(window, blend_flag);
                };
            }

            void draw_unclickable(const GLFWwindow& window, bool blend_flag = true){
                double mouse_x, mouse_y;
                input_.get_mouse_pos(mouse_x, mouse_y);
                if (mouse_x >= idle_texture_.get_params().get_coords()[0] &&
                    mouse_x <= idle_texture_.get_params().get_coords()[0] + idle_texture_.get_params().get_size()[0] &&
                    mouse_y >= idle_texture_.get_params().get_coords()[1] &&
                    mouse_y <= idle_texture_.get_params().get_coords()[1] + idle_texture_.get_params().get_size()[1]) {

                    if (input_.mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
                        click_texture_.draw(window, blend_flag);
                        click_callback_();
                    } else {
                        hover_texture_.draw(window, blend_flag);
                    }
                } else {
                    idle_texture_.draw(window, blend_flag);
                };
            }

        private:
            Input input_;
            ImageTexture idle_texture_, hover_texture_, click_texture_;
            void (*click_callback_)();
    };
}

#endif // BUTTON_HPP_INCLUDED