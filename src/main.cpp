#include <fstream>
#include <iostream>
#include <regex>

#include "../include/ofApp.hpp"
#include "../third_party/argparse/include/argparse/argparse.hpp"
#include "ofMain.h"

const std::regex r_config = std::regex(R"(^\d+:(.+))");
const std::regex r_pos = std::regex(R"(\((\d+),(\d+)\),)");

int main(int argc, char* argv[])
{
  argparse::ArgumentParser program("mapf-visualizer", "0.1.0");
  program.add_argument("-m", "--map").help("map file").required();
  program.add_argument("-p", "--plan").help("plan file").required();
  program.add_argument("--capture-only")
      .help("take a screen shot and save it in ~/Desktop/")
      .default_value(false)
      .implicit_value(true);

  try {
    program.parse_known_args(argc, argv);
  } catch (const std::runtime_error& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }

  // load graph
  auto map_name = program.get<std::string>("map");
  Graph G(map_name);

  // // load plan
  // const auto plan_name = ;
  auto solution_file = std::ifstream(program.get<std::string>("plan"));
  Solution solution;
  std::string line;
  std::smatch m, results;
  while (getline(solution_file, line)) {
    if (std::regex_match(line, results, r_config)) {
      auto s = results[1].str();
      Config c;
      auto iter = s.cbegin();
      while (std::regex_search(iter, s.cend(), m, r_pos)) {
        iter = m[0].second;
        auto x = std::stoi(m[1].str());
        auto y = std::stoi(m[2].str());
        c.push_back(G.U[G.width * y + x]);
      }
      solution.push_back(c);
    }
  }
  solution_file.close();

  // visualize
  ofSetupOpenGL(100, 100, OF_WINDOW);
  ofRunApp(new ofApp(&G, &solution, program.get<bool>("capture-only")));
  return 0;
}
