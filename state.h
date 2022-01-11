enum BitOp {
	BO_OR,
	BO_XOR,
	BO_AND
};

typedef struct GameState {
	int alive;
	enum BitOp op;

	int x, y;
} GameState;
