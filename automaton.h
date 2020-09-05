#ifndef __AUTOMATON_H__
#define __AUTOMATON_H__

#include "rgbcolor.h"
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <ctime>
#include <deque>
#include <utility>

typedef std::deque<std::pair<int, int>> CellQueue;

class Automaton
{
private:
    int window_w, window_h;
    int cell_per_chunk = 5;
    time_t old_time, new_time;
    float center_size = 10.0f;
    float update_time = 1.0f;

    int size_x, size_y;
    int *grid;

    CellQueue user_cell_queue;
    
    const RGBColor burgundy = RGBColor(102, 0, 0);
    const RGBColor white = RGBColor(255, 255, 255, 1.0);
    const RGBColor alpha_white = RGBColor(255, 255, 255, 0.2);
    const RGBColor orange = RGBColor(255, 128, 0);

    void setBackgroundColor(const RGBColor &color) const;
    void setColor(const RGBColor &color) const;
    void setViewport(int left, int right, int bottom, int top) const;
    void setRelativeViewport(GLsizei W, GLsizei H, float R) const;
    void init();
    void reshape(GLsizei W, GLsizei H);
    void display();
    void mouse(int button, int state, int x, int y);
    static void mouseCallback(int button, int state, int x, int y);
    static void displayCallback();
    static void reshapeCallback(GLint width, GLint height);
public:
    Automaton(int argc, char **argv, int window_width, int window_height,
        int size_x, int size_y);
    void setUpdateTime(float seconds) { update_time = seconds; }
    void setCellPerChunk(int count) { cell_per_chunk = count; }
    void run();
};

#endif