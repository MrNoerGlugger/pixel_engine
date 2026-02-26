#include "Position.hpp"

Position::Position(int x, int y) {
	this->x = x;
	this->y = y;
}
Position::Position() {};
int Position::getX() const {
	return x;
}
int Position::getY() const {
	return y;
}
void Position::setX(int x) {
	this->x = x;
}
void Position::setY(int y) {
	this->y = y;
}

int Position::directionToX(int dir) {
	int x = 0;
	if (dir > 0 && dir < 4) {
		x = -1;
	}
	else if (dir > 4 && dir < 8) {
		x = 1;
	}
	return x;
}
int Position::directionToY(int dir) {
	int y = 0;
	if (dir < 2 || dir > 6) {
		y = 1;
	}
	if (dir > 2 && dir < 6) {
		y = -1;
	}
	return y;
}