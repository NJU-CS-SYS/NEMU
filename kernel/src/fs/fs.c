#include "common.h"
#include "string.h"

typedef struct
{
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

typedef struct
{
	bool opened;
	uint32_t offset;
} FileState;

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

FileState file_state[NR_FILES];

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);
int print(void *buf, int len);
#define BUF_LEN 4096
/* This array stores the solid file info for game pal
 * reserving the front 3 pos for stdin, stdout, stderr
 */
int fs_open(const char *pathname, int flags)
{
	int fd; // file descriptor, ignore stdin, stdout, and stderr
	for (fd = 0; fd < NR_FILES; fd ++) {
		if ( !strcmp(pathname, file_table[fd].name) ) {
			file_state[fd].opened = true;
			file_state[fd].offset = 0;
			return fd + 3;
		}
	}
	assert(0);
	return -1;
}
/* read return 0 to indicate that it has reached the end of file
 * but how can read to detect the end of file?
 * The first time its return value is less than the count
 * parameter, the next time its return value is zero.
 */
int fs_read(int fd, void *buf, int len)
{
	
	fd -= 3; // for stdin, stdout, and stderr

	nemu_assert(fd < NR_FILES);

	nemu_assert(file_state[fd].opened);

	uint8_t temp;
	ide_read(&temp, file_table[fd].disk_offset, 1);

	if (len == 0)
		return 0;

	if (file_state[fd].offset + len <= file_table[fd].size) {
		ide_read(
				buf,
				file_table[fd].disk_offset + file_state[fd].offset,
				len);
		file_state[fd].offset += len;
		return len;
	}
	
	if (file_state[fd].offset >= file_table[fd].size)
		return 0;

	len = file_table[fd].size - file_state[fd].offset;
	ide_read(
			buf,
			file_table[fd].disk_offset + file_state[fd].offset,
			len);
	file_state[fd].offset += len;
	Log("file %s offset %x, buf %x", file_table[fd].name, file_state[fd].offset, *(uint32_t *)buf);
	return len;
}
int fs_write(int fd, void *buf, int len)
{
	if (fd == 1 || fd == 2) { // stdout or stderr
		return print(buf, len);
	}

	fd -= 3; // for stdin, stdout, and stderr

	nemu_assert(fd < NR_FILES);

	nemu_assert(file_state[fd].opened);

	if (len == 0)
		return 0;

	if (file_state[fd].offset + len <= file_table[fd].size) {
		ide_write(
				buf,
				file_table[fd].disk_offset + file_state[fd].offset,
				len);
		file_state[fd].offset += len;
		return len;
	}
	
	if (file_state[fd].offset >= file_table[fd].size)
		return 0;

	len = file_table[fd].size - file_state[fd].offset;
	ide_write(
			buf,
			file_table[fd].disk_offset + file_state[fd].offset,
			len);
	file_state[fd].offset += len;
	return len;
}
int fs_lseek(int fd, int offset, int whence)
{
	fd -= 3;

	nemu_assert(fd >= 0 && fd < NR_FILES);
	nemu_assert(file_state[fd].opened);
	switch (whence)	{
		case SEEK_SET:
			Log("SEEK offset %x", offset);
			file_state[fd].offset = offset;
			return offset;
		case SEEK_CUR:
			file_state[fd].offset += offset;
			return file_state[fd].offset;
		case SEEK_END:
			//Log("unclear SEEK_END");
			file_state[fd].offset = offset + file_table[fd].size;
			return file_state[fd].offset;
	}
	assert(0);
	return 0;
}
int fs_close(int fd)
{
	fd -= 3;
	file_state[fd].opened = false;
	file_state[fd].offset = 0;
	return 0;
}
