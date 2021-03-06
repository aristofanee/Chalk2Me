#include "Chalk2Me.h"
#include "linmath.h"
#include <stdio.h>
#include <glfw3.h>
#include <stdlib.h>

#define MEMORIA 60000
#define PI 3.14158

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius, float ratio) {
	int i;
	int triangleAmount = 20;
	GLfloat twicePi = 2.0f * PI;

	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle

	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i * twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))

		);

	}
	glEnd();
}

void interpolazione(int puntoAx, int puntoAy, int puntoBx, int puntoBy, int width, int height, float radius, float ratio) {

	float m = (puntoBy - puntoAy) / (float)(puntoBx - puntoAx);
	float d = sqrt((puntoBy - puntoAy) * (puntoBy - puntoAy) + (puntoBx - puntoAx) * (puntoBx - puntoAx));

	float delta_x = 1 / sqrt(1 + m * m);
	float delta_y = delta_x / m;

	if (puntoAx > puntoBx)
		delta_x = (-1) * delta_x;

	if (puntoAy > puntoBy)
		delta_y = (-1) * delta_y;

	for (int i = 0; i < d; i++) {
		drawFilledCircle(2 * (puntoAx + i * delta_x) / (float)width - 1, (-2) * (puntoAy + i * delta_y) / (float)height + 1, radius, ratio);
	}

}

int paint_main_debug() {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	int striscia[MEMORIA][2];


	GLFWwindow* window = glfwCreateWindow(800, 800, "Finestra", NULL, NULL); //glfwGetPrimaryMonitor()
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowAspectRatio(window, 1, 1);
	glfwSwapInterval(1);

	int tempo = 0;

	const float DEG2RAD = 3.14159 / 180;
	float radius = 0.01;
	float x = 0.5;
	float y = -0.33;
	double xpos = 0, ypos = 0;

	while (!glfwWindowShouldClose(window)) {
		//setup view
		int width, height;


		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 1, 1, 1);
		float ratio = width / (float)height;

		glfwGetCursorPos(window, &xpos, &ypos);
		printf("%f %f\n", xpos, ypos);


		striscia[tempo][0] = xpos; striscia[tempo][1] = ypos;

		if (tempo > 0) {

			for (int i = 0; i < tempo; i++) {

				interpolazione(striscia[i][0], striscia[i][1], striscia[i + 1][0], striscia[i + 1][1], width, height, radius, ratio);

			}

		}



		tempo++;

		glfwSwapBuffers(window);
		glFlush();
		glfwWaitEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}



