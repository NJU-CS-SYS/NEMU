#include "exec/helper.h"

#define DIR l
#define DIRTAG 0
#define SIGN a
#define SIGNTAG 1
#define DATA_BYTE 1
#include "shift-template.h"
#undef DATA_BYTE
#define DATA_BYTE 2
#include "shift-template.h"
#undef DATA_BYTE
#define DATA_BYTE 4
#include "shift-template.h"
#undef DATA_BYTE
#undef DIR
#undef DIRTAG
#undef SIGN
#undef SIGNTAG

#define DIR l
#define DIRTAG 0
#define SIGN h
#define SIGNTAG 0
#define DATA_BYTE 1
#include "shift-template.h"
#undef DATA_BYTE
#define DATA_BYTE 2
#include "shift-template.h"
#undef DATA_BYTE
#define DATA_BYTE 4
#include "shift-template.h"
#undef DATA_BYTE
#undef DIR
#undef DIRTAG
#undef SIGN
#undef SIGNTAG

#define DIR r
#define DIRTAG 1
#define SIGN a
#define SIGNTAG 1
#define DATA_BYTE 1
#include "shift-template.h"
#undef DATA_BYTE
#define DATA_BYTE 2
#include "shift-template.h"
#undef DATA_BYTE
#define DATA_BYTE 4
#include "shift-template.h"
#undef DATA_BYTE
#undef DIR
#undef DIRTAG
#undef SIGN
#undef SIGNTAG

#define DIR r
#define DIRTAG 1
#define SIGN h
#define SIGNTAG 0
#define DATA_BYTE 1
#include "shift-template.h"
#undef DATA_BYTE
#define DATA_BYTE 2
#include "shift-template.h"
#undef DATA_BYTE
#define DATA_BYTE 4
#include "shift-template.h"
#undef DATA_BYTE
#undef DIR
#undef DIRTAG
#undef SIGN
#undef SIGNTAG


extern char suffix;
v_helper(sal_i8);
v_helper(sar_i8);
v_helper(sal_12rm);
v_helper(sar_12rm);
