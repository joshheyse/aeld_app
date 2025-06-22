#include "../include/CanReader.h"
#include <cstring>

namespace bbc {

CanReader::CanReader(std::string interface) : interface_(interface) {
  sockaddr_can addr;
  struct ifreq ifr;

  if ((socket_ = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
    throw std::runtime_error("Error creating CAN socket");
    exit(EXIT_FAILURE);
    // Constructor implementation
  }

  strcpy(ifr.ifr_name, interface_.c_str());
  if (ioctl(socket_, SIOCGIFINDEX, &ifr) < 0) {
    close(socket_);
    throw std::runtime_error("Error getting interface index");
    exit(EXIT_FAILURE);
  }

  memset(&addr, 0, sizeof(addr));
  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;

  if (bind(socket_, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    close(socket_);
    throw std::runtime_error("Error binding CAN socket");
    exit(EXIT_FAILURE);
  }

  // Start the consumer thread to read CAN messages
  consumer_ = std::thread([this]() { consume(); });
}

void CanReader::consume() {
  struct can_frame frame;
  while (true) {
    int nbytes = read(socket_, &frame, sizeof(struct can_frame));
    if (nbytes < 0) {
      if (socket_ < 0) {
        break;
      }
      perror("Error reading CAN frame");
      break;
    } else if (nbytes < sizeof(struct can_frame)) {
      fprintf(stderr, "Read incomplete CAN frame\n");
      continue;
    }
    // Process the received CAN frame
    // printf("Received CAN ID: %x, Data: ", frame.can_id);
    // for (int i = 0; i < frame.can_dlc; i++) {
    //   printf("%02x ", frame.data[i]);
    // }
    // printf("\n");

    switch (frame.can_id) {
    case 0x153: {
      // Speed fame.data[1] LSB, fame.data[2] MSB
      double speed = (frame.data[1] | (frame.data[2] << 8));
      printf("Speed: %.2f km/h\n", speed);
      break;
    }
    case 0x316: {
      // RPM fame.data[2] LSB, fame.data[3] MSB / 6.4
      double rpm = (frame.data[1] | (frame.data[2] << 8)) / 6.4;
      printf("RPM: %.2f km/h\n", rpm);
      break;
    }
    case 0x329: {
      // TEMP fame.data[1] * .75 - 48.373
      double temp = frame.data[1] * 0.75 - 48.373;
      printf("temp: %.2f km/h\n", temp);

      // Throttle position: fame.data[5] (00-FE)
      double throttle = (frame.data[5] * 100.0) / 254.0; // 0-100%
      printf("throttle: %.2f %%\n", throttle);

      // Brake pedal: fame.data[6] (00-01)
      bool breakPedal = (frame.data[6] & 0x01) != 0; // Bit 0
      printf("brake pedal: %s\n", breakPedal ? "pressed" : "not pressed");
      break;
    }

    case 0x545: {
      // Check engine fame.data[0] (0b10)
      bool cel = (frame.data[0] & 0x02) != 0; // Bit 1
      printf("check engine: %s\n", cel ? "on" : "off");

      // Engine management light fame.data[0] (0b10000)
      bool eml = (frame.data[0] & 0x10) != 0; // Bit 4
      printf("engine management: %s\n", cel ? "on" : "off");

      break;
    }
    }
  }
}

CanReader::~CanReader() {
  if (socket_ >= 0) {
    close(socket_);
    socket_ = -1;
  }

  if (consumer_.joinable()) {
    consumer_.join(); // Wait for the consumer thread to finish
  }
}

} // namespace bbc
