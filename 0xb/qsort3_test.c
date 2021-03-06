/* SPDX-License-Identifier: GPL-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "qsort3.h"

static void swap(int x[], int a, int b)
{
	int tmp = x[a];
	x[a] = x[b];
	x[b] = tmp;
}

static void init_array(int x[], int nr, int v)
{
	int i;

	for (i = 0; i < nr; i++) {
		if (v)
			x[i] = v;
		else
			x[i] = i;
	}
}

static void generate_sorted_array(int x[], int nr)
{
	init_array(x, nr, 0);
}

static void generate_identical_array(int x[], int nr)
{
	init_array(x, nr, 1);
}

static void generate_random_array(int x[], int nr)
{
	int i;

	init_array(x, nr, 0);
	srand(time(0));
	for (i = 0; i < nr/2; i++) {
		int j = rand()%nr;
		swap(x, i, j);
	}
}

static void dump_array(const int x[], int nr)
{
	int i;

	for (i = 0; i < nr; i++) {
		if (!(i&0x7))
			printf("\n"); /* every eight entries */
		printf("%7d ", x[i]);
	}
}

static bool is_sorted(const int x[], int nr)
{
	int i;
	for (i = 1; i < nr; i++)
		if (x[i-1] > x[i])
			return false;
	return true;
}

static int test_qsort3(int *i, const char *func_name,
		       void (*generator)(int x[], int nr))
{
	const struct test {
		const char	*name;
		unsigned int	nr;
	} tests[] = {
		{
			.name	= "zero entry",
			.nr	= 0,
		},
		{
			.name	= "one entry",
			.nr	= 1,
		},
		{
			.name	= "two entries",
			.nr	= 2,
		},
		{
			.name	= "three entries",
			.nr	= 3,
		},
		{
			.name	= "four entries",
			.nr	= 4,
		},
		{
			.name	= "eight entries",
			.nr	= 8,
		},
		{
			.name	= "16 entries",
			.nr	= 16,
		},
		{
			.name	= "32 entries",
			.nr	= 32,
		},
		{
			.name	= "1024 entries",
			.nr	= 1024,
		},
		{
			.name	= "2048 entries",
			.nr	= 2048,
		},
		{
			.name	= "4096 entries",
			.nr	= 4096,
		},
		{
			.name	= "8192 entries",
			.nr	= 8192,
		},
		{
			.name	= "16384 entries",
			.nr	= 16384,
		},
		{
			.name	= "32768 entries",
			.nr	= 32768,
		},
		{
			.name	= "65536 entries",
			.nr	= 65536,
		},
		{
			.name	= "131072 entries",
			.nr	= 131072,
		},
		{ /* sentry */ },
	};
	const struct test *t;
	int fail = 0;

	for (t = tests; t->name; t++) {
#define MAX_NR_ENTRIES 1<<20
		int x[MAX_NR_ENTRIES];
#undef MAX_NR_ENTRIES
		double diff;
		clock_t c;

		printf("%3d) %-21s: %-35s", ++(*i), func_name, t->name);

		generator(x, t->nr);

		c = clock();
		qsort3(x, t->nr);
		c = clock() - c;
		diff = ((double)c/CLOCKS_PER_SEC);
		if (!is_sorted(x, t->nr)) {
			printf("FAIL: %9.6fsec\n", diff);
			fail++;
			dump_array(x, t->nr);
		} else
			printf("PASS: %9.6fsec\n", diff);
	}
	return fail;
}

int test_qsort3_random(int *i)
{
	return test_qsort3(i, __FUNCTION__, generate_random_array);
}

int test_qsort3_identical(int *i)
{
	return test_qsort3(i, __FUNCTION__, generate_identical_array);
}

int test_qsort3_sorted(int *i)
{
	return test_qsort3(i, __FUNCTION__, generate_sorted_array);
}
