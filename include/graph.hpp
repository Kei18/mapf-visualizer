#pragma once
#include <vector>
#include <set>
#include <string>

struct Vertex {
  const int id;     // index for V in Graph
  const int index;  // index for U, width * y + x, in Graph
  const int x;
  const int y;
  std::vector<Vertex*> neighbor;

  Vertex(int _id, int _index, int _x, int _y);
};
using Vertices = std::vector<Vertex*>;
using Config = std::vector<Vertex*>;  // a set of locations for all agents
using VertexSet = std::set<Vertex*>;  // like Vertices but with constant lookup
using Solution = std::vector<Config>;

struct Graph {
  Vertices V;  // without nullptr
  Vertices U;  // with nullptr
  int width;   // grid width
  int height;  // grid height
  VertexSet sources; // MAPD source
  VertexSet sinks;   // MAPD sink
  VertexSet homes;   // Robot homes
  Graph();
  Graph(char* filename);  // taking map filename
  ~Graph();
};

class Orientation
{
public:
  enum Value {
    NONE = 0,
    UP = 1,
    LEFT = 2,
    DOWN = 3,
    RIGHT = 4,
    NUM_ORIENTATIONS = 5
  };

  constexpr Orientation(Value v) : value(v) {}
  constexpr bool operator==(const Orientation::Value& v) const
  {
    return value == v;
  }
  constexpr bool operator==(const Orientation& o) const
  {
    return value == o.value;
  }
  constexpr bool operator!=(const Orientation::Value& v) const
  {
    return value != v;
  }
  constexpr bool operator!=(const Orientation& o) const
  {
    return value != o.value;
  }

  static Orientation from_string(const std::string& s)
  {
    if (s == "UP") return Orientation::UP;
    if (s == "LEFT") return Orientation::LEFT;
    if (s == "DOWN") return Orientation::DOWN;
    if (s == "RIGHT") return Orientation::RIGHT;
    return Orientation::NONE;
  }

  std::string to_str() const
  {
    switch (value) {
      case UP: return "UP";
      case LEFT: return "LEFT";
      case DOWN: return "DOWN";
      case RIGHT: return "RIGHT";
      default: return "NONE";
    }
  }

  float to_angle() const
  {
    switch (value) {
      case UP: return 90;
      case LEFT: return 180;
      case DOWN: return 270;
      case RIGHT: return 0;
      default: return 0;
    }
  }

  constexpr operator Value() const { return value; }
  explicit operator bool() const = delete;

private:
  Value value;
};
