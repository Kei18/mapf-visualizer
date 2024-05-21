#include "../include/ofApp.hpp"

#include <fstream>

#include "../include/param.hpp"

static int get_scale(const Graph* G)
{
  auto window_max_w =
      default_screen_width - screen_x_buffer * 2 - window_x_buffer * 2;
  auto window_max_h =
      default_screen_height - window_y_top_buffer - window_y_bottom_buffer;
  return std::min(window_max_w / G->width, window_max_h / G->height) + 1;
}

static void printKeys()
{
  std::cout << "keys for visualizer" << std::endl;
  std::cout << "- p : play or pause" << std::endl;
  std::cout << "- l : loop or not" << std::endl;
  std::cout << "- r : reset" << std::endl;
  std::cout << "- v : show virtual line to goals" << std::endl;
  std::cout << "- f : show agent & node id" << std::endl;
  std::cout << "- g : show goals" << std::endl;
  std::cout << "- right : progress" << std::endl;
  std::cout << "- left  : back" << std::endl;
  std::cout << "- up    : speed up" << std::endl;
  std::cout << "- down  : speed down" << std::endl;
  std::cout << "- i : toggle zoom in" << std::endl;
  std::cout << "- o : toggle zoom out" << std::endl;
  std::cout << "- G : toggle gridlines" << std::endl;
  std::cout << "- space : screenshot (saved in Desktop)" << std::endl;
  std::cout << "- esc : terminate" << std::endl;
}

ofApp::ofApp(Graph* _G, Solution* _P, bool _flg_capture_only)
    : G(_G),
      P(_P),
      N(P->front().size()),
      T(P->size() - 1),
      goals(P->back()),
      scale(get_scale(G)),
      agent_rad(scale / std::sqrt(2) / 2),
      goal_rad(scale / 4.0),
      font_size(std::max(scale / 8, 6)),
      flg_capture_only(_flg_capture_only),
      flg_autoplay(true),
      flg_loop(true),
      flg_goal(true),
      flg_font(false),
      flg_snapshot(flg_capture_only),
      flg_zoomout(false),
      flg_zoomin(false),
      flg_grid(true),
      line_mode(flg_capture_only ? LINE_MODE::PATH : LINE_MODE::STRAIGHT)
{
}

void ofApp::setup()
{
  auto w = G->width * scale + 2 * window_x_buffer;
  auto h = G->height * scale + window_y_top_buffer + window_y_bottom_buffer;
  ofSetWindowShape(w, h);
  ofBackground(Color::bg);
  ofSetCircleResolution(32);
  ofSetFrameRate(30);
  font.load("MuseoModerno-VariableFont_wght.ttf", font_size, true, false, true);

  // setup gui
  gui.setup();
  gui.add(timestep_slider.setup("time step", 0, 0, T));
  gui.add(speed_slider.setup("speed", 0.1, 0, 1));

  cam.setVFlip(true);
  cam.setGlobalPosition(ofVec3f(w / 2, h / 2 - window_y_top_buffer / 2, 580));
  cam.removeAllInteractions();
  cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);

  if (!flg_capture_only) printKeys();
}

void ofApp::update()
{
  if (!flg_autoplay) return;

  // t <- t + speed
  float t = timestep_slider + speed_slider;
  if (t <= T) {
    timestep_slider = t;
  } else {
    timestep_slider = 0;
    if (flg_loop) {
      timestep_slider = 0;
    } else {
      timestep_slider = T;
    }
  }

  if (flg_zoomout) {
    auto campos = cam.getGlobalPosition();
    campos.z = campos.z * 1.01;
    cam.setGlobalPosition(campos);
  }
  if (flg_zoomin) {
    auto campos = cam.getGlobalPosition();
    campos.z = std::max(campos.z * 0.99, 50.0);
    cam.setGlobalPosition(campos);
  }
}

