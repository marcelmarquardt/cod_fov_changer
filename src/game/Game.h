//
// Created by marcel on 19.05.24.
//

#ifndef GAME_H
#define GAME_H
#include <cstdint>
#include <optional>
#include <string>

class Game
{
public:
    Game(std::string  game_name,
        std::string  process_name,
        uintptr_t fov_address,
        uintptr_t fov_offset);

    [[nodiscard]] const std::string& get_game_name() const;
    bool initialize();
    void reset();
    [[nodiscard]] bool is_running() const;

    // reads & writes from/to the games memory
    [[nodiscard]] std::optional<float> read_fov() const;
    bool override_fov(float fov) const;

private:
    bool read_ptr(pid_t pid);

protected:
    std::string _game_name;
    std::string _process_name;

    std::optional<pid_t> _pid;

    uintptr_t _fov_address;
    uintptr_t _fov_offset;
};

#endif //GAME_H
