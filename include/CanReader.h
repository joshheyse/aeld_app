
#pragma once
#include <functional>
#include <optional>
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

struct Values {
  std::optional<double> speed{};
  std::optional<double> rpm{};
  std::optional<double> temp{};
  std::optional<double> throttle{};
  std::optional<bool> brake{};
  std::optional<bool> cel{};
  std::optional<bool> eml{};
};

class CanReader {
private:
  // Private members for CAN reader
  int socket_;
  std::string interface_;
  std::thread consumer_;
  Values values_;

  void consume();

public:
  CanReader(std::string interface);
  ~CanReader();

  Values getValues() const;
};
} // namespace bbc
