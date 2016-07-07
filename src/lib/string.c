#include  <stdio.h>

int strlen(const char *s){
	int n;

	for (n = 0; *s != '\0'; s++)
		n++;
	return n;
}

int
strnlen(const char *s, size_t size)
{
	int n;

	for (n = 0; size > 0 && *s != '\0'; s++, size--)
		n++;
	return n;
}

char *strcpy(char *dst, const char *src)
{
	char *ret;

	ret = dst;
	while ((*dst++ = *src++) != '\0')
		/* do nothing */;
	return ret;
}

size_t strlcpy(char *dst, const char *src, size_t size){
	char *dst_in;

	dst_in = dst;
	if (size > 0) {
		while (--size > 0 && *src != '\0')
			*dst++ = *src++;
		*dst = '\0';
	}
	return dst - dst_in;
}

int strcmp(const char *p, const char *q){
	while (*p && *p == *q)
		p++, q++;
	return (int) ((unsigned char) *p - (unsigned char) *q);
}

void *memset(void *v, int c, size_t n){
	char *p;
	int m;

	p = v;
	m = n;
	while (--m >= 0)
		*p++ = c;

	return v;
}

void *memmove(void *dst, const void *src, size_t n){
	const char *s;
	char *d;

	s = src;
	d = dst;
	if (s < d && s + n > d) {
		s += n;
		d += n;
		while (n-- > 0)
			*--d = *--s;
	} else
		while (n-- > 0)
			*d++ = *s++;

	return dst;
}

void *memcpy(void *dst, const void *src, size_t n){
	return memmove(dst, src, n);
}

int atoi(const char *nptr){
	int num=0;
	int i=1;
	if(nptr[0]!='-')num=nptr[0]-'0';
	while(nptr[i]!='\0')num=num*10+(nptr[i++]-'0'); //字符串未到尾，进行数的转换
	if(nptr[0]=='-')return (-num);
	else return num;
}

void *strtok(char *string_org, const char* demial){
	static unsigned char *last; //save remain
	unsigned char* str; //ret str
	const unsigned char* ctrl = (const unsigned char*)demial;
	
	//index table
	unsigned char map[32];
	int count;
	for (count =0; count <32; count++){
		map[count] = 0;
	}

	do{
		map[*ctrl >> 3] |= (1 <<(*ctrl&7));
	}while(*ctrl++);

	if (string_org){
		str=(unsigned char *)string_org;
	}
	else{
		str = last;
	}

	while((map[*str >> 3] & (1 << (*str & 7))) && *str){
		str++;
	}

	string_org = (char*)str;

	for (;*str; str++){
		if (map[*str >> 3] & (1 << (*str & 7))){
			*str++ = '\0';
			break;
		}
	}

	last = str;
	if (string_org == (char *)str){
		return NULL;
	}
	else {
		return string_org;
	}
}
