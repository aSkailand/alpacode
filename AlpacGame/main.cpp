#include <iostream>
#include <plog\log.h>
#include <SFML/Graphics.hpp>

int main() {
        
    plog::init(plog::debug,"Log.log");
    LOGI << "Initiating plog.";

    std::cout << "Hello, World!" << std::endl;
    return 0;
}