#include "automaton.h"
#include <cstdio>
#include <deque>

static Automaton *instance;

Automaton::Automaton(int argc, char **argv, int window_width, int window_height,
        int size_x, int size_y) : window_w(window_width), window_h(window_height),
        size_x(size_x), size_y(size_y) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(window_w, window_h);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Cellular automaton");
    instance = this;
    glutDisplayFunc(Automaton::displayCallback);
    glutReshapeFunc(Automaton::reshapeCallback);
    glutIdleFunc(Automaton::displayCallback);
    glutMouseFunc(Automaton::mouseCallback);
    //glutKeyboardFunc();
    grid = new int[size_x * size_y];
    old_time = time(NULL);
    printf("%d %d\n", size_x, size_y);
    init();
}

void Automaton::mouseCallback(int button, int state, int x, int y) {
    instance->mouse(button, state, x, y);
}

void Automaton::displayCallback() {
    instance->display();
}

void Automaton::reshapeCallback(GLint width, GLint height) {
    instance->reshape(width, height);
}

void Automaton::setBackgroundColor(const RGBColor &color) const {
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.alpha);
}

void Automaton::setColor(const RGBColor &color) const {
    glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.alpha);
}

void Automaton::setViewport(int left, int right, int bottom, int top) const {
    glViewport(left, bottom, right - left, top - bottom);
}

void Automaton::setRelativeViewport(GLsizei W, GLsizei H, float R = 0) const {
    if (R == 0) {
        R = window_w / float(window_h);
    }
    if (R > W / (float) H) {
        float height = W / R;
        setViewport(0, W, H / 2 - height / 2, H / 2 + height / 2);
    } else {
        float width = H * R;
        setViewport(W / 2 - width / 2, W / 2 + width / 2, 0, H);
    }
}

void Automaton::init() {
    setBackgroundColor(burgundy);
    setColor(white);
    
    glLoadIdentity();
    gluOrtho2D(0, size_x, size_y, 0);
    
    glMatrixMode(GL_MODELVIEW);
    setRelativeViewport(window_w, window_h);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Automaton::reshape(GLsizei W, GLsizei H) {
    setRelativeViewport(W, H, size_x / float(size_y));
}

void Automaton::mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        user_cell_queue.push_front(std::make_pair(x, y));
        float grid_x = x / float(window_w);
        float grid_y = y / float(window_h);
        if (window_w >= window_h) {

        } else {

        }
    }
}

void Automaton::display() {
    glPointSize(center_size);
    
    glClear(GL_COLOR_BUFFER_BIT);
    setColor(orange);
    new_time = time(NULL);
    if (difftime(new_time, old_time) >= update_time) {
        old_time = new_time;
        //center_size += 5;
    }
    glBegin(GL_POINTS);
        glVertex2f(0, 0);
    glEnd();
    for (int i = 0; i <= size_x; i++) {
        if (i % cell_per_chunk == 0) {
            glLineWidth(2);
            setColor(orange);
        } else {
            glLineWidth(1);
            setColor(alpha_white);
        }
        
        glBegin(GL_LINES);
            glVertex2f(i, -size_y);
            glVertex2f(i, size_y);
        glEnd();
    }
    for (int i = 0; i <= size_y; i++) {
        if (i % cell_per_chunk == 0) {
            glLineWidth(2);
            setColor(orange);
        } else {
            glLineWidth(1);
            setColor(alpha_white);
        }
        
        glBegin(GL_LINES);
            glVertex2f(-size_x, i);
            glVertex2f(size_x, i);
        glEnd();
    }
    setColor(white);
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0, 0);
        glVertex2f(size_x, 0);
        glVertex2f(size_x, size_y);
        glVertex2f(0, size_y);
    glEnd();
    glFlush();
    glutSwapBuffers();
}

void Automaton::run() {
    glutMainLoop();
}