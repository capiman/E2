#define ENABLE_ETERNITY2                       1 
#define ENABLE_TESTBOARD_10X10                 0 
#define ENABLE_TESTBOARD_6X6                   0

#if ENABLE_ETERNITY2 == 1
#define PIECES_FILENAME                     "e2pieces.txt"
#define NR_OF_CARDS                                    256
#define NR_OF_FIELDS                                   256
#define NR_OF_FIELDS_X                                  16
#define NR_OF_FIELDS_Y                                  16
#define NR_OF_ROTATIONS                                  4
#define NR_OF_PATTERNS                                  23
#define MAXIMUM_OCCURRENCE2PATTERNS03                   12
#define ENABLE_POSSIBILITY_FILE                          1
#define LEVEL_SECOND_OPTIMIZATION                       8400
#define MAX_POSSIBILITIES_PER_PATTERN                   56
#define NR_OF_SAT_VARIABLES                         130180
// Following is equal to (int)((NR_OF_SAT_VARIABLES + 7) / 8)
#define LENGTH_OF_LINE_IN_POSSIBILITY_MATRIX         16273
#endif // ENABLE_ETERNITY2 == 1

#if ENABLE_TESTBOARD_10X10 == 1
#define PIECES_FILENAME                 "pieces_10x10.txt"
#define NR_OF_CARDS                                    100
#define NR_OF_FIELDS                                   100
#define NR_OF_FIELDS_X                                  10
#define NR_OF_FIELDS_Y                                  10
#define NR_OF_ROTATIONS                                  4
#define NR_OF_PATTERNS                     ('O' - 'A' + 1)
#define MAXIMUM_OCCURRENCE2PATTERNS03                    9
#define ENABLE_GEOFF_HINTS                               0
#define LEVEL_SECOND_OPTIMIZATION                       15+1000
#define ENABLE_POSSIBILITY_FILE                          1
#define MAX_POSSIBILITIES_PER_PATTERN                   56
#define NR_OF_SAT_VARIABLES                          17056
// Following is equal to (int)((NR_OF_SAT_VARIABLES + 7) / 8)
#define LENGTH_OF_LINE_IN_POSSIBILITY_MATRIX          2133
#endif // ENABLE_TESTBOARD_10X10 == 1

#if ENABLE_TESTBOARD_6X6 == 1
#define PIECES_FILENAME                 "pieces_06x06.txt"
#define NR_OF_CARDS                                     36
#define NR_OF_FIELDS                                    36
#define NR_OF_FIELDS_X                                   6
#define NR_OF_FIELDS_Y                                   6
#define NR_OF_ROTATIONS                                  4
#define NR_OF_PATTERNS                                   9
#define MAXIMUM_OCCURRENCE2PATTERNS03                    6
#define ENABLE_GEOFF_HINTS                               0
#define LEVEL_SECOND_OPTIMIZATION                        6 + 1000
#define ENABLE_POSSIBILITY_FILE                          0
#define MAX_POSSIBILITIES_PER_PATTERN                   56
#define NR_OF_SAT_VARIABLES                           1280
#define LENGTH_OF_LINE_IN_POSSIBILITY_MATRIX           161
#endif // ENABLE_TESTBOARD_6X6 == 1

#define PATTERN_BORDER 0

