// Chalk2Me.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include <wiiuse.h>
#include <stdio.h>
#ifdef WIIUSE_WIN32

#include <windows.h>

void wiiuse_millisleep(int durationMilliseconds) {
	Sleep(durationMilliseconds);
}

#endif

int main()
{
    printf("Hello World!\n");
	wiimote** wiimotes = wiiuse_init(2);

}

