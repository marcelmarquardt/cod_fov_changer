//
// Created by marcel on 20.05.24.
//

#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <iostream>
#include <cinttypes>
#include <optional>
#include <sys/types.h>

namespace process
{
    std::optional<pid_t> get_pid(const std::string& process_name);
    bool is_running(pid_t pid);
    bool read(pid_t pid, uintptr_t address, void* buffer, size_t size);
    bool write(pid_t pid, uintptr_t address, void* buffer, size_t size);
}

#endif //PROCESS_H
