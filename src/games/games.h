//
// Created by marcel on 20.05.24.
//

#ifndef GAMES_H
#define GAMES_H
#include <string>
#include <vector>

class Game;

namespace games
{
    void add(Game* game);
    std::vector<std::string> get_game_names();
    std::shared_ptr<Game> get_game(int index);
}

#endif //GAMES_H
