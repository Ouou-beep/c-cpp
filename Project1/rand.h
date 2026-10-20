#pragma once
#include <io.h>
#include <windows.h>

int rand_count = 0;
int guess_count = 0;

// Function to roll dice and return the total

int get_rand(void) {
	int sides, dice;
	printf("please enter the sides of dice:(enter '0' to quit)\n");
	scanf("%d", &sides);
	while (sides != 0) {
		if (sides < 2) {
			printf("please enter thr valid valu of sides!");
			scanf("%d", &sides);
			continue;
		}
		printf("please enter the number of dice:(enter '0' to quit)\n");
		scanf("%d", &dice);
		while (dice != 0) {
			if (dice < 0) {
				printf("please enter the valid value of dice!");
				scanf("%d", &dice);
				continue;
			}
			int total = 0;
			srand((unsigned)time(NULL));
			for (int i = dice;i > 0;i--) {
				total += (rand() % sides + 1);
			}
			printf("the total of %d dice with %d sides is: %d\n", dice, sides, total);
			rand_count++;
			break;
		}
		printf("please enter the sides of dice:(enter '0' to quit)\n");
		scanf("%d", &sides);
	}
	printf("Done.\n");
}

// Function to guess a random number between a given range

void guess(void) {
	int min, max;
	printf("please enter the min of number:(enter '-1' to quit)\n");
	scanf("%d", &min);
	while (min != -1) {
		if (min < 0) {
			printf("please enter a positive number:(enter '-1' to quit)\n");
			scanf("%d", &min);
			continue;
		}
		printf("please enter the max of number:(enter '-1' to quit)\n");
		scanf("%d", &max);
		if (max == -1) break;
		while (max <= min) {
			printf("please enter a valid max number:(enter '-1' to quit)\n");
			scanf("%d", &max);
			if (max == -1) break;
		}

		srand((unsigned)time(NULL));
		int rand1 = rand() % (max - min + 1) + min;

		int rand2=min;
		while (rand2 >= min && rand2 <= max) {
			if (rand2 > rand1) max = rand2;
			else if (rand2 < rand1) min = rand2;
			printf("please guess the number between %d and %d:", min, max);
			scanf("%d", &rand2);
			guess_count++;
			if (rand2 == rand1) {
				printf("Congratulations! You guessed the number %d correctly!\n", rand1);
				printf("You have guessed %d times.\n", guess_count);
				break;
			}
		}
		printf("please enter the min of number:(enter '-1' to quit)\n");
		scanf("%d", &min);
	}
	printf("Done.\n");
}

// Function to take notes and save them to a log file

void note(void) {
	char text[256];
	char filename[64];
	int character;
	int suffix = 0;
	SYSTEMTIME current_time;
	FILE* log;

	GetLocalTime(&current_time);
	do {
		if (suffix == 0) {
			sprintf_s(filename, sizeof(filename), "log_%04d%02d%02d_%02d%02d%02d%03d.txt",
				current_time.wYear, current_time.wMonth, current_time.wDay,
				current_time.wHour, current_time.wMinute, current_time.wSecond,
				current_time.wMilliseconds);
		}
		else {
			sprintf_s(filename, sizeof(filename), "log_%04d%02d%02d_%02d%02d%02d%03d_%d.txt",
				current_time.wYear, current_time.wMonth, current_time.wDay,
				current_time.wHour, current_time.wMinute, current_time.wSecond,
				current_time.wMilliseconds, suffix);
		}
		log = fopen(filename, "r");
		if (log != NULL) {
			fclose(log);
			suffix++;
		}
	} while (log != NULL);

	// Create a new log file for writing

	log = fopen(filename, "w");
	if (log == NULL) {
		printf("Unable to create log file.\n");
		return;
	}

	printf("Saving text to %s (enter '0' to quit):\n", filename);
	while ((character = getchar()) != '\n' && character != EOF) {
	}
	while (fgets(text, sizeof(text), stdin) != NULL) {
		if (text[0] == '0' && (text[1] == '\n' || text[1] == '\0')) {
			break;
		}
		fputs(text, log);
		printf("Text saved. Enter more text (enter '0' to quit):\n");
	}

	fclose(log);
	printf("Done.\n");
}

// Function to read notes from existing log files	

void read_notes(void) {
	struct _finddata_t file_info;
	char files[100][MAX_PATH];
	char input[32];
	char text[256];
	int character;
	int file_count = 0;
	int choice;
	intptr_t search_handle;
	FILE* log;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO console_info;
	BOOL has_console_info;

	search_handle = _findfirst("log_*.txt", &file_info);
	if (search_handle == -1) {
		printf("No log files found.\n");
		return;
	}

	do {
		if (file_count < 100) {
			sprintf_s(files[file_count], MAX_PATH, "%s", file_info.name);
			file_count++;
		}
	} while (_findnext(search_handle, &file_info) == 0);
	_findclose(search_handle);

	printf("Available log files:\n");
	for (int i = 0; i < file_count; i++) {
		printf("%d. %s\n", i + 1, files[i]);
	}

	while ((character = getchar()) != '\n' && character != EOF) {
	}
	while (1) {
		printf("Please enter the file number to read (enter '0' to return):\n");
		if (fgets(input, sizeof(input), stdin) == NULL) {
			return;
		}
		if (sscanf_s(input, "%d", &choice) == 1 && choice == 0) {
			printf("Done.\n");
			return;
		}
		if (sscanf_s(input, "%d", &choice) == 1 && choice >= 1 && choice <= file_count) {
			break;
		}
		printf("Invalid file number.\n");
	}

	log = fopen(files[choice - 1], "r");
	if (log == NULL) {
		printf("Unable to open the selected log file.\n");
		return;
	}

	printf("Contents of %s:\n", files[choice - 1]);
	has_console_info = GetConsoleScreenBufferInfo(console, &console_info);
	if (has_console_info) {
		SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	while (fgets(text, sizeof(text), log) != NULL) {
		printf("%s", text);
	}
	if (has_console_info) {
		SetConsoleTextAttribute(console, console_info.wAttributes);
	}
	fclose(log);
	printf("Done.\n");
}