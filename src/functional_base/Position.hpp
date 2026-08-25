#pragma once
#include <iostream>

class Position
{
	int x = 0;
	int y = 0;
public:
	Position(int x, int y);
	Position();
	int getX() const;
	int getY() const;
	void setX(int x);
	void setY(int y);

	static int directionToX(int dir);
	static int directionToY(int dir);

	friend bool operator==(Position pos1, Position pos2) {
		return pos1.getX() == pos2.getX() && pos1.getY() == pos2.getY();
	}
	friend bool operator<(Position pos1, Position pos2) {
		return pos1.getY() < pos2.getY() ? true : pos1.getY() == pos2.getY() ? pos1.getX() < pos2.getX() : false;
	}
	friend Position operator+(Position pos1, Position pos2) {
		return Position(pos1.getX() + pos2.getX(), pos1.getY() + pos2.getY());
	}
	friend std::ostream& operator<<(std::ostream& stream, const Position pos) {
		stream << "x:" << pos.getX() << ",y:" << pos.getY() << "\n";
		return stream;
	}
};

