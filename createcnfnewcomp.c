#include <stdio.h>
#include <stdlib.h>

#include "eter172c.c"

#if ENABLE_ETERNITY2 == 1
#include "e2_info.c"
#endif // ENABLE_ETERNITY2 == 1

#if ENABLE_TESTBOARD_10X10 == 1
#include "10x10_info.c"
#endif // ENABLE_TESTBOARD_10X10 == 1

#if ENABLE_TESTBOARD_6X6 == 1
#include "6x6_info.c"
#endif // ENABLE_TESTBOARD_6X6 == 1

#if ENABLE_ETERNITY2 == 1
#if ETERNITY_FULL_FIELD 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if ETERNITY_FULL_FIELD_WITH_HOLES
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if ETERNITY_FULL_FIELD_WITH_HOLES_228
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if ETERNITY_FULL_FIELD_WITH_HOLES_232
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXX.X.X.XXXXXX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if ETERNITY_FULL_FIELD_WITH_HOLES_235
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if ETERNITY_FULL_FIELD_WITH_HOLES_239
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXX.X.X.XXXXXX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if ETERNITY_FULL_FIELD_WITH_HOLES_242
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X.X.X.X.XX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0
// 251 fields filled
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXX.X.X.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0
// 241 fields filled
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"X.X.X.X..X.X.X.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#ifdef E2_PARTIAL_FIELD_235_FIELDS_FILLED 
// 235 fields filled
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"X........X.X.X.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0 
#ifdef E2_PARTIAL_FIELD_233_FIELDS_FILLED 
// 233 fields filled
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"X..........X.X.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#ifdef E2_PARTIAL_FIELD_231_FIELDS_FILLED 
// 231 fields filled
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
"X............X.X"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#ifdef E2_PARTIAL_FIELD_229_FIELDS_FILLED
// 229 fields filled
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"X..............X"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXX.XXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0
unsigned char enabled_fields[] =
"...........XXXXX"
"...........XXXXX"
"...........XXXXX"
"...........XXXXX"
"...........XXXXX"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
;
#endif // 0
#if 0
// 12*12+3=144+3=147 fields filled
unsigned char enabled_fields[] =
"................"
"................"
"..X..........X.."
"................"
"XXXXXXXXXXXX...."
"XXXXXXXXXXXX...."
"XXXXXXXXXXXX...."
"XXXXXXXXXXXX...."
"XXXXXXXXXXXX...."
"XXXXXXXXXXXX...."
"XXXXXXXXXXXX...."
"XXXXXXXXXXXX...."
"XXXXXXXXXXXX...."
"XXXXXXXXXXXX.X.."
"XXXXXXXXXXXX...."
"XXXXXXXXXXXX...."
;
#endif // 0
#if 0
// 10*10+3=100+3=103 fields filled
unsigned char enabled_fields[] =
"................"
"................"
"..X..........X.."
"................"
"................"
"................"
"XXXXXXXXXX......"
"XXXXXXXXXX......"
"XXXXXXXXXX......"
"XXXXXXXXXX......"
"XXXXXXXXXX......"
"XXXXXXXXXX......"
"XXXXXXXXXX......"
"XXXXXXXXXX...X.."
"XXXXXXXXXX......"
"XXXXXXXXXX......"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"................"
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
".XXXXXXXXXXXXXX."
"................"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"................"
"................"
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"..XXXXXXXXXXXX.."
"................"
"................"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"X..............X"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XX............XX"
"XX............XX"
"XX............XX"
"XX............XX"
"XX............XX"
"XX............XX"
"XX............XX"
"XX............XX"
"XX............XX"
"XX............XX"
"XX............XX"
"XX............XX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXX..........XXX"
"XXX..........XXX"
"XXX..........XXX"
"XXX..........XXX"
"XXX..........XXX"
"XXX..........XXX"
"XXX..........XXX"
"XXX..........XXX"
"XXX..........XXX"
"XXX..........XXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXX........XXXX"
"XXXX........XXXX"
"XXXX........XXXX"
"XXXX........XXXX"
"XXXX........XXXX"
"XXXX........XXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXX......XXXXX"
"XXXXX......XXXXX"
"XXXXX......XXXXX"
"XXXXX......XXXXX"
"XXXXX......XXXXX"
"XXXXX......XXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXX....XXXXXX"
"XXXXXX....XXXXXX"
"XXXXXX....XXXXXX"
"XXXXXX....XXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXX..XXXXXXX"
"XXXXXXX..XXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"X.X.X.X.X.X.X.X."
".X.X.X.X.X.X.X.X"
"X.X.X.X.X.X.X.X."
".X.X.X.X.X.X.X.X"
"X.X.X.X.X.X.X.X."
".X.X.X.X.X.X.X.X"
"X.X.X.X.X.X.X.X."
".X.X.X.X.X.X.X.X"
"X.X.X.X.X.X.X.X."
".X.X.X.X.X.X.X.X"
"X.X.X.X.X.X.X.X."
".X.X.X.X.X.X.X.X"
"X.X.X.X.X.X.X.X."
".X.X.X.X.X.X.X.X"
"X.X.X.X.X.X.X.X."
".X.X.X.X.X.X.X.X"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"X.XXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXX.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXX........XXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXXXXXXXX"
"XXXXX......XXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"X.XXXXXXXXXXXX.X"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXXXXXXXXXXXXX"
"XXXXX......XXXXX"
"XXXXXXXXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXX..........."
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
;
#endif // 0
#ifdef ENABLE_TOP_LEFT_3X3  
unsigned char enabled_fields[] =
"XXX............."
"XXX............."
"XXX............."
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
;
#endif // 0
#ifdef ENABLE_TOP_RIGHT_3X3 
unsigned char enabled_fields[] =
".............XXX"
".............XXX"
".............XXX"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
;
#endif // 0
#ifdef ENABLE_BOTTOM_LEFT_3X3 
unsigned char enabled_fields[] =
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"XXX............."
"XXX............."
"XXX............."
;
#endif // 0
#ifdef ENABLE_BOTTOM_RIGHT_3X3 
unsigned char enabled_fields[] =
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
".............XXX"
".............XXX"
".............XXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
".............XX."
".............XXX"
"..............XX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXX............"
"XXXX............"
"XXXX............"
"XXXX............"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XX.............."
"XX.............."
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"................"
"................"
"..XX............"
"..XX............"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"................"
"................"
"XX.............."
"XX.............."
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"..XX............"
"..XX............"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXX..........XXX"
"XXX..........XXX"
"XXX..........XXX"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
"................"
;
#endif // 0
#endif // ENABLE_ETERNITY2 == 1

