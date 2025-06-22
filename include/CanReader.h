
#pragma once
#include <functional>
#include <string>
#include <thread>

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

namespace bbc {

template <typename T> class CanValue {

  canid_t id_;
  std::function<T(const struct can_frame &)> decoder_;

  CanValue(canid_t id, std::function<T(const struct can_frame &)> decoer)
      : id_(id), decoder_(decoer) {}
};

class CanReader {
private:
  // Private members for CAN reader
  int socket_;
  std::string interface_;
  std::thread consumer_;

  void consume();

public:
  CanReader(std::string interface);
  ~CanReader();
};
} // namespace bbc
