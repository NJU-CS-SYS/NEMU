#define RAND_MAX 0x7FFF

unsigned int rand(){
	static int ret = 123;
	ret=ret*3 & RAND_MAX;
	return ret;
}