#if ENABLE_TESTBOARD_10X10 == 1
#if 1 
unsigned char enabled_fields[] =
"XXXXXXXXXX" 
"XXXXXXXXXX"
"XXXXXXXXXX"
"XXXXXXXXXX"
"XXXXXXXXXX"
"XXXXXXXXXX" 
"XXXXXXXXXX"
"XXXXXXXXXX"
"XXXXXXXXXX"
"XXXXXXXXXX"
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XX........"
".........."
".........."
".........."
".........."
".........."
".........."
".........."
".........."
".........."
;
#endif // 0
#if 0 
unsigned char enabled_fields[] =
"XXXXXXXXXX" 
"X.X.XX.X.X"
"XXXXXXXXXX"
"XXXXXXXXXX"
"XXXXXXXXXX"
"XXXXXXXXXX" 
"XXXXXXXXXX"
"XXXXXXXXXX"
"XXXXXXXXXX"
"XXXXXXXXXX"
;
#endif // 0
#endif // ENABLE_TESTBOARD_10X10 == 1

#if ENABLE_TESTBOARD_6X6 == 1
unsigned char enabled_fields[] =
"XXXXXX" 
"XXXXXX"
"XXXXXX"
"XXXXXX"
"XXXXXX"
"XXXXXX";
#endif // ENABLE_TESTBOARD_6X6 == 1

unsigned long fieldMin[256];
unsigned long fieldMax[256];

unsigned long usedPieces[NR_OF_FIELDS][NR_OF_CARDS][NR_OF_ROTATIONS];

