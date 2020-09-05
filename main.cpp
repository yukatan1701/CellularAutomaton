#include <GL/gl.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

int WINDOW_H = 600;
int WINDOW_W = 800;
int chunk_per_width = 5;
int cell_per_chunk = 5;
time_t old_time, new_time;
float center_size = 10.0f;
float update_time = 1.0f;

struct RGBColor {
    int r, g, b;
    float alpha;
    RGBColor(int r, int g, int b, float alpha = 1.0f) : r(r), g(g), b(b), alpha(alpha) {}
};

RGBColor burgundy(102, 0, 0);
RGBColor white(255, 255, 255, 0.5);
RGBColor alpha_white(255, 255, 255, 0.2);
RGBColor orange(255, 128, 0);

void setBackgroundColor(const RGBColor &color) {
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.alpha);
}

void setColor(const RGBColor &color) {
    glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.alpha);
}

void reloadViewport(GLint width, GLint height) {
    if (width >= height) {
        glViewport(0, -(width - height) / 2, width, width);
    } else {
        glViewport(-(height - width) / 2, 0, height, height);
    }
}

void myInit() {
    
    setBackgroundColor(burgundy);
    setColor(white);
    reloadViewport(WINDOW_W, WINDOW_H);
    glMatrixMode(GL_PROJECTION);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    old_time = time(NULL);
}

void myReshape(GLint width, GLint height) {
    reloadViewport(width, height);
    WINDOW_W = width;
    WINDOW_H = height;
    glutPostRedisplay();
}

void myDisplay() {
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
    float rand_x, rand_y;
    int point_size;
    /*
    for (int i = 0; i < 200; i++) {
        point_size = rand() % 15;
        rand_x = 2 * (rand() / float(RAND_MAX)) - 1;
        rand_y = 2 * (rand() / float(RAND_MAX)) - 1;
        printf("[%f %f %d]\n", rand_x, rand_y, point_size);
        glPointSize(point_size);
        glBegin(GL_POINTS);
            glVertex2f(rand_x, rand_y);
        glEnd();
    }*/
    float chunk_size = 1.0f / float(chunk_per_width);
    for (int i = -chunk_per_width; i <= chunk_per_width; i++) {
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
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Cellular automaton");
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutIdleFunc(myDisplay);
    //glutMouseFunc();
    //glutKeyboardFunc();

    myInit();

    glutMainLoop();
    return 0;
}