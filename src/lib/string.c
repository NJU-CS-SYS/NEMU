#include <common.h>

size_t strlen(const char *s){
    size_t n;

    for (n = 0; *s != '\0'; s++)
        n++;
    return n;
}

size_t strnlen(const char *s, size_t size)
{
    size_t n;

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

char *strtok(char *string_org, const char* demial){
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

#define LONG_MAX 2147483647L
#define LONG_MIN (-2147483647L-1L)
long strtol ( char *  nptr, char  **  endptr, int  base   )  {
    const char *s = nptr;
    unsigned long acc;
    unsigned char c;
    unsigned long cutoff;
    int neg = 0,any, cutlim;
    //判断正负号
    do{ c = *s++;
    } while (c==' ');
    if (c == '-')
    {
        neg = 1;
        c = *s++;
    }
    else if (c == '+')
        c = *s++;
    //判断进制数
    if ((base == 0 || base == 16) &&
            c == '0' && (*s == 'x' || *s == 'X'))
    {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;

    //溢出处理
    cutoff = neg ? -(unsigned long) LONG_MIN : LONG_MAX;
    cutlim = cutoff % (unsigned long) base;
    cutoff /= (unsigned long) base;
    for (acc = 0, any = 0;; c = *s++)
    {
        if (c>='0'&&c<='9')
            c -= '0';
        else if ((c>='A'&&c<='Z')||(c>='a'&&c<='z'))
            c -= (c>='A'&&c<='Z') ? 'A' - 10 : 'a' - 10;
        else
            break;
        if ((int) c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && (int) c > cutlim))
            any = -1;
        else
        {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    //超过范围则输出range error
    if (any < 0)
    {
        // acc = neg ? LONG_MIN : LONG_MAX;
        //errno = ERANGE;
        printf("range error.\n");
    }
    else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = any ?(char *) ( s - 1) : (char *) nptr;
    return acc;
}
