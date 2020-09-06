#ifndef __AUTOMATON_H__
#define __AUTOMATON_H__

#include "rgbcolor.h"
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <sys/time.h>
#include <deque>
#include <utility>
#include <string>
#include <vector>

class Automaton
{
private:
    int window_w, window_h;
    int frame_w, frame_h;
    int cell_per_chunk = 5;
    double old_time, new_time;
    float center_size = 10.0f;
    float update_time = 0.5f;
    int step = 0;

    bool isRunning = false;

    std::string S, B;
    int C;

    int size_x, size_y;
    int cell_count;
    int *grid_old, *grid_new;

    const RGBColor burgundy = RGBColor(70, 0, 0);
    const RGBColor dark_burgundy = RGBColor(50, 0, 0);
    const RGBColor white = RGBColor(255, 255, 255, 1.0);
    const RGBColor alpha_white = RGBColor(255, 255, 255, 0.2);
    const RGBColor orange = RGBColor(255, 128, 0);
    const RGBColor yellow = RGBColor(255, 255, 0);
    const RGBColor red = RGBColor(200, 0, 0);
    const RGBColor alpha_red = RGBColor(255, 0, 0, 0.6);
    std::vector<RGBColor> gradient;

    void setBackgroundColor(const RGBColor &color) const;
    void setColor(const RGBColor &color) const;
    void setViewport(int left, int right, int bottom, int top) const;
    void setRelativeViewport(GLsizei W, GLsizei H, float R);
    void init();
    void reshape(GLsizei W, GLsizei H);
    void display();
    void mouse(int button, int state, int x, int y);
    static void mouseCallback(int button, int state, int x, int y);
    static void displayCallback();
    static void reshapeCallback(GLint width, GLint height);
    void loadGradient(const RGBColor &low, const RGBColor &high);

    bool isAlive(int neighbour_count) const;
    bool isBorn(int neighbour_count) const;
    void drawGrid() const;
    void drawCell(int i, int j, const RGBColor &color) const;
    void drawMesh(const RGBColor &cell, const RGBColor &chunk) const;
    int updateGrid();
    int countNeighbour(int i, int j);
    double rtclock();
public:
    Automaton(int argc, char **argv, int window_width, int window_height,
        int size_x, int size_y);
    void setUpdateTime(float seconds) { update_time = seconds; }
    void setCellPerChunk(int count) { cell_per_chunk = count; }
    void setMode(const std::string &str);
    void run();
    ~Automaton() { delete grid_old; delete grid_new; }

    const std::string spark = "2/2/25";
    const std::string bugs = "23/2/8";
};

#endif