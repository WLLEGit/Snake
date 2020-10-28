#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<time.h>
using namespace std;

enum Direction { Left, Right, Up, Down };

constexpr auto BLACK = '@';
constexpr auto WHITE = '.';
constexpr auto X = 30;
constexpr auto Y = 15;
constexpr auto X1000 = X * 1000;
constexpr auto Y1000 = Y * 1000;
constexpr auto PAUSE = 300;
constexpr auto CANDY = '$';
constexpr clock_t CANDY_DURATION = 6500;
constexpr clock_t CANDY_APPEAR = 10000;

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
	char table[Y][X];
	int candy_x, candy_y;
public:
	Snake(Node* head, Node* tail);
	~Snake();
	int turn_left();
	int turn_right();
	int upwards();
	int downwards();
	void add_length();
	Direction get_next_tail();
	Direction get_forward_dir();
	int run_once(Direction dir);
	void print();
	void creat_candy();
	bool is_get_candy(Direction);
	void destory_candy();
};

Direction convert_to_dir(int, Direction);

int main() {

	srand(time(NULL));

	Node* head = new Node(5, 5);
	Node* tail = new Node(4, 5);
	clock_t start;
	clock_t time;
	clock_t candy_clock = clock() + 10000;
	Snake snake(head, tail);
	Direction dir = Right;
	bool is_getch = false;
	bool flag = true;
	bool is_candy_exist = false;
	start = clock();

	while (flag) {
		is_getch = false;
		dir = snake.get_forward_dir();
		time = clock();
		while (clock() - time <= (clock_t)PAUSE) {
			if (!is_getch && _kbhit()) {
				_getch();
				int in = _getch();
				cin.sync();
				dir = convert_to_dir(in, dir);
				is_getch = true;
			}
		}
		if (snake.is_get_candy(dir)) {
			snake.add_length();
			snake.print();
		}
		if ((clock() - start) % CANDY_APPEAR < 300) {
			is_candy_exist = true;
			snake.creat_candy();
			candy_clock = clock();
		}
		if (is_candy_exist && clock() - candy_clock > CANDY_DURATION) {
			snake.destory_candy();
			is_candy_exist = false;
		}

		if (snake.run_once(dir) == -1)
			flag = false;
		snake.print();
	}

	system("pause");
	return 0;
}


