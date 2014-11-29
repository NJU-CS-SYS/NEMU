#include "common.h"
#include "lib/misc.h"
#include "cpu/reg.h"

/* Simulate the (main) behavor of CACHE. Although this will lower the performace of NEMU,
 * it makes you clear about how CACHE is read/written.
 */

struct _cache_ {
	uint8_t ***data;
	int nr_set;
	int nr_way;
	int nr_block;
	uint8_t **valid;
	struct _cache_ *next;
};
typedef struct _cache_ cache;

bool create_cache(cache **ppcache, int x, int y, int z) {
	(*ppcache) = (cache*)malloc(sizeof(cache));
	cache* pcache = *ppcache;
	pcache->nr_set = x;
	pcache->nr_way = y;
	pcache->nr_block = z;
	pcache->next = NULL;

	// allocate the mem for data
	int i, j;
	pcache->data = (uint8_t***)malloc(sizeof(uint8_t**) * x);
	for (i = 0; i < x; i ++) {
		pcache->data[i] = (uint8_t**)malloc(sizeof(uint8_t*) * y);
		for (j = 0; j < y; j ++) {
			pcache->data[i][j] = (uint8_t*)malloc(sizeof(uint8_t) * z);
		}
	}

	// allocate the mem for valid
	return 0;
}
