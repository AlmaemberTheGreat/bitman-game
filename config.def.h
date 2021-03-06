#define WND_WIDTH  1000
#define WND_HEIGHT 600
#define TOP_BAR_PCNT 10
#define FONT None
#define H_SQ_PER_WND 18
#define V_SQ_PER_WND 12
#define N_X_PAGES 3
#define N_Y_PAGES 3

#define ENABLE_GRID 0

#define MAX_ROOMS 100
#define MAX_ROOM_WIDTH 6
#define MIN_ROOM_WIDTH 3
#define MAX_ROOM_HEIGHT 6
#define MIN_ROOM_HEIGHT 3

#define N_ENEMIES 50

#define BATTLE_SECONDS 40

/* calculations below, DO NOT edit for configuration */
#define TOP_BAR_HEIGHT ((unsigned)(TOP_BAR_PCNT / (double)100 * WND_HEIGHT))
#define GAMEAREA_HEIGHT (WND_HEIGHT - TOP_BAR_HEIGHT)
#define SQ_WIDTH (WND_WIDTH / H_SQ_PER_WND)
#define SQ_HEIGHT (GAMEAREA_HEIGHT / V_SQ_PER_WND)
