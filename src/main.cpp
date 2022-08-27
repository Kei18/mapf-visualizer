#include <fstream>
#include <iostream>

#include "../include/ofApp.hpp"
#include "ofMain.h"

int main(int argc, char *argv[])
{
  // simple arguments check
  if (argc != 3 || !(std::ifstream(argv[1])) || !(std::ifstream(argv[2]))) {
    std::cout << "Please check the arguments, e.g.,\n"
              << "> execfile assets/random-32-32-20.map "
                 "assets/demo_random-32-32-20.txt"
              << std::endl;
    return 0;
  }

  ofSetupOpenGL(100, 100, OF_WINDOW);
  ofRunApp(new ofApp(&argv[1], &argv[2]));
  return 0;
}
