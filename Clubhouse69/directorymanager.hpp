#ifndef DIRECTORYMANAGER_HPP_INCLUDED
#define DIRECTORYMANAGER_HPP_INCLUDED

#include <dirent.h>
#include <string>
#include "opengltextures.hpp"
#include "button.hpp"
#include "callback.hpp"

namespace ch69{
    class Game {
        public:
            Game(std::string name, ButtonTexture button_128,  std::string local_executable_path)
            :
            name_(name),
            button_128_(button_128),
            local_execute_(local_executable_path)
            {}

            const std::string& get_name() {
                return name_;
            }

            ButtonTexture& get_button_128() {
                return button_128_;
            }

            const ExecutableCallback& get_local_execute() {
                return local_execute_;
            }

        private:
            const std::string name_;
            ButtonTexture button_128_;
            const ExecutableCallback local_execute_;
    };

    class DirectoryManager{
        public:
            // TODO: Needs error handling for missing files/directories, symlinks and file type of DT_UNKNOWN

            int games_count;

            DirectoryManager() {
                games_count = count_games();
            }

            int count_games() {
                DIR* games_directory = opendir(path_.c_str());

                int games_count = 0;
                struct dirent* entry;
                if (games_directory != NULL) {
                    while ((entry = readdir(games_directory)) != NULL) {
                        if (entry->d_type == DT_DIR) {
                            // check if dir is not . or ..
                            if (strcmp( entry->d_name, "." ) && strcmp( entry->d_name, ".." )) {
                                games_count++;
                            }
                        }
                    }
                    closedir(games_directory);
                } else {
                    std::cerr << "games directory not found" << std::endl;
                    games_count = 0;
                }
                return games_count;
            }

            void get_game_names(std::vector<std::string>& output_list) {
                output_list.clear();

                DIR* games_directory = opendir(path_.c_str());
                struct dirent* entry;
                while ((entry = readdir(games_directory)) != NULL) {
                    if (entry->d_type == DT_DIR) {
                       // check if dir is not . or ..
                        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
                            output_list.push_back(entry->d_name);
                        }
                    }
                }
               closedir(games_directory);
            }

            void get_local_executables(std::vector<std::string>& output_list) {
                output_list.clear();

                std::vector<std::string> games;

                get_game_names(games);
                for (int i = 0; i < games_count; i++) {
                    if (file_exists_in_directory("main", path_ + games[i] + "/local")) {
                        output_list.push_back(path_ + games[i] + "/local/main");
                    } else if (file_exists_in_directory("main.py", path_ + games[i] + "/local")) {
                        output_list.push_back("python3 " + path_ + games[i] + "/local/main.py");
                    }
                }
            }

            void get_button_textures(int resolution, std::vector<ButtonTexture>& output_list) {
                // TODO: fix
                // icons default to idle when other textures don't exist.
                // icons are found in game/icon/
                output_list.clear();
                std::vector<std::string> games;
                get_game_names(games);

                ImageTexture idle_texture = ImageTexture("./resources/images/tps.png");
                ImageTexture hover_texture = ImageTexture("./games/a/icon/128_idle.png");
                ImageTexture click_texture = ImageTexture("./games/pyfrick/icon/128_idle.png");

                for (int i = 0; i < games_count; i++) {
//                    idle_texture = get_idle_texture(resolution, games[i]);
//                    hover_texture = get_idle_texture(resolution, games[i]);
//                    click_texture = get_idle_texture(resolution, games[i]);
//                    if (file_exists_in_directory(std::to_string(resolution) + "_hover.png", path_ + games[i])) {
//                        ImageTexture hover_texture = get_hover_texture(resolution, games[i]);
//                    } else {
//                        ImageTexture hover_texture = get_idle_texture(resolution, games[i]);
//                    }
//                    if (file_exists_in_directory(std::to_string(resolution) + "_click.png", path_ + games[i])) {
//                        ImageTexture click_texture = get_click_texture(resolution, games[i]);
//                    } else {
//                        ImageTexture click_texture = get_idle_texture(resolution, games[i]);
//                    }

                    output_list.push_back(ButtonTexture(idle_texture, hover_texture, click_texture));
                }
            }

            ImageTexture get_idle_texture(int resolution, std::string game) {
                std::string icon_path;
                icon_path = path_;
                icon_path += game;
                icon_path += "/icon/";
                icon_path += std::to_string(resolution);
                icon_path += "_idle.png";
                ImageTexture icon_texture = ImageTexture(icon_path.c_str());
                //  4 games per row, 160 pixels per image, offset of 240 away from side, 160 pixels between images.
                // location of icons relative to the screen are calculated on render.
                icon_texture.get_params().set_size(resolution, resolution);
                return icon_texture;
            }

            ImageTexture get_hover_texture(int resolution, std::string game) {
                std::string icon_path;
                icon_path = path_;
                icon_path += game;
                icon_path += "/icon/";
                icon_path += std::to_string(resolution);
                icon_path += "_hover.png";
                ImageTexture icon_texture = ImageTexture(icon_path.c_str());
                //  4 games per row, 160 pixels per image, offset of 240 away from side, 160 pixels between images.
                // location of icons relative to the screen are calculated on render.
                icon_texture.get_params().set_size(resolution, resolution);
                return icon_texture;
            }

            ImageTexture get_click_texture(int resolution, std::string game) {
                std::string icon_path;
                icon_path = path_ + game + "/icon/" + std::to_string(resolution) + "_click.png";
                ImageTexture icon_texture = ImageTexture(icon_path.c_str());
                //  4 games per row, 160 pixels per image, offset of 240 away from side, 160 pixels between images.
                // location of icons relative to the screen are calculated on render.
                icon_texture.get_params().set_size(resolution, resolution);
                return icon_texture;
            }

            void get_games(std::vector<Game>& output_list) {
                output_list.clear();

                std::vector<std::string> names;
                std::vector<std::string> executables;
                std::vector<ButtonTexture> buttons;
                get_game_names(names);
                get_button_textures(128, buttons);
                get_local_executables(executables);

                for (int i = 0; i < games_count; i++) {
                    output_list.push_back(Game(names[i], buttons[i], executables[i]));
                }
            }

            bool file_exists_in_directory(const std::string& file_name, const std::string& path, bool recursive = false) {
                bool return_value = false;
                DIR* directory = opendir(path.c_str());
                struct dirent* entry;
                unsigned char file_type;
                while ((entry = readdir(directory)) != NULL) {
                    if (entry->d_type == DT_REG) {
                        // check if dir is not . or .., returns 0 when identical
                        if (!(strcmp(entry->d_name, file_name.c_str()))) {
                            return_value = true;
                        }
                    } else if (recursive && entry->d_type == DT_DIR) {
                        // check if dir is not . or .., returns 0 when identical
                        if ((strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".DS_Store"))) {

                            return_value = file_exists_in_directory(file_name, (path + std::string(entry->d_name)));
                        }
                    }
                    if (return_value) {
                        break;
                    }
                }
                return return_value;
            }

        private:
            const std::string path_ = "./games/";

            // unsigned char replace_symlink_() {}
    };
}

#endif //DIRECTORYMANAGER_HPP_INCLUDED