void ofApp::draw()
{
  cam.begin();
  if (flg_snapshot) {
    ofBeginSaveScreenAsPDF(ofFilePath::getUserHomeDir() +
                               "/Desktop/screenshot-" + ofGetTimestampString() +
                               ".pdf",
                           false);
  }

  // draw graph
  ofSetLineWidth(1);
  ofFill();
  for (int x = 0; x < G->width; ++x) {
    for (int y = 0; y < G->height; ++y) {
      auto index = x + y * G->width;
      if (G->U[index] == nullptr) continue;
      ofSetColor(Color::node);
      auto x_draw = x * scale - scale / 2 + window_x_buffer + scale / 2 - 0.15;
      auto y_draw =
          y * scale - scale / 2 + window_y_top_buffer + scale / 2 - 0.15;
      auto gridline_space = flg_grid ? 0.3 : 0.0;
      ofDrawRectangle(x_draw, y_draw, scale - gridline_space, scale - gridline_space);
      if (flg_font) {
        ofSetColor(Color::font);
        font.drawString(std::to_string(index), x_draw + 1,
                        y_draw + font_size + 1);
      }
    }
  }

  // draw goals
  if (flg_goal) {
    for (int i = 0; i < N; ++i) {
      ofSetColor(Color::agents[i % Color::agents.size()]);
      auto g = goals[i];
      int x = g->x * scale + window_x_buffer + scale / 2;
      int y = g->y * scale + window_y_top_buffer + scale / 2;
      ofDrawRectangle(x - goal_rad / 2, y - goal_rad / 2, goal_rad, goal_rad);
    }
  }

  // draw agents
  for (int i = 0; i < N; ++i) {
    ofSetColor(Color::agents[i % Color::agents.size()]);
    auto t1 = (int)timestep_slider;
    auto t2 = t1 + 1;

    // agent position
    auto v = P->at(t1)[i];
    float x = v->x;
    float y = v->y;

    if (t2 <= T) {
      auto u = P->at(t2)[i];
      x += (u->x - x) * (timestep_slider - t1);
      y += (u->y - y) * (timestep_slider - t1);
    }
    x *= scale;
    y *= scale;
    x += window_x_buffer + scale / 2;
    y += window_y_top_buffer + scale / 2;

    ofDrawCircle(x, y, agent_rad);

    // goal
    if (line_mode == LINE_MODE::STRAIGHT) {
      ofDrawLine(goals[i]->x * scale + window_x_buffer + scale / 2,
                 goals[i]->y * scale + window_y_top_buffer + scale / 2, x, y);
    } else if (line_mode == LINE_MODE::PATH) {
      // next loc
      ofSetLineWidth(2);
      if (t2 <= T) {
        auto u = P->at(t2)[i];
        ofDrawLine(x, y, u->x * scale + window_x_buffer + scale / 2,
                   u->y * scale + window_y_top_buffer + scale / 2);
      }
      for (int t = t1 + 1; t < T; ++t) {
        auto v_from = P->at(t)[i];
        auto v_to = P->at(t + 1)[i];
        if (v_from == v_to) continue;
        ofDrawLine(v_from->x * scale + window_x_buffer + scale / 2,
                   v_from->y * scale + window_y_top_buffer + scale / 2,
                   v_to->x * scale + window_x_buffer + scale / 2,
                   v_to->y * scale + window_y_top_buffer + scale / 2);
      }
      ofSetLineWidth(1);
    }

    // agent at goal
    if (v == goals[i]) {
      ofSetColor(255, 255, 255);
      ofDrawCircle(x, y, agent_rad * 0.7);
    }

    // id
    if (flg_font) {
      ofSetColor(Color::font);
      font.drawString(std::to_string(i), x - font_size / 2, y + font_size / 2);
    }
  }

  if (flg_snapshot) {
    ofEndSaveScreenAsPDF();
    flg_snapshot = false;
    if (flg_capture_only) std::exit(0);
  }

  cam.end();
  gui.draw();
}

void ofApp::keyPressed(int key)
{
  if (key == 'r') timestep_slider = 0;  // reset
  if (key == 'p') flg_autoplay = !flg_autoplay;
  if (key == 'l') flg_loop = !flg_loop;
  if (key == 'g') flg_goal = !flg_goal;
  if (key == 'f') {
    flg_font = !flg_font;
    flg_font &= (scale - font_size > 6);
  }
  if (key == 32) flg_snapshot = true;  // space
  if (key == 'v') {
    line_mode =
        static_cast<LINE_MODE>(((int)line_mode + 1) % (int)LINE_MODE::NUM);
  }
  float t;
  if (key == OF_KEY_RIGHT) {
    t = timestep_slider + speed_slider;
    timestep_slider = std::min((float)T, t);
  }
  if (key == OF_KEY_LEFT) {
    t = timestep_slider - speed_slider;
    timestep_slider = std::max((float)0, t);
  }
  if (key == OF_KEY_UP) {
    t = speed_slider + 0.001;
    speed_slider = std::min(t, (float)speed_slider.getMax());
  }
  if (key == OF_KEY_DOWN) {
    t = speed_slider - 0.001;
    speed_slider = std::max(t, (float)speed_slider.getMin());
  }
  if (key == 'i') {
    flg_zoomin = !flg_zoomin;
  }
  if (key == 'o') {
    flg_zoomout = !flg_zoomout;
  }
  if (key == 'G') {
    flg_grid = !flg_grid;
  }
}

void ofApp::keyReleased(int key) {}

void ofApp::mouseMoved(int x, int y) {}

void ofApp::mouseDragged(int x, int y, int button) {}

void ofApp::mousePressed(int x, int y, int button) {}

void ofApp::mouseReleased(int x, int y, int button) {}

void ofApp::mouseEntered(int x, int y) {}

void ofApp::mouseExited(int x, int y) {}

void ofApp::windowResized(int w, int h) {}

void ofApp::gotMessage(ofMessage msg) {}

void ofApp::dragEvent(ofDragInfo dragInfo) {}
