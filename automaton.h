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
    int window_w;
    int window_h;
    int chunk_half_count;
    int cell_per_chunk;
    time_t old_time, new_time;
    float center_size = 10.0f;
    float update_time = 1.0f;

    int grid_size;
    int *grid;

    CellQueue user_cell_queue;
    
    const RGBColor burgundy = RGBColor(102, 0, 0);
    const RGBColor white = RGBColor(255, 255, 255, 0.5);
    const RGBColor alpha_white = RGBColor(255, 255, 255, 0.2);
    const RGBColor orange = RGBColor(255, 128, 0);

    void setBackgroundColor(const RGBColor &color) const;
    void setColor(const RGBColor &color) const;
    void reloadViewport(GLint width, GLint height) const;
    void init();
    void reshape(GLsizei W, GLsizei H);
    void display();
    void mouse(int button, int state, int x, int y);
    static void mouseCallback(int button, int state, int x, int y);
    static void displayCallback();
    static void reshapeCallback(GLint width, GLint height);
    void setViewport(int left, int right, int bottom, int top);
public:
    Automaton(int argc, char **argv, int window_width, int window_height);
    void setUpdateTime(float seconds) { update_time = seconds; }
    void setCellPerChunk(int count) { cell_per_chunk = count; }
    void setChunkHalfCount(int half_count) { chunk_half_count = half_count; }
    void run();
};

#endif