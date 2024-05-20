//
// Created by marcel on 20.05.24.
//

#include <memory>
#include <string>
#include <vector>
#include "../game/Game.h"

namespace games
{
    static std::vector<std::shared_ptr<Game>> games;

    void add(Game* game)
    {
        games.emplace_back(game);
    }

    std::vector<std::string> get_game_names()
    {
        std::vector<std::string> game_name_list;

        for (const std::shared_ptr<Game>& game : games)
            game_name_list.push_back(game->get_game_name());

        return game_name_list;
    }

    std::shared_ptr<Game> get_game(int index)
    {
        return games.at(index);
    }
}
