#include <GL/glut.h>
#include <cmath>
#include <vector>

float starPosX = 0.50f;
float starPosY = 0.075f;

float boatRotation = 0.0f;
bool boatDirection = true;

std::vector<float> boatsPosX = { 0.10f, 0.50f, 0.10f };
std::vector<float> boatsPosY = { 0.225f, 0.525f, 0.825f };

float boatSpeed[] = { 0.0003f, -0.0003f, 0.0003f }; //boat movement speeds

int successCount = 0;
int failCount = 0;

float elapsedTime = 0.0f;

const float boatWidth = 0.15f;
const float boatHeight = 0.075f;
const float starHeight = 0.08f;

//function to check if there is a collision
bool checkCollision(float starPosX, int boatIndex) {
    float boatLeft = boatsPosX[boatIndex] - boatWidth / 2;
    float boatRight = boatsPosX[boatIndex] + boatWidth / 2;

    float tolerance = 0.01f; //tolerance value for error margin

    return (starPosX > boatLeft - tolerance && starPosX < boatRight + tolerance);
}

//function to draw a rectangle
void drawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

//function to draw the score elements
void drawScore() {
    float gap = 0.001f;

    //red rectangles for failed attempts
    for (int i = 0; i < failCount; i++) {
        drawRect(0.01f + i * (0.04f + gap), 0.925f, 0.02f, 0.05f, 1.0f, 0.0f, 0.0f);
    }

    //green rectangles for successful attempts
    for (int i = 0; i < successCount; i++) {
        drawRect(1.0f - 0.03f - i * (0.04f + gap), 0.925f, 0.02f, 0.05f, 0.0f, 1.0f, 0.0f);
    }
}

//function for drawing the circles
void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

//function for drawing the star
void drawStar(float x, float y, float scale) {
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y);

    const int num_points = 5;
    const float angle_step = 2.0f * 3.1415926f / num_points;
    const float inner_scale = 0.5f * scale;

    for (int i = 0; i <= 2 * num_points; ++i) {
        float angle = 3.1415926f / 2.0f - i * angle_step / 2.0f; //setting up the angle for the star
        float radius = (i % 2 == 0) ? scale : inner_scale;
        float xPos = radius * cosf(angle);
        float yPos = radius * sinf(angle);

        glVertex2f(x + xPos, y + yPos);
    }

    glEnd();
}

//function for drawing the boats (using the draw circle function)
void drawBoat(float x, float y, float angle) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f); //move to the boats position
    glRotatef(angle, 0.0f, 0.0f, 1.0f); //rotate the boat around its center

    glColor3f(0.0f, 0.0f, 1.0f);
    drawCircle(0.0f, 0.0f, 0.075f, 30);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.03f, 0.0f);
    glVertex2f(0.03f, 0.0f);
    glVertex2f(0.0f, -0.03f);
    glVertex2f(0.0f, 0.03f);
    glEnd();

    glPopMatrix();
}


//function for drawing the rivers
void drawRiver(float x, float y) {
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 1.0f, y);
    glVertex2f(x + 1.0f, y + 0.15f);
    glVertex2f(x, y + 0.15f);
    glEnd();
}

//function for drawing the grounds
void drawGround(float x, float y) {
    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 1.0f, y);
    glVertex2f(x + 1.0f, y + 0.15f);
    glVertex2f(x, y + 0.15f);
    glEnd();
}

//function for drawing the figures
void drawFigures() {
    //drawing 4 lines on the left side with the black color
    glColor3f(0.0f, 0.0f, 0.0f);
    for (float i = 0.0f; i < 0.4f; i += 0.1f) {
        glBegin(GL_LINES);
        glVertex2f(i, 0.05f);
        glVertex2f(i + 0.025f, 0.1f);
        glEnd();
    }

    //drawing 4 lines on the right side with the black color
    glColor3f(0.0f, 0.0f, 0.0f);
    for (float i = 0.6f; i < 1.0f; i += 0.1f) {
        glBegin(GL_LINES);
        glVertex2f(i, 0.05f);
        glVertex2f(i + 0.025f, 0.1f);
        glEnd();
    }

    //drawing 3 squares between the lines on the left side with the red color
    glColor3f(1.0f, 0.0f, 0.0f);
    for (float i = 0.05f; i < 0.35f; i += 0.1f) {
        glBegin(GL_QUADS);
        glVertex2f(i + 0.015f, 0.075f);
        glVertex2f(i + 0.035f, 0.075f);
        glVertex2f(i + 0.035f, 0.095f);
        glVertex2f(i + 0.015f, 0.095f);
        glEnd();
    }

    //drawing 3 squares between the lines on the right side with the red color
    glColor3f(1.0f, 0.0f, 0.0f);
    for (float i = 0.65f; i < 0.95f; i += 0.1f) {
        glBegin(GL_QUADS);
        glVertex2f(i + 0.015f, 0.075f);
        glVertex2f(i + 0.035f, 0.075f);
        glVertex2f(i + 0.035f, 0.095f);
        glVertex2f(i + 0.015f, 0.095f);
        glEnd();
    }
}

//function for displaying the contents
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < 4; i++) {
        drawGround(0.0f, 0.0f + i * 0.3f);
    }

    elapsedTime = glutGet(GLUT_ELAPSED_TIME) * 0.0005f;

    for (int i = 0; i < 3; i++) {
        //update boat positions and bounce if they reach the borders
        boatsPosX[i] += boatSpeed[i];
        if (boatsPosX[i] <= 0.1f || boatsPosX[i] >= 0.9f) {
            boatSpeed[i] = -boatSpeed[i];
        }

        drawRiver(0.0f, 0.15f + i * 0.3f);
        drawBoat(boatsPosX[i], 0.225f + i * 0.3f, (i % 2 == 0) ? boatRotation : -boatRotation);
    }

    drawStar(starPosX, starPosY, 0.04f);

    drawScore();
    drawFigures();

    glutSwapBuffers();
    glutPostRedisplay();
}

//function for reshaping
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//function for assigning operations for keybord keys
void keyboard(unsigned char key, int x, int y) {
    if (key == 'x' || key == 'X') {
        exit(0);
    }
}

//function for assigning an operation for special keybord keys
void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        bool collision = false;

        int currentGround = (int)(starPosY / 0.3f);
        float newStarPosY = (currentGround + 1) * 0.3f + 0.075f;

        for (int i = 0; i < 3; ++i) {
            if (checkCollision(starPosX, i)) {
                collision = true;
                starPosY = newStarPosY;

                if (starPosY > 0.9f) {
                    starPosY = 0.075f;
                }
                successCount++;
                break;
            }
        }

        if (!collision) {
            starPosY = 0.075f;
            failCount++;
        }
    }

    glutPostRedisplay();
}


//function for animating the boats
void animate(int value) {
    boatRotation += boatDirection ? 2.0f : -2.0f;

    if (boatRotation > 45.0f || boatRotation < -45.0f) {
        boatDirection = !boatDirection;
    }

    glutPostRedisplay();
    glutTimerFunc(25, animate, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Assignment");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutTimerFunc(0, animate, 0);

    glutMainLoop();

    return 0;
}