#pragma once

#include "graph.hpp"
#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp
{
public:
  const Graph* G;
  const Solution* P;  // plan
  const int N;        // number of agents
  const int T;        // makespan
  const Config goals;

  // size
  const int scale;
  const float agent_rad;
  const float goal_rad;
  const int font_size;

  // flg
  bool flg_capture_only;
  bool flg_autoplay;
  bool flg_loop;
  bool flg_goal;
  bool flg_font;
  bool flg_snapshot;
  bool flg_zoomout;
  bool flg_zoomin;
  bool flg_grid;

  enum struct LINE_MODE { STRAIGHT, PATH, NONE, NUM };
  LINE_MODE line_mode;

  // font
  ofTrueTypeFont font;

  // gui
  ofxFloatSlider timestep_slider;
  ofxFloatSlider speed_slider;
  ofxPanel gui;

  // camera
  ofEasyCam cam;

  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  ofApp(Graph* _G, Solution* _P, bool flg_capture_only = false);
};
