#include <iostream>
#include <fstream>
#include <vector>

#include "chip8em.h"

int main(int argc, char *argv[]) {
  Chip8 myc8;
  myc8.initialize();

  if(argc != 2) {
    exit(-1);
  }
  std::ifstream infile(argv[1], std::ios::binary | std::ios::ate);
  if(infile.is_open()) {
    auto filesize = infile.tellg();
    if(filesize <= 4096 - 0x200) {
      std::vector<u8> program(filesize);
      infile.seekg(0);
      if(infile.read(reinterpret_cast<char*>(&program[0]), filesize)) {
        myc8.load_program(&program[0], int(filesize));
        myc8.run();
      }
    }

  }



  return 0;
}