#include <fstream>
#include <iostream>
#include <regex>

#include "../include/ofApp.hpp"
#include "ofMain.h"

const std::regex r_pos = std::regex(R"(\((\d+),(\d+)\),)");

int main(int argc, char *argv[])
{
  // simple arguments check
  if (argc < 3 || !std::ifstream(argv[1]) || !std::ifstream(argv[2])) {
    std::cout << "Please check the arguments, e.g.,\n"
              << "> mapf-visualizer assets/random-32-32-20.map "
                 "assets/demo_random-32-32-20.txt"
              << std::endl;
    return 0;
  }

  // load graph
  Graph G(argv[1]);

  // load plan
  auto solution_file = std::ifstream(argv[2]);
  Solution solution;
  std::string line;
  std::smatch m, results;
  while (getline(solution_file, line)) {
    // so we only process real solution lines
    if(line.find(":(") == std::string::npos) continue;

    auto iter = line.cbegin();
    Config c;
    while (iter < line.cend()) {
      auto search_end = std::min(iter + 128, line.cend());
      if (std::regex_search(iter, search_end, m, r_pos)) {
        auto x = std::stoi(m[1].str());
        auto y = std::stoi(m[2].str());
        c.push_back(G.U[G.width * y + x]);
        iter += m[0].length();
      } else {
        break;
      }
    }
    solution.push_back(c);
  }
  solution_file.close();

  // visualize
  ofSetupOpenGL(100, 100, OF_WINDOW);
  ofRunApp(new ofApp(&G, &solution,
                     (argc > 3 && std::string(argv[3]) == "--capture-only")));
  return 0;
}
