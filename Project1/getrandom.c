#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "rand.h"

extern int get_rand(void);
extern void guess(void);

int main() {
	while (1) {
		int min_choice = 1, max_choice = 3, choice;
		printf("Welcome to the random number generator!\n"
			"What would you like to do?\n"
			"1. Roll dice\n"
			"2. Guess a number\n"
			"3. Quit\n"
			"Please enter your choice (1-3): \n"
		);
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			get_rand();
			printf("You have rolled the dice %d times.\n", rand_count);
			break;
		case 2:
			guess();
			break;
		case 3:
			printf("Thank you for playing!\n");
			goto end;
		default:
			printf("Invalid choice. Please enter a number between %d and %d.\n", min_choice, max_choice);
			break;
		}
	}
end:
	return 0;
}