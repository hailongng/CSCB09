#include "aux.h"

int min(int a, int b) {
	if (a <= b) {
		return a;
	}
	return b;
}

int max(int a, int b) {
	if (a >= b) {
		return a;
	}
	return b;
}

int find_index(double value) {

	// Helper function to find the index on a graph to plot the data

	if ((value >= 0) && (value < 10)) {
		return 9;
	}
	else if (value < 20) {
		return 8;
	}
	else if (value < 30) {
		return 7;
	}
	else if (value < 40) {
		return 6;
	}
	else if (value < 50) {
		return 5;
	}
	else if (value < 60) {
		return 4;
	}
	else if (value < 70) {
		return 3;
	}
	else if (value < 80) {
		return 2;
	}
	else if (value < 90) {
		return 1;
	}
	return 0;
}

