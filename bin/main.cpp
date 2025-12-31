#include<lib/Game.h>
#include <iostream>
#include <string>

int main() {
    Game game;

    while (game.isRunning()) {
        std::string cmd_str;
        std::getline(std::cin, cmd_str);

        game.ExecuteCommand(cmd_str);
    }

    return 0;
}