#include <iostream>
#include <chrono>
#include <thread>
int main(){
    std::cout << "welcome 2 coinflip" << std::endl << "pls wait a couple secs" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int randint = rand() % 2;
    std::cout << ((randint == 0) ? "heads" : "tails");
}