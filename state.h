enum BitOp {
	BO_OR,
	BO_XOR,
	BO_AND
};

typedef struct GameState {
	int alive;
	enum BitOp op;

	int x, y;
	unsigned goal;
} GameState;

enum SquareType {
	SQ_WALL,
	SQ_EMPTY,
	SQ_ENEMY
};

typedef struct Square {
	enum SquareType t;
	void *data;
} Square;

typedef struct Enemy {
	short nbits;
	unsigned bits;
	Square *sq;
	unsigned hosts;
} Enemy;
