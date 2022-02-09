#include <stdio.h>

int main() {
	/*
	 * %c	char
	 * %d	signed int
	 * %e	scientific notation float
	 * %f	float
	 * %g	similar to %e
	 * %hi	signed integer (short)
	 * %hu	unsigned integer (short)
	 * %i 	unsigned integer
	 * %l 	long
	 * %lf	double
	 * %lu 	unsigned long
	 *
	 * %s	string (char array only)
	 */


	int a;

	printf("Enter an integer: ");
	// taking values
	scanf("%d", &a);	// needs to be pointer (so we use address operator
	// printing values
	printf("Hello World\nYou entered: %d");

	return 0;
}
