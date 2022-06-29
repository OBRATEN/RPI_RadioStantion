#include <cstdlib>
#include <fstream>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

#include "RF24/nRF24L01.h"
#include "RF24/RF24.h"

int main() {
  RF24 radio0(26, 29, 0);
  RF24 radio1(36, 31, 1);
  radio0.begin();
  radio1.begin();
  radio0.setDataRate(RF24_1MBPS);
  radio1.setDataRate(RF24_1MBPS);
  radio0.openReadingPipe(0, 0x7878787878LL);
  radio1.openReadingPipe(0, 0x7878787878LL);
  radio0.startListening();
  radio1.startListening();
  std::cout << "Radio initialisated..." << std::endl;
  std::ofstream output0("output0.txt");
  std::ofstream output1("output1.txt");
  if (output0.is_open() && output1.is_open()) std::cout << "Output files opened..." << std::endl;
  std::thread radio0_thread([&]() {
    char buff[32];
    while (true) {
      if (radio1.available()) {
        radio1.read(buff, 32);
        output1 << buff << std::endl;
      }
    }
  });
  std::thread radio1_thread([&]() {
    char buff[32];
    while (true) {
      if (radio0.available()) {
        radio0.read(buff, 32);
        output0 << buff << std::endl;
      }
    }
  });
  radio0_thread.join();
  radio1_thread.join();
  return 0;
}