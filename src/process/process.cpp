//
// Created by marcel on 20.05.24.
//

#include "process.h"

#include <sys/uio.h>
#include <sys/stat.h>
#include <thread>
#include <cstring>

namespace process
{
    std::optional<pid_t> get_pid(const std::string& process_name)
    {
        FILE *cmd = popen(std::string("pidof ").append(process_name).c_str(), "r");
        char line[20];
        memset(line, '0', 20);
        fgets(line, 19, cmd);
        pid_t pid = strtoul(line, nullptr, 10);
        pclose(cmd);

        if (!pid)
            return std::nullopt;

        return pid;
    }

    bool is_running(pid_t pid)
    {
        struct stat sts;
        return !(stat(("/proc/" + std::to_string(pid)).c_str(), &sts) == -1 && errno == ENOENT);
    }

    bool read(pid_t pid, uintptr_t address, void* buffer, size_t size)
    {
        iovec local{};
        iovec remote{};

        local.iov_base = buffer;
        local.iov_len = size;
        remote.iov_base = (void*)address;
        remote.iov_len = size;

        return process_vm_readv(pid, &local, 1, &remote, 1, 0) == size;
    }

    bool write(pid_t pid, uintptr_t address, void* buffer, size_t size)
    {
        iovec local{};
        iovec remote{};

        local.iov_base = buffer;
        local.iov_len = size;
        remote.iov_base = (void*)address;
        remote.iov_len = size;

        return process_vm_writev(pid, &local, 1, &remote, 1, 0) == size;
    }
}