unsigned char loader[] = {
  0xbd, 0x00, 0x00, 0x00, 0x00, 0xbc, 0x00, 0x00, 0x00, 0x08, 0xe9, 0x01,
  0x00, 0x00, 0x00, 0x90, 0x83, 0xec, 0x0c, 0xe8, 0x18, 0x00, 0x00, 0x00,
  0xb8, 0x01, 0x00, 0x00, 0x00, 0x82, 0x83, 0xc4, 0x0c, 0xc3, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x55, 0x57, 0x31, 0xff, 0x56, 0x53, 0x83, 0xec, 0x1c, 0x0f, 0xb7, 0x05,
  0x2c, 0x00, 0x00, 0x00, 0x8b, 0x15, 0x1c, 0x00, 0x00, 0x00, 0x66, 0x89,
  0x44, 0x24, 0x0e, 0x0f, 0xb7, 0x05, 0x2a, 0x00, 0x00, 0x00, 0xc1, 0xe0,
  0x05, 0x66, 0x83, 0x7c, 0x24, 0x0e, 0x00, 0x89, 0xc5, 0x75, 0x17, 0xeb,
  0x77, 0x8d, 0xb4, 0x26, 0x00, 0x00, 0x00, 0x00, 0x01, 0xea, 0x83, 0xc7,
  0x01, 0x66, 0x39, 0x3d, 0x2c, 0x00, 0x00, 0x00, 0x76, 0x5a, 0x83, 0x3a,
  0x01, 0x75, 0xed, 0x8b, 0x42, 0x14, 0x0f, 0xb7, 0x5a, 0x10, 0x8b, 0x4a,
  0x08, 0x8b, 0x72, 0x04, 0x89, 0x44, 0x24, 0x08, 0x31, 0xc0, 0x85, 0xdb,
  0x74, 0x18, 0x89, 0x54, 0x24, 0x04, 0x66, 0x90, 0x0f, 0xb6, 0x14, 0x30,
  0x88, 0x14, 0x08, 0x83, 0xc0, 0x01, 0x39, 0xd8, 0x75, 0xf2, 0x8b, 0x54,
  0x24, 0x04, 0x0f, 0xb7, 0x5c, 0x24, 0x08, 0x39, 0xd8, 0x7d, 0xb5, 0x01,
  0xc8, 0x01, 0xd9, 0x90, 0xc6, 0x00, 0x00, 0x83, 0xc0, 0x01, 0x39, 0xc8,
  0x75, 0xf6, 0x01, 0xea, 0x83, 0xc7, 0x01, 0x66, 0x39, 0x3d, 0x2c, 0x00,
  0x00, 0x00, 0x77, 0xa6, 0x66, 0x83, 0x7c, 0x24, 0x0e, 0x03, 0x74, 0x06,
  0xb8, 0x01, 0x00, 0x00, 0x00, 0x82, 0xff, 0x15, 0x18, 0x00, 0x00, 0x00,
  0x31, 0xc0, 0x82, 0x83, 0xc4, 0x1c, 0x5b, 0x5e, 0x5f, 0x5d, 0xc3, 0x00,
  0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7a, 0x52, 0x00,
  0x01, 0x7c, 0x08, 0x01, 0x1b, 0x0c, 0x04, 0x04, 0x88, 0x01, 0x00, 0x00,
  0x14, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
  0x12, 0x00, 0x00, 0x00, 0x00, 0x43, 0x0e, 0x10, 0x4e, 0x0e, 0x04, 0x00,
  0x38, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x08, 0xff, 0xff, 0xff,
  0xbf, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x08, 0x85, 0x02, 0x41, 0x0e,
  0x0c, 0x87, 0x03, 0x43, 0x0e, 0x10, 0x86, 0x04, 0x41, 0x0e, 0x14, 0x83,
  0x05, 0x43, 0x0e, 0x30, 0x02, 0xb1, 0x0e, 0x14, 0x41, 0xc3, 0x0e, 0x10,
  0x41, 0xc6, 0x0e, 0x0c, 0x41, 0xc7, 0x0e, 0x08, 0x41, 0xc5, 0x0e, 0x04
};
unsigned int loader_len = 348;
