#define WND_WIDTH  1000
#define WND_HEIGHT 600
#define TOP_BAR_PCNT 10
#define FONT None
#define H_SQ_PER_WND 18
#define V_SQ_PER_WND 12
#define N_X_PAGES 10
#define N_Y_PAGES 10

#define ENABLE_GRID 1

/* calculations below, DO NOT edit for configuration */
#define TOP_BAR_HEIGHT ((unsigned)(TOP_BAR_PCNT / (double)100 * WND_HEIGHT))
#define GAMEAREA_HEIGHT (WND_HEIGHT - TOP_BAR_HEIGHT)
#define SQ_WIDTH (WND_WIDTH / H_SQ_PER_WND)
#define SQ_HEIGHT (GAMEAREA_HEIGHT / V_SQ_PER_WND)
