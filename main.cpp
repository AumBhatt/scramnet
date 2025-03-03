#include <iostream>
#include "network/network.hpp"
#include "url/url.hpp"
#include <string>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

std::string response;

void displayText(std::string text, float x, float y) {
    glColor3f(255.0, 255.0, 255.0);
    glRasterPos2f(x, y);
    for (std::string::iterator c = text.begin(); c != text.end(); ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glutSwapBuffers();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(
        0.0, 0.0, 0.5,
        0.0, 0.0, 0.5,
        0.0, 0.1, 0.0
    );

    displayText(response, 0.0f, -10.0f);
}

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        std::cout << "Missing arguments" << "\nUsage:" << "\n ./url <website_url>" << std::endl;
        return -1;
    }
    URL u(argv[1]);
    Socket s(u.hostname, u.scheme);
    response = s.request(u.hostname);
    //
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
    glutCreateWindow("Hello, World!!!");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
