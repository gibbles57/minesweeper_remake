/*
Nicholas Gibbs G01482635
CS 262, Project 2
This program runs a game similar to Minesweeper.
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int get_bomb_percent(char * level) {
	if (strcmp(level, "easy\0") == 0) {
		return 5;
	} else if (strcmp(level, "medium\0") == 0) {
		return 20;
	} else if (strcmp(level, "hard\0") == 0) {
             	return 35;
	} else {
		printf("Error! Invalid input for difficulty, please enter [easy|medium|hard]\n");
		exit(0);
	}
}

int get_num_bomb(int rows, int cols, int bomb_percent) {
	return (rows * cols) * bomb_percent / 100;
}

char ** setup(int rows, int cols) {
	char ** temp = calloc(rows, sizeof(char *));
	for (int i = 0; i < rows; i++) {
		temp[i] = calloc(cols, 1);
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			temp[i][j] = '.';
		}
	}
	return temp;
}

void init(char ** gboard, int rows, int cols, int num_bomb) {
	int bomb_count = 0;
	while (bomb_count < num_bomb) {
		int randRow = rand() % rows;
		int randCol = rand() % cols;
		if (gboard[randRow][randCol] != '*') {
			gboard[randRow][randCol] = '*';
			bomb_count += 1;
		}
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (gboard[i][j] == '*') {
				continue;
			}
			int near_count = 0;
			int bounds[] = {0, 3, 0, 3};
			if (i == 0) {
				bounds[0] = 1;
			} else if (i == rows - 1) {
				bounds[1] = 2;
			}
			if (j == 0) {
				bounds[2] = 1;
			} else if (j == cols - 1) {
				bounds[3] = 2;
			}
			for (int k = bounds[0]; k < bounds[1]; k++) {
				for (int l = bounds[2]; l < bounds[3]; l++) {
						if (gboard[i - 1 + k][j - 1 + l] == '*') {
							near_count += 1;
						}	
				}
			}
			gboard[i][j] = near_count + '0';
		}
	}
}

void print_board(char ** board, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void walker(char ** pboard, char ** gboard, int rows, int cols, int click_r, int click_c) {
	pboard[click_r][click_c] = gboard[click_r][click_c];
	if (pboard[click_r][click_c] == '0') {
		int bounds[] = {0, 3, 0, 3};
		if (click_r == 0) {
			bounds[0] = 1;
		} else if (click_r == rows - 1) {
			bounds[1] = 2;
		}
		if (click_c == 0) {
			bounds[2] = 1;
		} else if (click_c == cols - 1) {
			bounds[3] = 2;
		}
		for (int i = bounds[0]; i < bounds[1]; i++) {
			for (int j = bounds[2]; j < bounds[3]; j++) {
				if (pboard[click_r - 1 + i][click_c - 1 + j] == '.') {
					walker(pboard, gboard, rows, cols, click_r - 1 + i, click_c - 1 + j);
				}
			}
		}
	}
}

void play(char ** pboard, char ** gboard, int rows, int cols, int num_bomb) {
	int click_r = rand() % rows;
	int click_c = rand() % cols;
	while (gboard[click_r][click_c] != '*' && pboard[click_r][click_c] != '.') {
		click_r = rand() % rows;
		click_c = rand() % cols;	
	}
	if (gboard[click_r][click_c] == '*') {
		printf("Click at (%d, %d). Bomb Exploded! Game Over!\n", click_r, click_c);
                printf("This is the play board after explosion\n");
                pboard[click_r][click_c] = '!';
                print_board(pboard, rows, cols);
		free(pboard);
        	free(gboard);
        	exit(0);
        } else {
		printf("Click at (%d, %d)\n", click_r, click_c);
		walker(pboard, gboard, rows, cols, click_r, click_c);
		print_board(pboard, rows, cols);
	}
	int count = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (pboard[i][j] != '.') {
				count += 1;
			}
		}
	}
	if (count < rows * cols - num_bomb) {
		play(pboard, gboard, rows, cols, num_bomb);
	} else {
		printf("Game Completed!\n");
		print_board(pboard, rows, cols);
		for (int i = 0; i < rows; i++) {
			free(pboard[i]);
		}
		for (int i = 0; i < rows; i++) {
                        free(gboard[i]);
                }
		free(pboard);
		free(gboard);
		exit(0);
	}
}

int main(int argc, char ** argv) {
	int rows = 0;
	int cols = 0;
	int difficulty = 0;
	for (int i = 1; i < argc; i++) {
		if (i < 3) {
			int temp = atoi(argv[i]);
			if (temp > 0) {
				if (i == 1) {
					rows = temp;
				} else {
					cols = temp;
				}
			} else {
				printf("Error! Invalid input for %s, please enter a positive integer.\n", (i == 1)?"rows":"columns");
			exit(0);
			}
		} else if (i == 3) {
			difficulty = get_bomb_percent(argv[i]);
		} else if (i == 4) {
			srand(atoi(argv[i]));
		}
	}
	int bombs = get_num_bomb(rows, cols, difficulty);
	char ** gboard = setup(rows, cols);
	char ** pboard = setup(rows, cols);
	init(gboard, rows, cols, bombs);
	printf("This is the game board after initialization\n");
	print_board(gboard, rows, cols);
	printf("This is the play board after setup\n");
	print_board(pboard, rows, cols);
	printf("The game will start\n");
	play(pboard, gboard, rows, cols, bombs);
	return 0;
}