Snake::Snake(Node* head, Node* tail) {
	this->head = head;
	this->tail = tail;
	head->next = tail;
	tail->pre = head;
	candy_x = 32767;
	candy_y = 32767;
	length = 2;
	for (int i = 0; i < Y; ++i)
		for (int j = 0; j < X; ++j)
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
int Snake::turn_left() {
	int tmp_x = (((*head).x - 1) + X1000) % X;
	int tmp_y = (((*head).y + Y1000)) % Y;
	if (table[tmp_y][tmp_x] == BLACK) return -1;
	table[tmp_y][tmp_x] = BLACK;

	head = new Node((*head).x - 1, (*head).y, nullptr, head);
	head->next->pre = head;
	Node* tmp = tail;
	tail = tail->pre;
	table[((*tmp).y + Y1000) % Y][((*tmp).x + X1000) % X] = WHITE;
	delete tmp;
}
int Snake::turn_right() {
	int tmp_x = (((*head).x + 1) + X1000) % X;
	int tmp_y = ((*head).y + Y1000) % Y;
	if (table[tmp_y][tmp_x] == BLACK) return -1;
	table[tmp_y][tmp_x] = BLACK;
	head = new Node((*head).x + 1, (*head).y, nullptr, head);
	head->next->pre = head;
	Node* tmp = tail;
	tail = tail->pre;
	table[((*tmp).y + Y1000) % Y][((*tmp).x + X1000) % X] = WHITE;
	delete tmp;
}
int Snake::upwards() {
	int tmp_x = ((*head).x + X1000) % X;
	int tmp_y = ((*head).y - 1 + Y1000) % Y;
	if (table[tmp_y][tmp_x] == BLACK) return -1;
	table[tmp_y][tmp_x] = BLACK;
	head = new Node((*head).x, (*head).y - 1, nullptr, head);
	head->next->pre = head;
	Node* tmp = tail;
	tail = tail->pre;
	table[((*tmp).y + Y1000) % Y][((*tmp).x + X1000) % X] = WHITE;
	delete tmp;
}
int Snake::downwards() {
	int tmp_x = ((*head).x + X1000) % X;
	int tmp_y = ((*head).y + 1 + Y1000) % Y;
	if (table[tmp_y][tmp_x] == BLACK) return -1;
	table[tmp_y][tmp_x] = BLACK;
	head = new Node((*head).x, (*head).y + 1, nullptr, head);
	head->next->pre = head;
	Node* tmp = tail;
	tail = tail->pre;
	table[((*tmp).y + Y1000) % Y][((*tmp).x + X1000) % X] = WHITE;
	delete tmp;
}
void Snake::add_length() {
	int pre_x = 2 * tail->x - tail->pre->x;
	int pre_y = 2 * tail->y - tail->pre->y;
	Node* new_tail = new Node(pre_x, pre_y, tail, nullptr);
	tail->next = new_tail;
	tail = new_tail;
	length++;
	table[(pre_y + Y1000) % Y][(pre_x + X1000) % X] = BLACK;
}
Direction Snake::get_next_tail() {
	int dx = tail->x - tail->pre->x;
	int dy = tail->y - tail->pre->y;
	if (dx == 0 && dy == 1)return Down;
	else if (dx == 0 && dy == -1)return Up;
	else if (dx == 1 && dy == 0)return Right;
	else return Left;
}

int Snake::run_once(Direction dir = Right) {
	switch (dir) {
	case Up:
		return upwards();
		break;
	case Down:
		return downwards();
		break;
	case Right:
		return turn_right();
		break;
	case Left:
		return turn_left();
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
Direction Snake::get_forward_dir() {
	int dx = head->x - head->next->x;
	int dy = head->y - head->next->y;
	if (dx == 0 && dy == 1)return Down;
	else if (dx == 0 && dy == -1)return Up;
	else if (dx == 1 && dy == 0)return Right;
	else return Left;
}
void Snake::creat_candy() {
	int rand_x = rand() % X;
	int rand_y = rand() % Y;
	while (table[rand_y][rand_x] == BLACK) {
		int rand_x = rand() % X;
		int rand_y = rand() % Y;
	}
	table[rand_y][rand_x] = CANDY;
	candy_x = rand_x;
	candy_y = rand_y;
}
bool Snake::is_get_candy(Direction dir) {
	switch (dir)
	{
	case Left:
		return (head->x + X1000 - 1) % X == candy_x && (head->y + Y1000) % Y == candy_y;
		break;
	case Right:
		return (head->x + X1000 + 1) % X == candy_x && (head->y + Y1000) % Y == candy_y;
		break;
	case Up:
		return (head->x + X1000) % X == candy_x && (head->y + Y1000 - 1) % Y == candy_y;
		break;
	case Down:
		return (head->x + X1000) % X == candy_x && (head->y + Y1000 + 1) % Y == candy_y;
		break;
	default:
		break;
	}
}
void Snake::destory_candy() {
	if (table[candy_y][candy_x] == CANDY)
		table[candy_y][candy_x] = WHITE;
	candy_x = 32767;
	candy_y = 32767;
}



Direction convert_to_dir(int in, Direction dir) {
	switch (in)
	{
	case 72:
		if (dir == Up || dir == Down) return dir;
		return Up;
	case 75:
		if (dir == Left || dir == Right) return dir;
		return Left;
	case 77:
		if (dir == Left || dir == Right) return dir;
		return Right;
	case 80:
		if (dir == Up || dir == Down) return dir;
		return Down;
	}
}