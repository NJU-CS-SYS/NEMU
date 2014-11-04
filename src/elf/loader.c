unsigned char loader[] = {
  0x55, 0x89, 0xe5, 0x83, 0xec, 0x18, 0x8b, 0x15, 0x80, 0x01, 0x10, 0x00,
  0xa1, 0x80, 0x01, 0x10, 0x00, 0x89, 0x54, 0x24, 0x04, 0x89, 0x04, 0x24,
  0xe8, 0xb7, 0x00, 0x00, 0x00, 0x8b, 0x15, 0xa0, 0x01, 0x10, 0x00, 0x39,
  0xd0, 0x74, 0x06, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x82, 0x8b, 0x15, 0x84,
  0x01, 0x10, 0x00, 0xa1, 0x80, 0x01, 0x10, 0x00, 0x89, 0x54, 0x24, 0x04,
  0x89, 0x04, 0x24, 0xe8, 0x90, 0x00, 0x00, 0x00, 0x8b, 0x15, 0xa4, 0x01,
  0x10, 0x00, 0x39, 0xd0, 0x74, 0x06, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x82,
  0xc7, 0x45, 0xf4, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x45, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0xeb, 0x56, 0xc7, 0x45, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xeb,
  0x41, 0x8b, 0x45, 0xf8, 0x8b, 0x14, 0x85, 0x80, 0x01, 0x10, 0x00, 0x8b,
  0x45, 0xfc, 0x8b, 0x04, 0x85, 0x80, 0x01, 0x10, 0x00, 0x89, 0x54, 0x24,
  0x04, 0x89, 0x04, 0x24, 0xe8, 0x47, 0x00, 0x00, 0x00, 0x8b, 0x55, 0xf4,
  0x8b, 0x14, 0x95, 0xa0, 0x01, 0x10, 0x00, 0x39, 0xd0, 0x0f, 0x95, 0xc0,
  0x83, 0x45, 0xf4, 0x01, 0x84, 0xc0, 0x74, 0x06, 0xb8, 0x01, 0x00, 0x00,
  0x00, 0x82, 0x83, 0x45, 0xf8, 0x01, 0x8b, 0x45, 0xf8, 0x83, 0xf8, 0x07,
  0x76, 0xb7, 0x83, 0x45, 0xfc, 0x01, 0x8b, 0x45, 0xfc, 0x83, 0xf8, 0x07,
  0x76, 0xa2, 0xe8, 0x23, 0x00, 0x00, 0x00, 0xb8, 0x00, 0x00, 0x00, 0x00,
  0x82, 0xb8, 0x00, 0x00, 0x00, 0x00, 0xc9, 0xc3, 0x55, 0x89, 0xe5, 0x83,
  0xec, 0x10, 0x8b, 0x45, 0x0c, 0x8b, 0x55, 0x08, 0x01, 0xd0, 0x89, 0x45,
  0xfc, 0x8b, 0x45, 0xfc, 0xc9, 0xc3, 0x55, 0x89, 0xe5, 0xb8, 0x0c, 0x00,
  0x00, 0x00, 0x5d, 0xc3, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x7a, 0x52, 0x00, 0x01, 0x7c, 0x08, 0x01, 0x1b, 0x0c, 0x04, 0x04,
  0x88, 0x01, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0xc0, 0xff, 0xff, 0xff, 0x16, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x08,
  0x85, 0x02, 0x42, 0x0d, 0x05, 0x52, 0xc5, 0x0c, 0x04, 0x04, 0x00, 0x00,
  0x1c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0xb6, 0xff, 0xff, 0xff,
  0x0a, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x08, 0x85, 0x02, 0x42, 0x0d,
  0x05, 0x46, 0xc5, 0x0c, 0x04, 0x04, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0x5c, 0x00, 0x00, 0x00, 0xac, 0xfe, 0xff, 0xff, 0xd4, 0x00, 0x00, 0x00,
  0x00, 0x41, 0x0e, 0x08, 0x85, 0x02, 0x42, 0x0d, 0x05, 0x02, 0xd0, 0xc5,
  0x0c, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80,
  0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x7f,
  0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0xfe, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80,
  0x02, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x7f,
  0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0xfe, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xff, 0xff, 0x7f,
  0xfe, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80,
  0x02, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f,
  0x01, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x80, 0xfe, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xff, 0xff, 0x7f,
  0xfe, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xfc, 0xff, 0xff, 0xff,
  0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f,
  0x00, 0x00, 0x00, 0x80, 0xfd, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff
};
unsigned int loader_len = 672;
