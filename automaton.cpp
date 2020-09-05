#include "automaton.h"
#include <deque>

static Automaton *instance;

Automaton::Automaton(int argc, char **argv, int window_width, int window_height) :
    window_w(window_width), window_h(window_height), chunk_half_count(5),
    cell_per_chunk(5) {
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
    int grid_size = cell_per_chunk * 2 * chunk_half_count;
    grid = new int[grid_size * grid_size];
    old_time = time(NULL);
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

void Automaton::reloadViewport(GLint width, GLint height) const {
    /*
    if (width >= height) {
        glViewport(0, -(width - height) / 2, width, width);
    } else {
        glViewport(-(height - width) / 2, 0, height, height);
    }
    *//*
    if (width >= height) {
        glViewport( , height);
    } else {
        glViewport();
    }*/
}

void Automaton::init() {
    setBackgroundColor(burgundy);
    setColor(white);
    //reloadViewport(window_w, window_h);
    
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
    glLoadIdentity();
    setViewport(0, window_w, 0, window_h);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Automaton::reshape(GLsizei W, GLsizei H) {
    /*reloadViewport(width, height);
    window_w = width;
    window_h = height;*/
    float R = 1.0f;
    if (R > W / (float) H) {
        float height = W / R;
        setViewport(0, W, H / 2 - height / 2, H / 2 + height / 2);
    } else {
        float width = H * R;
        setViewport(W / 2 - width / 2, W / 2 + width / 2, 0, H);
    }
    //glutPostRedisplay();
}

void Automaton::setViewport(int left, int right, int bottom, int top) {
    glViewport(left, bottom, right - left, top - bottom);
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
    float chunk_size = 1.0f / float(chunk_half_count);
    for (int i = -chunk_half_count; i <= chunk_half_count; i++) {
        float chunk_i = chunk_size * i;
        glLineWidth(1);
        setColor(alpha_white);
        glBegin(GL_LINES);
            float cell_size = chunk_size / float(cell_per_chunk);
            for (int j = 0; j <= cell_per_chunk; j++) {
                // vertical
                float cell_ij = chunk_i + j * cell_size;
                glVertex2f(cell_ij, -1.0);
                glVertex2f(cell_ij, 1.0);

                //horizontal
                glVertex2f(-1.0, cell_ij);
                glVertex2f(1.0, cell_ij);
            }
        glEnd();

        if (i == 0) {
            glLineWidth(2);
            setColor(orange);
        } else {
            glLineWidth(1);
            setColor(white);
        }
        glBegin(GL_LINES);
            // vertical
            
            glVertex2f(chunk_i, -1.0);
            glVertex2f(chunk_i, 1.0);
            // horizontal
            glVertex2f(-1.0, chunk_i);
            glVertex2f(1.0, chunk_i);
        glEnd();
    }
    
    glFlush();
    glutSwapBuffers();
}

void Automaton::run() {
    glutMainLoop();
}