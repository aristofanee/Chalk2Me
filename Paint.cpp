
#include "Chalk2Me.h"
#include "linmath.h"
#include <stdio.h>
#include <glfw3.h>
#include <stdlib.h>
#include <wiiuse.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <math.h>

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

void interpolazione(int puntoAx, int puntoAy, int puntoBx, int puntoBy,int width, int height, float radius, float ratio) {

	if (puntoAx == -1)
		return;
	if (puntoBx == -1)
		return;

	double d = sqrt((puntoBy - puntoAy) * (puntoBy - puntoAy) + (puntoBx - puntoAx) * (puntoBx - puntoAx));
	double m;
	double delta_y;
	double delta_x;

	if (puntoBx != puntoAx) {
		m = (puntoBy - puntoAy) / (float)(puntoBx - puntoAx);
		 delta_x = 1 / sqrt(1 + m * m);

		if (puntoAx > puntoBx)
			delta_x = (-1) * delta_x;

		delta_y = delta_x * m;

		

	}
	else if (puntoBx==puntoAx) {

		delta_x = 0;

		if (puntoAy < puntoBy)
			delta_y = 1;
		else
			delta_y = -1;
	}


	for (int i = 0; i < d; i++ ) {
		drawFilledCircle( 2*(puntoAx + i * delta_x)/(float)width - 1, (-2) * (puntoAy + i * delta_y) / (float)height + 1, radius, ratio);
	}

}




int paint_main(struct wiimote_t** wiimotes) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	int striscia[MEMORIA][2];	
	

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Finestra", NULL, NULL); //glfwGetPrimaryMonitor()
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	//glfwSetWindowAspectRatio(window, 1, 1);
	glfwSwapInterval(1);

	

	const float DEG2RAD = 3.14159 / 180;
	float radius = 0.005;
	float x = 0.5;
	float y = -0.33;
	double xpos = 0, ypos = 0;
	int tempo = 0;
	int premuto = 0;
	
		while (!glfwWindowShouldClose(window)) {
		//setup view
		int width, height;
	

		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 1, 1, 1);
		float ratio = width / (float)height;
		
			glfwGetCursorPos(window, &xpos, &ypos);
			//printf("%f %f\n", xpos, ypos);

			
			if (wiiuse_poll(wiimotes, 1)) {

				wiiuse_set_ir(wiimotes[0], 1);
				wiiuse_motion_sensing(wiimotes[0], 1); //Bisogna attivare anche l'accellerometro per avere un tracking più accurato
				wiiuse_set_ir_sensitivity(wiimotes[0], 5); //Sensibilità, 5 è il massimo


				if (wiimotes[0]->ir.dot[0].visible) {
					premuto = 1;
					striscia[tempo][0] = wiimotes[0]->ir.x; striscia[tempo][1] = wiimotes[0]->ir.y;

				}
				else if (!(wiimotes[0]->ir.dot[0].visible) && (premuto == 1)) {
					striscia[tempo][0] = -1;
					tempo++;
					premuto = 0;
					printf("rilasaciato");
				}
				else {

					premuto = 0;
				}
			}
			
			/*
			int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
			if (state == GLFW_PRESS)
			{
				premuto = 1;
				striscia[tempo][0] = xpos; striscia[tempo][1] = ypos;
			}
			else if ((state == GLFW_RELEASE) && (premuto == 1)) {
				striscia[tempo][0] = -1;
				tempo++;
				premuto = 0;
				printf("rilasaciato");
			}
			else {
				premuto = 0;
			}
			*/

			if (tempo > 0) {

				for (int i = 0; i < tempo; i++) {

					interpolazione(striscia[i][0], striscia[i][1], striscia[i + 1][0], striscia[i + 1][1], width, height, radius, ratio);

				}

			}
			

			if (premuto == 1)
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



