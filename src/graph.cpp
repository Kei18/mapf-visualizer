#include "../include/graph.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <cmath>

Vertex::Vertex(int _id, int _index, int _x, int _y)
    : id(_id), index(_index), x(_x), y(_y), neighbor(Vertices())
{
}

Graph::Graph() : V(Vertices()), width(0), height(0) {}
Graph::~Graph()
{
  for (auto& v : V)
    if (v != nullptr) delete v;
}

// to load graph
static const std::regex r_height = std::regex(R"(height\s(\d+))");
static const std::regex r_width = std::regex(R"(width\s(\d+))");
static const std::regex r_map = std::regex(R"(map)");

Graph::Graph(char* filename) : V(Vertices()), width(0), height(0)
{
  std::ifstream file(filename);
  if (!file) {
    std::cout << "failed to load " << filename << std::endl;
    return;
  }
  std::string line;
  std::smatch results;

  // read fundamental graph parameters
  while (getline(file, line)) {
    // for CRLF coding
    if (*(line.end() - 1) == 0x0d) line.pop_back();

    if (std::regex_match(line, results, r_height)) {
      height = std::stoi(results[1].str());
    }
    if (std::regex_match(line, results, r_width)) {
      width = std::stoi(results[1].str());
    }
    if (std::regex_match(line, results, r_map)) break;
  }

  U = Vertices(width * height, nullptr);

  // create vertices
  int y = 0;
  while (getline(file, line)) {
    // for CRLF coding
    if (*(line.end() - 1) == 0x0d) line.pop_back();
    for (int x = 0; x < width; ++x) {
      char s = line[x];
      if (s == 'T' or s == '@') continue;  // object
      auto index = width * y + x;
      auto v = new Vertex(V.size(), index, x, y);
      V.push_back(v);
      U[index] = v;
    }
    ++y;
  }
  file.close();

  // create edges
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      auto v = U[width * y + x];
      if (v == nullptr) continue;
      // left
      if (x > 0) {
        auto u = U[width * y + (x - 1)];
        if (u != nullptr) v->neighbor.push_back(u);
      }
      // right
      if (x < width - 1) {
        auto u = U[width * y + (x + 1)];
        if (u != nullptr) v->neighbor.push_back(u);
      }
      // up
      if (y < height - 1) {
        auto u = U[width * (y + 1) + x];
        if (u != nullptr) v->neighbor.push_back(u);
      }
      // down
      if (y > 0) {
        auto u = U[width * (y - 1) + x];
        if (u != nullptr) v->neighbor.push_back(u);
      }
    }
  }
}

std::string Orientation::to_str() const
{
  switch (value) {
    case X_MINUS: return "X_MINUS";
    case X_PLUS: return "X_PLUS";
    case Y_MINUS: return "Y_MINUS";
    case Y_PLUS: return "Y_PLUS";
    default: return "NONE";
  }
}

float Orientation::to_angle() const
{
  switch (value) {
    case X_MINUS: return 180;
    case X_PLUS: return 0;
    case Y_MINUS: return 270;
    case Y_PLUS: return 90;
    default: return NAN;
  }
}
