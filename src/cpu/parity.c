bool parity(uint32_t val) {
	val = 0xff & val;
	int c = 0;
	for (; val != 0; val = val >> 1)
		if (val & 0x1) c++;
	return !(c & 0x1);
}
