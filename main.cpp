#include <cstdlib>
#include <fstream>
#include <iostream>
#include <thread>
#include <boost/lexical_cast.hpp>
#include <string>
#include <vector>

#include <nRF24L01.h>
#include <RF24.h>

struct rx_settings {
  int channel;
  int rate;
  int ce0;
  int ce1;
  int spidev;
  uint64_t addr;
} rx_data;

static unsigned long int data0_idx = 0;
static unsigned long int data1_idx = 0;

std::vector<char[32]> data0;
std::vector<char[32]> data1;

int main() {
  std::ifstream input("input.txt");
  if (input.is_open()) {
    std::string line;
    std::getline(input, line);
    rx_data.channel = std::stoi(line);
    std::getline(input, line);
    rx_data.rate = std::stoi(line);
    std::getline(input, line);
    rx_data.ce0 = std::stoi(line);
    std::getline(input, line);
    rx_data.ce1 = std::stoi(line);
    std::getline(input, line);
    rx_data.spidev = std::stoi(line);
    std::getline(input, line);
    rx_data.addr = std::stoi(line);
    input.close();
    std::cout << "Reading input file successfull..." << std::endl;
  } else {
    std::cout << "Cannot read input file!" << std::endl;
    return 1;
  }

  RF24 radio0(rx_data.ce0, rx_data.spidev);
  RF24 radio0(rx_data.ce1, rx_data.spidev);
  radio0.begin();
  radio1.begin();
  radio0.setDataRate((rx_data.rate == 1) ? RF24_1MBPS : RF24_2MBPS);
  radio1.setDataRate((rx_data.rate == 1) ? RF24_1MBPS : RF24_2MBPS);
  radio0.openReadingPipe(0, rx_data.addr);
  radio1.openReadingPipe(0, rx_data.addr);
  radio0.startListening();
  radio1.startListening();
  
  std::cout << "Radio initialisated..." << std::endl;
  std::ofstream output0("output0.txt");
  std::ofstream output1("output1.txt");
  if (out0.is_open() && out1.is_open()) std::cout << "Output files opened..." << std::endl;
  std::thread radio0_thread([&]() {
    char buff[32];
    while (true) {
      if (radio1.available()) {
        radio1.read(buff, 32);
        data1_idx++;
        data1.push_end(buff);
      }
    }
  });
  std::thread radio1_thread([&]() {
    char buff[32];
    while (true) {
      if (radio0.available()) {
        radio0.read(buff, 32);
        data0_idx++;
        data0.push_end(buff);
      }
    }
  });
  std::thread writer_thread([&]() {
    while (true) {
      if (data0_idx >= 0) output0 << data0.at(data0_idx) << std::endl;
      if (data1_idx >= 0) output1 << data1.at(data1_idx) << std::endl;
    }
  });
  radio0_thread.join();
  radio1_thread.join();
  writer_thread.join();
  return 0;
}