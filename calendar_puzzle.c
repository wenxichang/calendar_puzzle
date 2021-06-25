#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct cord_st {
	int y;
	int x;
} cord_st;

typedef struct pile_st {
	cord_st cord;
	char pile[4][4];
} pile_st;

typedef struct block_st {
	int nr_pile;
	pile_st piles[8];
} block_st;

static block_st blocks[8] = {
	{
		.nr_pile = 4, .piles = {{
			.pile = {
				{ 1, 1 },
				{ 0, 1 },
				{ 1, 1 },
			}
		}}
	},
	{
		.nr_pile = 2, .piles = {{
			.pile = {
				{ 3, 3, 3 },
				{ 3, 3, 3 },
			}
		}}
	},
	{
		.nr_pile = 8, .piles = {{
			.pile = {
				{ 5, 0 },
				{ 5, 5 },
				{ 5, 5 },
			}
		}}
	},
	{
		.nr_pile = 8, .piles = {{
			.pile = {
				{ 0, 0, 7 },
				{ 7, 7, 7},
				{ 7, 0, 0 },
			}
		}}
	},
	{
		.nr_pile = 8, .piles = {{
			.pile = {
				{ 0, 0, 9 },
				{ 0, 0, 9 },
				{ 9, 9, 9 },
			}
		}}
	},
	{
		.nr_pile = 8, .piles = {{
			.pile = {
				{ 11, 11 },
				{ 11, 0 },
				{ 11, 0 },
				{ 11, 0 },
			}
		}}
	},
	{
		.nr_pile = 8, .piles = {{
			.pile = {
				{ 13, 13, 13, 0 },
				{  0,  0, 13, 13 },
			}
		}}
	},
	{
		.nr_pile = 8, .piles = {{
			.pile = {
				{ 15, 15, 15, 15 },
				{  0, 15,  0,  0 },
			}
		}}
	}
};

typedef struct map_st {
	int used;
	char m[13][13];
} map_st;

static const map_st origin_map = {
	.used = 0,
	.m = {
		{ 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 },
		{ 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 },
		{ 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 },
		{ 17, 17, 17,  0,  0,  0,  0,  0,  0, 17, 17, 17, 17 },
		{ 17, 17, 17,  0,  0,  0,  0,  0,  0, 17, 17, 17, 17 },
		{ 17, 17, 17,  0,  0,  0,  0,  0,  0,  0, 17, 17, 17 },
		{ 17, 17, 17,  0,  0,  0,  0,  0,  0,  0, 17, 17, 17 },
		{ 17, 17, 17,  0,  0,  0,  0,  0,  0,  0, 17, 17, 17 },
		{ 17, 17, 17,  0,  0,  0,  0,  0,  0,  0, 17, 17, 17 },
		{ 17, 17, 17,  0,  0,  0, 17, 17, 17, 17, 17, 17, 17 },
		{ 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 },
		{ 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 },
		{ 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 },
	}
};

static void rotate(char f[4][4], char t[4][4])
{
	int i, j;
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			t[j][3 - i] = f[i][j];
		}
	}
}

static void flip(char f[4][4], char t[4][4])
{
	int i, j;
	for (i = 0; i <  4; ++i) {
		for (j = 0; j < 4; ++j) {
			t[i][3 - j] = f[i][j];
		}
	}
}

static void init_cord(pile_st *p)
{
	int i, j;
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			if (p->pile[i][j]) {
				p->cord.y = i;
				p->cord.x = j;
				return;
			}
		}
	}
	assert(0);
}

static void init_block(block_st *b)
{
	rotate(b->piles[0].pile, b->piles[1].pile);
	if (b->nr_pile > 2) {
		rotate(b->piles[1].pile, b->piles[2].pile);
		rotate(b->piles[2].pile, b->piles[3].pile);
		if (b->nr_pile > 4) {
			flip(b->piles[0].pile, b->piles[4].pile);
			flip(b->piles[1].pile, b->piles[5].pile);
			flip(b->piles[2].pile, b->piles[6].pile);
			flip(b->piles[3].pile, b->piles[7].pile);
		}
	}

	int i;
	for (i = 0; i < b->nr_pile; ++i) {
		init_cord(&b->piles[i]);
	}
}

static void init_blocks(void)
{
	int i;
	for (i = 0; i < 8; ++i) {
		init_block(&blocks[i]);
	}
}

static map_st *fill(map_st *old, int y, int x, pile_st *p)
{
	int sy = y - p->cord.y;
	int sx = x - p->cord.x;

	if (((*(unsigned int *)(&old->m[sy][sx])) & (*(unsigned int *)(&p->pile[0][0]))) != 0)
		return NULL;
	if (((*(unsigned int *)(&old->m[sy + 1][sx])) & (*(unsigned int *)(&p->pile[1][0]))) != 0)
		return NULL;
	if (((*(unsigned int *)(&old->m[sy + 2][sx])) & (*(unsigned int *)(&p->pile[2][0]))) != 0)
		return NULL;
	if (((*(unsigned int *)(&old->m[sy + 3][sx])) & (*(unsigned int *)(&p->pile[3][0]))) != 0)
		return NULL;
	
	map_st *news = malloc(sizeof(map_st));
	memcpy(news, old, sizeof(map_st));

	int i, j;
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			if (p->pile[i][j]) {
				news->m[sy + i][sx + j] = p->pile[i][j];
			}
		}
	}

	return news;
}

static void show_map(map_st *s)
{
	const char *str = " @ $ % T * X > <      ";
	int i, j;

	for (i = 3; i < 10; ++i) {
		for (j = 3; j < 10; ++j) {
			printf(" %c", str[(int)s->m[i][j]]);
		}
		printf("\n");
	}
}

static int fill_and_check(map_st *s)
{
	int i, j;
	int x, y;

	for (i = 3; i < 10; ++i) {
		for (j = 3; j < 10; ++j) {
			if (s->m[i][j] == 0) {
				y = i;
				x = j;
				goto start_fill;
			}
		}
	}

start_fill:
	for (i = 0; i < 8; ++i) {
		if (s->used & (1 << i))
			continue;
		
		for (j = 0; j < blocks[i].nr_pile; ++j) {
			map_st *news = fill(s, y, x, &blocks[i].piles[j]);
			if (!news)
				continue;
			
			news->used |= (1 << i);
			if (news->used == 0xff) {
				printf("found\n");
				show_map(news);
				free(news);
				return 0;
			}

			if (fill_and_check(news) == 0) {
				free(news);
				return 0;
			}
			
			free(news);
		}
	}

	return -1;
}

static int solve(int month, int day)
{
	map_st *start = malloc(sizeof(map_st));
	memcpy(start, &origin_map, sizeof(map_st));

	month--;
	day--;

	start->m[3 + month / 6][3 + month % 6] = 17;
	start->m[5 + day / 7][3 + day % 7] = 17;

	int res = fill_and_check(start);
	free(start);
	return res;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Usage: %s <month(1-12)> <day(1-31)>\n", argv[0]);
		return 1;
	}

	init_blocks();

	int month = atoi(argv[1]);
	int day = atoi(argv[2]);

	if (month < 1 || month > 12) {
		printf("Invalid month: %s\n", argv[1]);
		return 1;
	}

	if (day < 1 || day > 31) {
		printf("Invalid day: %s\n", argv[2]);
		return 1;
	}

	return solve(month, day);
}
