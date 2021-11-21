#include <fstream>
#include <stdio.h>
#include <vector>

#include "chip8em.h"

void print_usage(FILE *stream, const char *pname) {
  fprintf(stream, "Usage: %s romfile\n", pname);
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    print_usage(stderr, argv[0]);
    exit(-1);
  }

  std::ifstream infile(argv[1], std::ios::binary | std::ios::ate);
  if (!infile.is_open()) {
    print_usage(stderr, argv[0]);
    fprintf(stderr, "Error: could not open file %s\n", argv[1]);
  } else {
    auto filesize = infile.tellg();
    if (filesize > 4096 - 0x200) {
      print_usage(stderr, argv[0]);
      fprintf(stderr,
              "Error: rom file %s is too large. Filesize must not exceed %d "
              "bytes.\n",
              argv[1], 4096 - 0x200);
    } else {
      std::vector<u8> program(filesize);
      infile.seekg(0);
      if (infile.read(reinterpret_cast<char *>(&program[0]), filesize)) {
        Chip8 myc8;
        myc8.initialize();
        myc8.load_program(&program[0], int(filesize));
        myc8.run();
      }
    }
  }

  return 0;
}