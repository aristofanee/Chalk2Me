// Chalk2Me.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
// http://www.macs.hw.ac.uk/~ruth/year4VEs/Labs/wiiuse.html

#include <iostream>
#include <wiiuse.h>
#include <stdio.h>
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

int main()
{
    printf("Hello World!\n");
	wiimote** wiimotes = wiiuse_init(1);
	int found = wiiuse_find(wiimotes, 1, 5);

	int connected = wiiuse_connect(wiimotes, 1);
	if (connected)
		printf("La base di Chalk2Me è stata trovata.\n");
	else {
		printf("Base di Chalk2Me non trovata.\n");
	
	}
	while (any_wiimote_connected(wiimotes, MAX_WIIMOTES)) {
		if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
			/*
			 *	This happens if something happened on any wiimote.
			 *	So go through each one and check if anything happened.
			 */
			int i = 0;
			for (; i < MAX_WIIMOTES; ++i) {
				switch (wiimotes[i]->event) {
				case WIIUSE_EVENT:
					/* a generic event occurred */
					handle_event(wiimotes[i]);
					break;

				case WIIUSE_STATUS:
					/* a status event occurred */
					handle_ctrl_status(wiimotes[i]);
					break;

				case WIIUSE_DISCONNECT:
				case WIIUSE_UNEXPECTED_DISCONNECT:
					/* the wiimote disconnected */
					handle_disconnect(wiimotes[i]);
					break;

				case WIIUSE_READ_DATA:
					/*
					 *	Data we requested to read was returned.
					 *	Take a look at wiimotes[i]->read_req
					 *	for the data.
					 */
					break;

				case WIIUSE_NUNCHUK_INSERTED:
					/*
					 *	a nunchuk was inserted
					 *	This is a good place to set any nunchuk specific
					 *	threshold values.  By default they are the same
					 *	as the wiimote.
					 */
					 /* wiiuse_set_nunchuk_orient_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 90.0f); */
					 /* wiiuse_set_nunchuk_accel_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 100); */
					printf("Nunchuk inserted.\n");
					break;
				default:
					break;
				}
			}
		}
	}

	/*
	 *	Disconnect the wiimotes
	 */
	wiiuse_cleanup(wiimotes, MAX_WIIMOTES);



}

