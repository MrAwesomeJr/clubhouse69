#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "opengltextures.hpp"
#include "windowmanager.hpp"
#include "directorymanager.hpp"
#include "text.hpp"
#include "resources/font/menlo/menlo.hpp"
#include "callback.hpp"

namespace ch69{
    class Clubhouse{
        public:
            Clubhouse(int window_width, int window_height)
            :
            glfw_init_(glfwInit()),
            window_manager_("Clubhouse69", window_width, window_height),
            menlo_(Text(font_Menlo, "./resources/font/menlo/menlo.png"))
            {
                directory_manager_.get_games(games_);
                glOrtho(0, window_width, window_height, 0, -1, 1);

                for (int i = 0; i < directory_manager_.games_count; i++) {
                    games_buttons_.push_back(Button(games_[i].get_button_128(), window_manager_.get_input()));
                }
            }

            ~Clubhouse() {
                glfwTerminate();
            }

            int run() {
                set_state_(Menu);

                while (state_ == Menu && !glfwWindowShouldClose(&window_manager_.get_window())){
                    render_menu_();
                }

                return 0;
            }

        private:
            bool glfw_init_, glew_init_;
            double menu_scroll_distance_ = 0;
            std::vector<Game> games_;
            std::vector<Button> games_buttons_;
            WindowManager window_manager_;
            DirectoryManager directory_manager_;
            Text menlo_;

            enum State_ {Menu};
            State_ state_ = Menu;

            void set_state_(State_ state) {
                switch (state) {
                    case Menu:
                        for (int i = 0; i < directory_manager_.games_count; i++) {
                            games_buttons_[i].set_click_callback(games_[i].get_local_execute());
                        }
                }
            }

            void update_screen_() {
                glFlush();
                glfwSwapBuffers(&window_manager_.get_window());
                window_manager_.get_input().reset_inputs();
                glfwPollEvents();
            }

            void render_menu_() {
                update_screen_();
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0, 0, 0, 1.0);
                glColor3f(1,1,1);

                // display menu buttons
                menu_scroll_distance_ += window_manager_.get_input().get_scroll_distance()[1];
                menlo_.render_centered_text(window_manager_.get_window(), 640, 120+menu_scroll_distance_, 64, "Clubhouse69");

                for(int i = 0; i < directory_manager_.games_count; i++) {
                    games_buttons_[i].set_coords(240+(i%3)*320, menu_scroll_distance_+240+((int)i/3)*320);
                    games_buttons_[i].set_size(128, 128);
                    games_buttons_[i].set_vertices_as_rectangle();
                    games_buttons_[i].draw(window_manager_.get_window());
                }
            }
    };
}