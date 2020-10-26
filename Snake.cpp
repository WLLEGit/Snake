#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<time.h>
using namespace std;

enum Direction { Left, Right, Up, Down };
constexpr auto BLACK = '@';
constexpr auto WHITE = '.';
constexpr auto X = 15;
constexpr auto Y = 15;

class Node {
public:
	int x;
	int y;
	Node* pre;
	Node* next;

	Node(int x, int y, Node* pre = nullptr, Node* next = nullptr) {
		this->x = x;
		this->y = y;
		this->next = next;
		this->pre = pre;
	}
};

class Snake {
private:
	Node* head;
	Node* tail;
	int length;
	char table[X][Y];
public:
	Snake(Node* head, Node* tail);
	~Snake();
	void turn_left();
	void turn_right();
	void upwards();
	void downwards();
	void add_length();
	Direction get_next_direction();
	void run_once(Direction dir);
	void print();
};

int main() {
	Node* head = new Node(5, 5);
	Node* tail = new Node(4, 5);
	Snake snake(head, tail);
	Direction dir = Right;
	bool flag = true;



	return 0;
}


Snake::Snake(Node* head, Node* tail) {
	this->head = head;
	this->tail = tail;
	head->next = tail;
	tail->pre = head;
	length = 2;
	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 15; ++j)
			table[i][j] = WHITE;
	table[(*head).y][(*head).x] = BLACK;
	table[(*tail).y][(*tail).x] = BLACK;
}
Snake::~Snake() {
	Node* now = head;
	Node* next = head->next;
	for (int i = 0; i < length; ++i) {
		next = now->next;
		delete now;
		now = next;
	}
}
void Snake::turn_left() {
	table[(*head).y][(*head).x - 1] = BLACK;
	head = new Node((*head).x - 1, (*head).y, nullptr, head);
	head->next->pre = head;
	Node* tmp = tail;
	tail = tail->pre;
	table[(*tmp).y][(*tmp).x] = WHITE;
	delete tmp;
}
void Snake::turn_right() {
	table[(*head).y][(*head).x + 1] = BLACK;
	head = new Node((*head).x + 1, (*head).y, nullptr, head);
	head->next->pre = head;
	Node* tmp = tail;
	tail = tail->pre;
	table[(*tmp).y][(*tmp).x] = WHITE;
	delete tmp;
}
void Snake::upwards() {
	table[(*head).y - 1][(*head).x] = BLACK;
	head = new Node((*head).x, (*head).y - 1, nullptr, head);
	head->next->pre = head;
	Node* tmp = tail;
	tail = tail->pre;
	table[(*tmp).y][(*tmp).x] = WHITE;
	delete tmp;
}
void Snake::downwards() {
	table[(*head).y + 1][(*head).x] = BLACK;
	head = new Node((*head).x, (*head).y + 1, nullptr, head);
	head->next->pre = head;
	Node* tmp = tail;
	tail = tail->pre;
	table[(*tmp).y][(*tmp).x] = WHITE;
	delete tmp;
}
void Snake::add_length() {
	int pre_x = 2 * tail->x - tail->pre->x;
	int pre_y = 2 * tail->y - tail->pre->y;
	Node* new_tail = new Node(pre_x, pre_y, tail, nullptr);
	tail->next = new_tail;
	tail = new_tail;
	length++;
	table[pre_y][pre_x] = BLACK;
}
Direction Snake::get_next_direction() {
	int dx = tail->x - tail->pre->x;
	int dy = tail->y - tail->pre->y;
	if (dx == 0 && dy == 1)return Down;
	else if (dx == 0 && dy == -1)return Up;
	else if (dx == 1 && dy == 0)return Right;
	else return Left;
}

void Snake::run_once(Direction dir = Right) {
	switch (dir) {
	case Up:
		upwards();
		break;
	case Down:
		downwards();
		break;
	case Right:
		turn_right();
		break;
	case Left:
		turn_left();
		break;
	}
}
void Snake::print() {
	system("cls");
	for (int i = 0; i < Y; ++i) {
		for (int j = 0; j < X; ++j)
			cout << table[i][j];
		cout << endl;
	}
}
