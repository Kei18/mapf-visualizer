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
using Solution = std::vector<Config>;

struct Graph {
  Vertices V;  // without nullptr
  Vertices U;  // with nullptr
  int width;   // grid width
  int height;  // grid height
  Graph();
  Graph(char* filename);  // taking map filename
  ~Graph();
};

class Orientation
{
public:
  enum Value {
    NONE = 0,
    X_MINUS = 1,
    X_PLUS = 2,
    Y_MINUS = 3,
    Y_PLUS = 4,
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
    if (s == "X_MINUS") return Orientation::X_MINUS;
    if (s == "X_PLUS") return Orientation::X_PLUS;
    if (s == "Y_MINUS") return Orientation::Y_MINUS;
    if (s == "Y_PLUS") return Orientation::Y_PLUS;
    return Orientation::NONE;
  }

  std::string to_str() const
  {
    switch (value) {
      case X_MINUS: return "X_MINUS";
      case X_PLUS: return "X_PLUS";
      case Y_MINUS: return "Y_MINUS";
      case Y_PLUS: return "Y_PLUS";
      default: return "NONE";
    }
  }

  float to_angle() const
  {
    switch (value) {
      case X_MINUS: return 180;
      case X_PLUS: return 0;
      case Y_MINUS: return 270;
      case Y_PLUS: return 90;
      default: return 0;
    }
  }

  constexpr operator Value() const { return value; }
  explicit operator bool() const = delete;

private:
  Value value;
};
