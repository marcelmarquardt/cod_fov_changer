//
// Created by marcel on 19.05.24.
//

#include "Game.h"

#include <thread>
#include <utility>
#include "../process/process.h"

Game::Game(std::string  game_name,
           std::string  process_name,
           uintptr_t fov_address,
           uintptr_t fov_offset) : _game_name(std::move(game_name)), _process_name(std::move(process_name)),
                                   _fov_address(fov_address), _fov_offset(fov_offset), _pid(std::nullopt)
{}

const std::string& Game::get_game_name() const
{
    return _game_name;
}

bool Game::initialize()
{
    if (_pid.has_value())
        return true;

    std::optional<pid_t> pid = process::get_pid(_process_name);
    if (!pid.has_value())
        return false;

    // give the game some times to initialize, increase this number if your PC is not up to 2012 speed
    std::this_thread::sleep_for(std::chrono::seconds(10));

    if (!read_ptr(pid.value()))
        return false;

    _pid = pid;
    return true;
}

bool Game::read_ptr(pid_t pid)
{
    uint32_t ptr;
    if (!process::read(pid, _fov_address, &ptr, sizeof(ptr)))
        return false;

    if (!ptr)
        return false;

    _fov_address = ptr + _fov_offset;
    return true;
}

void Game::reset()
{
    _pid = std::nullopt;
}

bool Game::is_running() const
{
    if (!_pid.has_value())
        return false;

    return process::is_running(_pid.value());
}

std::optional<float> Game::read_fov() const
{
    if (!_pid.has_value())
        return std::nullopt;

    float fov;
    if (!process::read(_pid.value(), _fov_address, (void*)&fov, sizeof(fov)))
        return std::nullopt;

    return fov;
}

bool Game::override_fov(float fov) const
{
    if (!_pid.has_value())
        return false;

    return process::write(_pid.value(), _fov_address, (void*)&fov, sizeof(fov));
}
