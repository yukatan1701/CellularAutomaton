#include "automaton.h"
#include <cstdio>
#include <deque>
#include <cctype>

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
    grid_old = new int[size_x * size_y];
    grid_new = new int[size_x * size_y];
    for (int i = 0; i < size_x * size_y; i++) {
        grid_old[i] = 0;
        grid_new[i] = 0;
    }
    old_time = time(NULL);
    printf("%d %d\n", size_x, size_y);
    setMode(spark);
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

void Automaton::setRelativeViewport(GLsizei W, GLsizei H, float R = -1) {
    if (R < 0) {
        R = window_w / float(window_h);
    }
    if (R > W / (float) H) {
        // vertical space
        float height = W / R;
        setViewport(0, W, H / 2 - height / 2, H / 2 + height / 2);
        frame_w = W;
        frame_h = height;
    } else {
        // horizontal space
        float width = H * R;
        setViewport(W / 2 - width / 2, W / 2 + width / 2, 0, H);
        frame_h = H;
        frame_w = width;
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
    window_w = W;
    window_h = H;
}

void Automaton::mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        //printf("X, Y: (%d, %d)\n", x, y);
        //printf("Window size: %d %d\n", window_w, window_h);
        int x_cell, y_cell;
       // printf("Frame: w %d, h %d\n", frame_w, frame_h);
        
        if (frame_h == window_h) {
            float left_space = (window_w - frame_w) / 2.0f;
            x_cell = int((x - left_space) / float(frame_w) * size_x);
            y_cell = int(y / float(frame_h) * size_y);
        } else {
            float vert_space = (window_h - frame_h) / 2.0f;
            x_cell = int(x / float(frame_w) * size_x);
            y_cell = int((y - vert_space) / float(frame_h) * size_y);
        }
        if (x_cell >= 0 && y_cell >= 0 && x_cell < size_x && y_cell < size_y) {
            grid_old[y_cell * size_x + x_cell] = C;
        }
        //printf("Position: (%d, %d)\n", x_cell, y_cell);
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        isRunning = !isRunning;
    }
}

void Automaton::drawCell(int i, int j, const RGBColor &color) const {
    setColor(color);
    glRectf(j, i, j + 1, i + 1);
}

void Automaton::drawGrid() const {
    int value;
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            value = grid_old[i * size_x + j];
            if (value == C) {
                drawCell(i, j, yellow);
            } else if (value > 0) {
                drawCell(i, j, orange);
            }
        }
    }
}

void Automaton::drawMesh(const RGBColor &cell, const RGBColor &chunk) const {
    glLineWidth(1);
    for (int i = 0; i <= size_x; i++) {
        if (i % cell_per_chunk == 0) {
            setColor(chunk);
        } else {
            setColor(cell);
        }
        
        glBegin(GL_LINES);
            glVertex2f(i, -size_y);
            glVertex2f(i, size_y);
        glEnd();
    }
    for (int i = 0; i <= size_y; i++) {
        if (i % cell_per_chunk == 0) {
            setColor(chunk);
        } else {
            setColor(cell);
        }
        
        glBegin(GL_LINES);
            glVertex2f(-size_x, i);
            glVertex2f(size_x, i);
        glEnd();
    }
}

int Automaton::countNeighbour(int i, int j) {
    int count = 0;
    //int left, right, bottom, top, topleft, topright, bottonleft, bottomright;
    int i_top = (i == 0 ? (size_y - 1) : (i - 1));
    int i_bottom = (i == (size_y - 1) ? 0 : (i + 1));
    int j_left = (j == 0 ? (size_x - 1) : (j - 1));
    int j_right = (j == (size_x - 1) ? 0 : (j + 1));
    /*int i_top = i - 1, i_bottom = i + 1;
    int j_left = j - 1, j_right = j + 1;*/

    int *top_line = grid_old + i_top * size_x;
    int *middle_line = grid_old + i * size_x;
    int *bottom_line = grid_old + i_bottom * size_x;
    if (top_line[j_left] == C)
        count++;
    if (top_line[j] == C)
        count++;
    if (top_line[j_right] == C)
        count++;
    if (middle_line[j_left] == C)
        count++;
    if (middle_line[j_right] == C)
        count++;
    if (bottom_line[j_left] == C)
        count++;
    if (bottom_line[j] == C)
        count++;
    if (bottom_line[j_right] == C)
        count++;
    return count;
}

void Automaton::updateGrid() {
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            int cell_old = grid_old[i * size_x + j];
            int &cell_new = grid_new[i * size_x + j];
            int neighbour_count = countNeighbour(i, j);
            if (cell_old == C && !isAlive(neighbour_count)) {
                cell_new--;
            } else if (cell_old > 0 && cell_old < C) {
                cell_new--;
            } else if (cell_old == 0 && isBorn(neighbour_count)) {
                cell_new = C;
            } else {
                cell_new = cell_old;
            }
        }
    }
    int *tmp = grid_old;
    grid_old = grid_new;
    grid_new = grid_old;
}

void Automaton::display() {
    glClear(GL_COLOR_BUFFER_BIT);

    new_time = time(NULL);
    if (isRunning) {
        if (difftime(new_time, old_time) >= update_time) {
            old_time = new_time;
            updateGrid();
        }
    }
    drawGrid();
    drawMesh(alpha_white, red);

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

void Automaton::setMode(const std::string &str) {
    int slash_count = 0;
    char sep = '/';
    std::string params[3];
    for (char letter: str) {
        if (!isdigit(letter) && letter != sep) {
            printf("Invalid argument: %c %d.\n", letter, isdigit(letter));
            return;
        }
        if (letter == sep) {
            if (params[slash_count].size() == 0) {
                printf("Invalid number.");
                return;
            }
            slash_count++;
        } else {
            params[slash_count].push_back(letter);
        }
    }
    if (slash_count != 2 || params[2].size() == 0) {
        printf("Invalid slash count.\n");
        return;
    }
    S = params[0], B = params[1], C = atoi(params[2].c_str());
    printf("Mode: [%s, %s, %d]\n", S.c_str(), B.c_str(), C);
}

bool Automaton::isAlive(int neighbour_count) const {
    if (neighbour_count < 0 || neighbour_count > 9) {
        printf("Something is wrong. Neighbour count: %d\n", neighbour_count);
        return false;
    }
    char neigh_char = '0' + neighbour_count;
    for (char letter: S) {
        if (letter == neigh_char) {
            return true;
        }
    }
    return false;
}

bool Automaton::isBorn(int neighbour_count) const {
    if (neighbour_count < 0 || neighbour_count > 9) {
        printf("Something is wrong. Neighbour count: %d\n", neighbour_count);
        return false;
    }
    char neigh_char = '0' + neighbour_count;
    for (char letter: B) {
        if (letter == neigh_char) {
            return true;
        }
    }
    return false;
}

void Automaton::run() {
    glutMainLoop();
}