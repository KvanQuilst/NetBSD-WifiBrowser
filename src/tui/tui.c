/*
 * Wifi Browser TUI / main
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 *
 * This software is under the terms of the BSD license.
 * See README for more details.
 */

#include <curses.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	char connects[5][256] = {"Connection 1", "Connection 2", "Connection 3", "Connection 4", "Connection 5"};
	int connectSize = sizeof connects / sizeof connects[0];

	initscr();
	noecho();
	WINDOW *win2;
	WINDOW *win;
	int height = LINES-10;
	int width = (COLS/2) - 10;
	//int startx = (LINES - height) / 2;
	//int starty = (COLS - width) / 2;
	win = newwin(height, width, 0,0);
	refresh();
	box(win, '*', '*');
	mvwprintw(win, 1, 1, "Checking for WIFI Conncection...");
	for(int i = 0; i < connectSize; i++){
 		mvwaddstr(win, (i*2+5), 1, connects[i]);
		wrefresh(win);
	}
	wrefresh(win);
	char ch;
	while((ch = getch()) != 'q'){
		switch(ch){
			case '1':
				win2 = newwin(height, width, 0, width + 1);
				mvwprintw(win2, 1, 1, "Connection 1 selected");
				box(win2, '*', '*');
				wrefresh(win2);
				break;
		}
	}
	endwin();
	return 0;
}
