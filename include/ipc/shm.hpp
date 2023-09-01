#pragma once

#include <fcntl.h>  // For O_* constants
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>  // For mode constants
#include <unistd.h>

#include <cstring>
#include <iostream>

namespace mylib::IPC {

template <class T>
class SharedMemoryService {
  int fd;

  struct Shared {
    pthread_mutex_t mutex;
    T data;
  };

 public:
  explicit SharedMemoryService(const char* memname)
      : fd(shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666)) {
    if (fd == -1) {
      std::cerr << "shm_open failed" << std::endl;
      exit(1);
    }

    if (ftruncate(fd, sizeof(Shared)) == -1) {
      std::cerr << "ftruncate failed" << std::endl;
      exit(1);
    }

    auto* shared = static_cast<Shared*>(mmap(
        nullptr, sizeof(Shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (shared == MAP_FAILED) {
      std::cerr << "mmap failed" << std::endl;
      exit(1);
    }
  }
};

};  // namespace mylib::IPC
