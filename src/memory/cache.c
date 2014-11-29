#include "common.h"
#include "lib/misc.h"
#include "cpu/reg.h"

/* Simulate the (main) behavor of CACHE. Although this will lower the performace of NEMU,
 * it makes you clear about how CACHE is read/written.
 */

struct _cache_ {
	int ***data;
	int nr_set;
	int nr_way;
	int nr_block;
	int **valid;
	struct _cache_ *next;
};
typedef struct _cache_ cache;

bool create_cache(cache **ppcache, int x, int y, int z) {
	(*ppcache) = (cache*)malloc(sizeof(cache));
	return 0;
}
