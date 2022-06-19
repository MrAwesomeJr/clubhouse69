#ifndef BUTTON_HPP_INCLUDED
#define BUTTON_HPP_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "input.hpp"

namespace ch69 {
    class ButtonTexture {
        public:
            ButtonTexture(ImageTexture idle_texture, ImageTexture hover_texture, ImageTexture click_texture)
            :
            idle_texture_(idle_texture),
            hover_texture_(hover_texture),
            click_texture_(click_texture)
            {}

            ImageTexture& get_idle_texture() {
                return idle_texture_;
            }

            ImageTexture& get_hover_texture() {
                return hover_texture_;
            }

            ImageTexture& get_click_texture() {
                return click_texture_;
            }

        private:
            ImageTexture idle_texture_, hover_texture_, click_texture_;
    };

    class Button {
        public:
            Button(ImageTexture idle_texture, ImageTexture hover_texture, ImageTexture click_texture, const Input& input, int (*click_callback)() = []{return 0;})
            :
            input_(input),
            idle_texture_(idle_texture),
            hover_texture_(hover_texture),
            click_texture_(click_texture),
            click_callback_(click_callback)
            {}

            Button(ButtonTexture& button_texture, const Input& input, int (*click_callback)() = []{return 0;})
            :
            input_(input),
            idle_texture_(button_texture.get_idle_texture()),
            hover_texture_(button_texture.get_hover_texture()),
            click_texture_(button_texture.get_click_texture()),
            click_callback_(click_callback)
            {}

            void draw(GLFWwindow& window, bool blend_flag = true){
                double mouse_x, mouse_y;
                input_.get_mouse_pos(window, mouse_x, mouse_y);
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

            void draw_unclickable(GLFWwindow& window, bool blend_flag = true){
                double mouse_x, mouse_y;
                input_.get_mouse_pos(window, mouse_x, mouse_y);
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

            void set_click_callback(int (*click_callback)()) {
                click_callback_ = click_callback;
            }

            int get_click_callback() {
                return (*click_callback_)();
            }

            void move(double d_x, double d_y){
                idle_texture_.get_params().move(d_x, d_y);
                hover_texture_.get_params().move(d_x, d_y);
                click_texture_.get_params().move(d_x, d_y);
            }

            void set_coords(double coords_x, double coords_y){
                idle_texture_.get_params().set_coords(coords_x, coords_y);
                hover_texture_.get_params().set_coords(coords_x, coords_y);
                click_texture_.get_params().set_coords(coords_x, coords_y);
            }

            void set_size(int size_x, int size_y){
                idle_texture_.get_params().set_size(size_x, size_y);
                hover_texture_.get_params().set_size(size_x, size_y);
                click_texture_.get_params().set_size(size_x, size_y);
            }

            void set_resolution(int resolution_x, int resolution_y){
                idle_texture_.get_params().set_resolution(resolution_x, resolution_y);
                hover_texture_.get_params().set_resolution(resolution_x, resolution_y);
                click_texture_.get_params().set_resolution(resolution_x, resolution_y);
            }

            void set_vertices(const std::vector<GLfloat>& vertices) {
                idle_texture_.get_params().set_vertices(vertices);
                hover_texture_.get_params().set_vertices(vertices);
                click_texture_.get_params().set_vertices(vertices);
            }

            void set_indices(const std::vector<GLubyte>& indices) {
                idle_texture_.get_params().set_indices(indices);
                hover_texture_.get_params().set_indices(indices);
                click_texture_.get_params().set_indices(indices);
            }

            void set_tex_coord(const std::vector<GLfloat>& tex_coord) {
                idle_texture_.get_params().set_tex_coord(tex_coord);
                hover_texture_.get_params().set_tex_coord(tex_coord);
                click_texture_.get_params().set_tex_coord(tex_coord);
            }

            void set_vertices_as_rectangle(){
                idle_texture_.get_params().set_vertices_as_rectangle();
                hover_texture_.get_params().set_vertices_as_rectangle();
                click_texture_.get_params().set_vertices_as_rectangle();
            }

        private:
            Input input_;
            ImageTexture idle_texture_, hover_texture_, click_texture_;
            int (*click_callback_)();
    };
}

#endif // BUTTON_HPP_INCLUDED