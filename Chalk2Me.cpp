// Chalk2Me.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
// http://www.macs.hw.apc.uk/~ruth/year4VEs/Labs/wiiuse.html

#include <iostream>
#include <wiiuse.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include "Chalk2Me.h"

#ifdef WIIUSE_WIN32

#include <windows.h>

void wiiuse_millisleep(int durationMilliseconds) {
	Sleep(durationMilliseconds);
}

#endif
#define MAX_WIIMOTES				1


void handle_event(struct wiimote_t* wm) {
	printf("\n\n--- EVENT [id %i] ---\n", wm->unid);

	/* if a button is pressed, report it */
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)) {
		printf("A pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_B)) {
		printf("B pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
		printf("UP pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN)) {
		printf("DOWN pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT)) {
		printf("LEFT pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT)) {
		printf("RIGHT pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) {
		printf("MINUS pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) {
		printf("PLUS pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE)) {
		printf("ONE pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO)) {
		printf("TWO pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME)) {
		printf("HOME pressed\n");
	}

	/*
	 *	Pressing minus will tell the wiimote we are no longer interested in movement.
	 *	This is useful because it saves battery power.
	 */
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) {
		wiiuse_motion_sensing(wm, 0);
	}

	/*
	 *	Pressing plus will tell the wiimote we are interested in movement.
	 */
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) {
		wiiuse_motion_sensing(wm, 1);
	}

	/*
	 *	Pressing B will toggle the rumble
	 *
	 *	if B is pressed but is not held, toggle the rumble
	 */
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_B)) {
		wiiuse_toggle_rumble(wm);
	}

	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
		wiiuse_set_ir(wm, 1);
	}
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_DOWN)) {
		wiiuse_set_ir(wm, 0);
	}

	

	/* if the accelerometer is turned on then print angles */
	if (WIIUSE_USING_ACC(wm)) {
		printf("wiimote roll  = %f [%f]\n", wm->orient.roll, wm->orient.a_roll);
		printf("wiimote pitch = %f [%f]\n", wm->orient.pitch, wm->orient.a_pitch);
		printf("wiimote yaw   = %f\n", wm->orient.yaw);
	}

	/*
	 *	If IR tracking is enabled then print the coordinates
	 *	on the virtual screen that the wiimote is pointing to.
	 *
	 *	Also make sure that we see at least 1 dot.
	 */
	if (WIIUSE_USING_IR(wm)) {
		int i = 0;

		/* go through each of the 4 possible IR sources */
		for (; i < 4; ++i) {
			/* check if the source is visible */
			if (wm->ir.dot[i].visible) {
				printf("IR source %i: (%u, %u)\n", i, wm->ir.dot[i].x, wm->ir.dot[i].y);
			}
		}

		printf("IR cursor: (%u, %u)\n", wm->ir.x, wm->ir.y);
		printf("IR z distance: %f\n", wm->ir.z);
	}

}

/**
 *	@brief Callback that handles a read event.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param data		Pointer to the filled data block.
 *	@param len		Length in bytes of the data block.
 *
 *	This function is called automatically by the wiiuse library when
 *	the wiimote has returned the full data requested by a previous
 *	call to wiiuse_read_data().
 *
 *	You can read data on the wiimote, such as Mii data, if
 *	you know the offset address and the length.
 *
 *	The \a data pointer was specified on the call to wiiuse_read_data().
 *	At the time of this function being called, it is not safe to deallocate
 *	this buffer.
 */
void handle_read(struct wiimote_t* wm, byte* data, unsigned short len) {
	int i = 0;

	printf("\n\n--- DATA READ [wiimote id %i] ---\n", wm->unid);
	printf("finished read of size %i\n", len);
	for (; i < len; ++i) {
		if (!(i % 16)) {
			printf("\n");
		}
		printf("%x ", data[i]);
	}
	printf("\n\n");
}

short any_wiimote_connected(wiimote** wm, int wiimotes) {
	int i;
	if (!wm) {
		return 0;
	}

	for (i = 0; i < wiimotes; i++) {
		if (any_wiimote_connected(wm, MAX_WIIMOTES)!=0) {
			return 1;
		}
	}

	return 0;
}

void handle_ctrl_status(struct wiimote_t* wm) {
	printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", wm->unid);

	printf("attachment:      %i\n", wm->exp.type);
	printf("speaker:         %i\n", WIIUSE_USING_SPEAKER(wm));
	printf("ir:              %i\n", WIIUSE_USING_IR(wm));
	printf("leds:            %i %i %i %i\n", WIIUSE_IS_LED_SET(wm, 1), WIIUSE_IS_LED_SET(wm, 2), WIIUSE_IS_LED_SET(wm, 3), WIIUSE_IS_LED_SET(wm, 4));
	printf("battery:         %f %%\n", wm->battery_level);
}

void handle_disconnect(wiimote* wm) {
	printf("\n\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
}

void textcolor(int color)
{
	static int __BACKGROUND;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;


	GetConsoleScreenBufferInfo(h, &csbiInfo);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color + (__BACKGROUND << 4));
}
void delay(float number_of_seconds)
{
	// Converting time into milli_seconds
	float milli_seconds = 1000 * number_of_seconds;

	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + (int)milli_seconds);
}


void calibrazione(struct wiimote_t* wm, int *angolo) { //!!!!!non so quale sia il tipo di ir.dot

	
	/*
IR tracking reports an XY position on a virtual screen whose resolution is defined by the user. By default this resolution is dependent on the set aspect ratio and is:
For 16:9, 660x370
For 4:3, 560x420
This resolution can be changed by calling the function:

		void wiiuse_set_ir_vres(struct wiimote_t* wm, unsigned int x, unsigned int y);
	
The virtual screen resolution only applies to the x and y members of the IR structure and does not apply to the individual IR source positions defined in ir_t::ir_dot_t. The individual IR source coordinates are on a fixed virtual screen resolution of 1024x768 and can not be changed.

The coordinate (0,0) is at the top left hand corner of the virtual screen.*/

	printf("\nPremi l'angolo in alto a sinistra della lavagna...\n");

	while (!wm->ir.dot[0].visible) {
		angolo[0] = wm->ir.dot[0].x; //!!!!non so quale sia il tipo di ir.dot
		angolo[1] = wm->ir.dot[0].y;
	}

	delay(5);

	printf("Premi l'angolo in basso a destra della lavagna...\n");

	while (!wm->ir.dot[0].visible) {
		angolo[2] = wm->ir.dot[0].x;
		angolo[3] = wm->ir.dot[0].y;
	}

	float ratio = (angolo[1] - angolo[0]) / (float)(angolo[3] - angolo[2]);

	if (abs(16/(float)9 - ratio) > abs(4/(float)3 - ratio)) {
		wiiuse_set_aspect_ratio(wm, WIIUSE_ASPECT_4_3);
		wiiuse_set_ir_vres(wm, 1440, 1080);
	} else {
		wiiuse_set_aspect_ratio(wm, WIIUSE_ASPECT_16_9);
		wiiuse_set_ir_vres(wm, 1920, 1080);
	}

	printf("Calibrazione effettuata con successo!");




}

int main()
{
	wiimote** wiimotes = wiiuse_init(1);
	int found = wiiuse_find(wiimotes, 1, 5);
	int connected = wiiuse_connect(wiimotes, 1);


	//return paint_main();
	

	if (connected) { 
		textcolor(10);
		printf("La base di Chalk2Me e' stata trovata\n"); 
		textcolor(15);
	} else {
		textcolor(12);
		printf("Base di Chalk2Me non trovata\n");
		textcolor(15);	
		while (!wiiuse_connect(wiimotes, 1)) {
			textcolor(11);
			printf("\rIn attesa del collegamento   ");
			delay(0.3);
			printf("\rIn attesa del collegamento.  ");
			delay(0.3);
			printf("\rIn attesa del collegamento.. ");
			delay(0.3);
			printf("\rIn attesa del collegamento...");
			delay(0.3);
			textcolor(12);
		}
		textcolor(10);
		printf("La base di Chalk2Me è stata trovata\n");
		textcolor(15);
	}

	while (wiiuse_connect(wiimotes, 1)) {


		//calibrazione

		return paint_main(wiimotes);


		if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {

			wiiuse_set_ir(wiimotes[0], 1);
			wiiuse_motion_sensing(wiimotes[0], 1); //Bisogna attivare anche l'accellerometro per avere un tracking più accurato
			wiiuse_set_ir_sensitivity(wiimotes[0], 5); //Sensibilità, 5 è il massimo

			


			for (int i=0; i < 4; ++i) {
				/* check if the source is visible */
				if (wiimotes[0]->ir.dot[i].visible) {
					printf("IR source %i: (%u, %u)\n", i, wiimotes[0]->ir.dot[i].x, wiimotes[0]->ir.dot[i].y);
				}
			}

			printf("IR cursor: (%u, %u)\n", wiimotes[0]->ir.x, wiimotes[0]->ir.y);
			printf("IR z distance: %f\n", wiimotes[0]->ir.z);
			
			
		}
	}
	/*
	 *	Disconnect the wiimotes
	 */
	wiiuse_cleanup(wiimotes, MAX_WIIMOTES);

	return 0;
}

