#ifndef DIRECTORYMANAGER_HPP_INCLUDED
#define DIRECTORYMANAGER_HPP_INCLUDED

#include <dirent.h>
#include <string>
#include "opengltextures.hpp"

namespace ch69{
    class DirectoryManager{
        public:
            int games_count;
            std::string path = "./games/";

            int count_games(){
                DIR *games_directory = opendir("./games");
                games_count = 0;
                struct dirent * entry;
                while ((entry = readdir(games_directory)) != NULL) {
                    if (entry->d_type == DT_DIR) {
                        // check if dir is not . or ..
                        if (strcmp( entry->d_name, "." ) && strcmp( entry->d_name, ".." )){
                            games_count++;
                        }
                    }
                }
                closedir(games_directory);
                return games_count;
            }

            void get_icon_textures(int resolution, std::vector<ImageTexture>& output_list){
                std::string icon_path;
                DIR *games_directory = opendir("./games");
                struct dirent * entry;
                while ((entry = readdir(games_directory)) != NULL) {
                    if (entry->d_type == DT_DIR) {
                        // check if dir is not . or .., returns 0 when identical
                        if ((strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".DS_Store"))){
                            icon_path = "./games/";
                            icon_path += entry->d_name;
                            icon_path += "/icon/";
                            icon_path += std::to_string(resolution);
                            icon_path += ".png";
                            ImageTexture icon_texture = ImageTexture(icon_path.c_str());
                            //  4 games per row, 160 pixels per image, offset of 240 away from side, 160 pixels between images.
                            // location of icons relative to the screen are calculated on render.
                            icon_texture.get_params().set_size(160, 160);
                            output_list.push_back(icon_texture);
                        }
                    }
                }
                closedir(games_directory);
            }
    };
}

#endif //DIRECTORYMANAGER_HPP_INCLUDED