unsigned long cardsPerField[NR_OF_FIELDS][NR_OF_CARDS];
unsigned long nrOfCardsPerField[NR_OF_FIELDS];

unsigned char IsFieldEnabled(int Field)
{
    if(Field < 0) 
    {
        return 0;
    }
    if(Field >= NR_OF_FIELDS)
    {
        return 0;
    }
    if(enabled_fields[Field] != 'X')
    {
        return 0;
    }
    return 1;
}


int main()
{
    int i;
    int j;
    
    if(sizeof(enabled_fields) != NR_OF_FIELDS + 1)
    {
        printf("sizeof(enabled_fields)=%ld != (%d+1)\n", sizeof(enabled_fields), NR_OF_FIELDS);
        exit(1);
    }

    for(int i = 0; i < NR_OF_FIELDS; i++)
    {
        fieldMin[i] = NR_OF_SAT_VARIABLES + 1;
        fieldMax[i] = 1 - 1;
    }

    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        if(i < fieldMin[combinations[i-1].Field]) fieldMin[combinations[i-1].Field] = i;
        if(fieldMax[combinations[i-1].Field] < i) fieldMax[combinations[i-1].Field] = i;
    }

    for(int i = 0; i < NR_OF_FIELDS; i++)
    {
        printf("c Field %d: min=%ld max=%ld (diff=%ld)\n", i, fieldMin[i], fieldMax[i], fieldMax[i] - fieldMin[i] + 1);
    }

    for(int i = 0; i < NR_OF_FIELDS; i++)
    {
        for(int j = 0; j < NR_OF_CARDS; j++)
        {
            for(int r = 0; r < NR_OF_ROTATIONS; r++)
            {
                usedPieces[i][j][r] = 0;
	    }
	}
        nrOfCardsPerField[i] = 0;
    }

    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        usedPieces[combinations[i-1].Field][combinations[i-1].Card][combinations[i-1].Rotation] = combinations[i-1].Index;
	if(combinations[i-1].Field == 0 || combinations[i-1].Field == 1)
        {
            printf("c F%d C%d R%d\n", combinations[i-1].Field, combinations[i-1].Card, combinations[i-1].Rotation);
        }
	if(nrOfCardsPerField[combinations[i-1].Field] > 0)
	{
	    if(cardsPerField[combinations[i-1].Field][nrOfCardsPerField[combinations[i-1].Field]-1] != combinations[i-1].Card)
            {
	        cardsPerField[combinations[i-1].Field][nrOfCardsPerField[combinations[i-1].Field]] = combinations[i-1].Card;
                nrOfCardsPerField[combinations[i-1].Field]++;
	    }
	}
	else
        {
            cardsPerField[combinations[i-1].Field][nrOfCardsPerField[combinations[i-1].Field]] = combinations[i-1].Card;
            nrOfCardsPerField[combinations[i-1].Field]++;
	}
    }

    for(int i = 0; i < nrOfCardsPerField[0]; i++)
    {
        printf("c Entry %d: C%ld\n", i, cardsPerField[0][i]);
    }

    for(int i = 0; i < nrOfCardsPerField[1]; i++)
    {
        printf("c Entry %d: C%ld\n", i, cardsPerField[1][i]);
    }

    printf("c Step 1\n");
    
    for(i = 0; i < NR_OF_FIELDS; i++)
    {
        if(!IsFieldEnabled(i))
        {
            for(j = i * 10; j < (i+1)*10; j++)
            {
                printf("-%d 0\n", 1 + j);
            }
        }
        else
        {
            // 256 is max sub index on a given field
            for(int v = 0; v < 256; v++)
            {
		printf("c v=%d\n", v);
                if(v == 0 || v > (nrOfCardsPerField[i]))
                {
                    printf("c F%d not allowed value %d\n", i, v);
                    for(int k = 0; k < 8; k++)
                    {
                        printf("%d ", (v & (1 << k)) == 0 ? 1 + 2 + i * 10 + k : -(1 + 2 + i * 10 + k));
                    }
                    printf("0\n");
                }
		else
                {
                    for(int r = 0; r < NR_OF_ROTATIONS; r++)
                    {
		        if(usedPieces[i][cardsPerField[i][v-1]][r] == 0)
			{
		            for(int k = 0; k < 2; k++)
                            {
                                printf("%d ", (r & (1 << k)) == 0 ? 1 + i * 10 + k : -(1 + i * 10 + k));
                            }
                            for(int k = 0; k < 8; k++)
                            {
                                printf("%d ", (v & (1 << k)) == 0 ? 1 + 2 + i * 10 + k : -(1 + 2 + i * 10 + k));
                            }
                            printf("0\n");
	 	        }
		    }
		}
            }
        } 
    }

    printf("c Step 2\n");

    // No cards on two fields
    for(i = 0; i < NR_OF_SAT_VARIABLES; i++)
    {
        for(j = i + 1; j < NR_OF_SAT_VARIABLES; j++)
        {
            if(combinations[i].Card == combinations[j].Card && combinations[i].Field != combinations[j].Field && IsFieldEnabled(combinations[i].Field) && IsFieldEnabled(combinations[j].Field))
            {
                //TODOTODOTODO
		int vi;
		for(vi = 0; vi < nrOfCardsPerField[combinations[i].Field]; vi++)
		{
		    if(cardsPerField[combinations[i].Field][vi] == combinations[i].Card) break; 
		}
		if(vi >= nrOfCardsPerField[combinations[i].Field])
                {
                    printf("Internal error %d %ld F%d (Pos 1)\n", vi, nrOfCardsPerField[combinations[i].Field], combinations[i].Card);
		    exit(1);
		}
		vi++;
		int vj;
                for(vj = 0; vj < nrOfCardsPerField[combinations[j].Field]; vj++)
                {
                    if(cardsPerField[combinations[j].Field][vj] == combinations[j].Card) break;
                }
                if(vj >= nrOfCardsPerField[combinations[j].Field])
                {
                    printf("Internal error %d %ld F%d (Pos 2)\n", vj, nrOfCardsPerField[combinations[j].Field], combinations[j].Field);
                    exit(1);
                }
		vj++;

                printf("c %d %d %d %d\n", i + 1, j + 1, vi, vj);
		// Rotation of i not needed here, jump over 2 bits
                for(int k = 0; k < 8; k++)
                {
                    printf("%d ", (vi & (1 << k)) == 0 ? 1 + 2 + combinations[i].Field * 10 + k : -(1 + 2 + combinations[i].Field * 10 + k));
                }
		// Rotation of j not needed here, jump over 2 bits
                for(int k = 0; k < 8; k++)
                {
                    printf("%d ", (vj & (1 << k)) == 0 ? 1 + 2 + combinations[j].Field * 10 + k : -(1 + 2 + combinations[j].Field * 10 + k));
                }
                printf("0\n");
            }
        }
    }

    printf("c Step 3\n");

    // Vertical neighbour
    for(i = 0; i < NR_OF_SAT_VARIABLES; i++)
    {
        for(j = i + 1; j < NR_OF_SAT_VARIABLES; j++)
        {
            if((combinations[i].Field + NR_OF_FIELDS_X) == combinations[j].Field && combinations[i].PatternSouth != combinations[j].PatternNorth && IsFieldEnabled(combinations[i].Field + NR_OF_FIELDS_X) && IsFieldEnabled(combinations[j].Field))
            {
                int ri = combinations[i].Rotation;
                int vi;
                for(vi = 0; vi < nrOfCardsPerField[combinations[i].Field]; vi++)
                {
                    if(cardsPerField[combinations[i].Field][vi] == combinations[i].Card) break;
                }
                if(vi >= nrOfCardsPerField[combinations[i].Field])
                {
                    printf("Internal error %d %ld (Pos 3)\n", vi, nrOfCardsPerField[combinations[i].Field]);
                    exit(1);
                }
                vi++;
		int rj = combinations[j].Rotation;
                int vj;
                for(vj = 0; vj < nrOfCardsPerField[combinations[j].Field]; vj++)
                {
                    if(cardsPerField[combinations[j].Field][vj] == combinations[j].Card) break;
                }
                if(vj >= nrOfCardsPerField[combinations[j].Field])
                {
                    printf("Internal error %d %ld F%d (Pos 4)\n", vj, nrOfCardsPerField[combinations[j].Field], combinations[j].Field);
                    exit(1);
                }
                vj++;
                printf("c %d %d F%d R%d V%d F%d R%d V%d\n", i + 1, j + 1, combinations[i].Field, ri, vi, combinations[j].Field, rj, vj);
		for(int k = 0; k < 2; k++)
                {
                    printf("%d ", (ri & (1 << k)) == 0 ? 1 + combinations[i].Field * 10 + k : -(1 + combinations[i].Field * 10 + k));
                }
                for(int k = 0; k < 8; k++)
                {
                    printf("%d ", (vi & (1 << k)) == 0 ? 1 + 2 + combinations[i].Field * 10 + k : -(1 + 2 + combinations[i].Field * 10 + k));
                }
                for(int k = 0; k < 2; k++)
                {
                    printf("%d ", (rj & (1 << k)) == 0 ? 1 + combinations[j].Field * 10 + k : -(1 + combinations[j].Field * 10 + k));
                }
                for(int k = 0; k < 8; k++)
                {
                    printf("%d ", (vj & (1 << k)) == 0 ? 1 + 2 + combinations[j].Field * 10 + k : -(1 + 2 + combinations[j].Field * 10 + k));
                }
                printf("0\n");
            }
        }
    }

    printf("c Step 4\n");

    // Horizontal neighbour
    for(i = 0; i < NR_OF_SAT_VARIABLES; i++)
    {
        for(j = i + 1; j < NR_OF_SAT_VARIABLES; j++)
        {
            if((combinations[i].Field + 1) == combinations[j].Field && combinations[i].PatternEast != combinations[j].PatternWest && IsFieldEnabled(combinations[i].Field + 1) && IsFieldEnabled(combinations[j].Field))
            {
                int ri = combinations[i].Rotation;
                int vi;
                for(vi = 0; vi < nrOfCardsPerField[combinations[i].Field]; vi++)
                {
                    if(cardsPerField[combinations[i].Field][vi] == combinations[i].Card) break;
                }
                if(vi >= nrOfCardsPerField[combinations[i].Field])
                {
                    printf("Internal error %d %ld (Pos 5)\n", vi, nrOfCardsPerField[combinations[i].Field]);
                    exit(1);
                }
                vi++;
                int rj = combinations[j].Rotation;
                int vj;
                for(vj = 0; vj < nrOfCardsPerField[combinations[j].Field]; vj++)
                {
                    if(cardsPerField[combinations[j].Field][vj] == combinations[j].Card) break;
                }
                if(vj >= nrOfCardsPerField[combinations[j].Field])
                {
                    printf("Internal error %d %ld F%d (Pos 6)\n", vj, nrOfCardsPerField[combinations[j].Field], combinations[j].Field);
                    exit(1);
                }
                vj++;
		printf("c %d %d F%d R%d V%d F%d R%d V%d\n", i + 1, j + 1, combinations[i].Field, ri, vi, combinations[j].Field, rj, vj);
                for(int k = 0; k < 2; k++)
                {
                    printf("%d ", (ri & (1 << k)) == 0 ? 1 + combinations[i].Field * 10 + k : -(1 + combinations[i].Field * 10 + k));
                }
                for(int k = 0; k < 8; k++)
                {
                    printf("%d ", (vi & (1 << k)) == 0 ? 1 + 2 + combinations[i].Field * 10 + k : -(1 + 2 + combinations[i].Field * 10 + k));
                }
                for(int k = 0; k < 2; k++)
                {
                    printf("%d ", (rj & (1 << k)) == 0 ? 1 + combinations[j].Field * 10 + k : -(1 + combinations[j].Field * 10 + k));
                }
                for(int k = 0; k < 8; k++)
                {
                    printf("%d ", (vj & (1 << k)) == 0 ? 1 + 2 + combinations[j].Field * 10 + k : -(1 + 2 + combinations[j].Field * 10 + k));
                }
                printf("0\n");
            }
        }
    }
}
