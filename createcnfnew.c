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
"XXXXXXXXXXXXXXXX"
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
#if 0
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
#if 0 
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
#if 0
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
#if 0
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
#if 0 
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
#if 0 
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
"XXX............."
"XXX............."
"XXX............."
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
".............XXX"
".............XXX"
".............XXX"
;
#endif // 0
#if 1
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
    
    // At least 1 card per field
    // beside field is switched off
    for(i = 0; i < NR_OF_FIELDS; i++)
    {
        if(IsFieldEnabled(i))
        {
            for(j = 0; j < NR_OF_SAT_VARIABLES; j++)
            {
                if(combinations[j].Field == i)
                {
                    printf("%d ", combinations[j].Index);
                }
            }
            printf("0\n");
        }
        else
        {
            for(j = 0; j < NR_OF_SAT_VARIABLES; j++)
            {
                if(combinations[j].Field == i)
                {
                    printf("-%d 0\n", combinations[j].Index);
                }
            }
        }
    }

    // No two cards on same field
    // beside field is switched off
    for(i = 0; i < NR_OF_SAT_VARIABLES; i++)
    {
        for(j = i + 1; j < NR_OF_SAT_VARIABLES; j++)
        {
            if(IsFieldEnabled(combinations[i].Field) && combinations[i].Field == combinations[j].Field)
            {
                printf("-%d -%d 0\n", combinations[i].Index, combinations[j].Index);
            }
        }
    }

    // No cards on two fields
    for(i = 0; i < NR_OF_SAT_VARIABLES; i++)
    {
        for(j = i + 1; j < NR_OF_SAT_VARIABLES; j++)
        {
            if(combinations[i].Card == combinations[j].Card && combinations[i].Field != combinations[j].Field && IsFieldEnabled(combinations[i].Field) && IsFieldEnabled(combinations[j].Field))
            {
                printf("-%d -%d 0\n", combinations[i].Index, combinations[j].Index);
            }
        }
    }

    // Vertical neighbour
    for(i = 0; i < NR_OF_SAT_VARIABLES; i++)
    {
        for(j = i + 1; j < NR_OF_SAT_VARIABLES; j++)
        {
            if((combinations[i].Field + NR_OF_FIELDS_X) == combinations[j].Field && combinations[i].PatternSouth != combinations[j].PatternNorth && IsFieldEnabled(combinations[i].Field + NR_OF_FIELDS_X) && IsFieldEnabled(combinations[j].Field))
            {
                printf("-%d -%d 0\n", combinations[i].Index, combinations[j].Index);
            }
        }
    }

    // Horizontal neighbour
    for(i = 0; i < NR_OF_SAT_VARIABLES; i++)
    {
        for(j = i + 1; j < NR_OF_SAT_VARIABLES; j++)
        {
            if((combinations[i].Field + 1) == combinations[j].Field && combinations[i].PatternEast != combinations[j].PatternWest && IsFieldEnabled(combinations[i].Field + 1) && IsFieldEnabled(combinations[j].Field))
            {
                printf("-%d -%d 0\n", combinations[i].Index, combinations[j].Index);
            }
        }
    }
}
