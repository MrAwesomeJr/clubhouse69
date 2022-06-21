#include "clubhouse.hpp"


int main(){
    ch69::Clubhouse clubhouse(1280, 720);
    bool ret = clubhouse.run();
    return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}