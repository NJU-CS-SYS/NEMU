#include "common.h"
#include "string.h"

typedef struct
{
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] =
{
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);
int print(void *buf, int len);
#define BUF_LEN 4096
/* This array stores the solid file info for game pal
 * reserving the front 3 pos for stdin, stdout, stderr
 */

/* TODO: implement a simplified file system here. */
int fs_open(const char *pathname, int flags)
{
	int fd; // file descriptor, ignore stdin, stdout, and stderr
	for (fd = 0; fd < NR_FILES; fd ++) {
		if ( !strcmp(pathname, file_table[fd].name) ) {
			return fd;
		}
	}
	
	/* Don't find the file, which is impossible
	 * and I don't allocate new file descriptor
	 * for the non-existed file
	 */
	return -1;
}
int fs_read(int fd, void *buf, int len)
{
	Log("In fs_read len %x", len);

	fd -= 3; // for stdin, stdout, and stderr

	nemu_assert(fd < NR_FILES);

	len = len < file_table[fd].size ? len : file_table[fd].size;
	ide_read(buf, file_table[fd].disk_offset, len);
	return len;
}
int fs_write(int fd, void *buf, int len)
{
	if (fd == 1 || fd == 2) { // stdout or stderr
		return print(buf, len);
	}

	fd -= 3; // for stdin, stdout, and stderr
	nemu_assert(fd < NR_FILES);
	len = len < file_table[fd].size ? len : file_table[fd].size;
	ide_write(buf, file_table[fd].disk_offset, len);
	return len;
}
int fs_lseek(int fd, int offset, int whence);
int fs_close(int fd)
{
	return 0;
}
