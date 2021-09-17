#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

// #include "sha256.h"

#include "eter172c.c"

#include "e2_info.c"

// #define RUN_FAST 1
#define CHECK_WITH_256

unsigned char ring[] =
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
  0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 5, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 5, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0,
  0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

typedef enum
{
    INFO_UNKNOWN_COMBINATION = 0,
    INFO_PREVIOUSLY_USED_COMBINATION = 1,
    INFO_FORBIDDEN_COMBINATION = 2,
    INFO_FORBIDDEN_EASY_COMBINATION = 3
} enumInfoCombination;

unsigned char *forbiddenCombinations[1 + NR_OF_SAT_VARIABLES];

unsigned long fieldMin[NR_OF_FIELDS];
unsigned long fieldMax[NR_OF_FIELDS];

unsigned long nrOfForbiddenCombinations = 0;
unsigned long nrOfForbiddenEasyCombinations = 0;
unsigned long nrOfPreviouslyUsedCombinations = 0;

unsigned char *usedCombinationsCounter[1 + 130180];

unsigned long diffInvalid[NR_OF_FIELDS];

unsigned long totalSolutions = 0;
unsigned long totalInvalidSolutions = 0;

unsigned long usedIndizes[256];
unsigned long nrOfUsedIndizes = 0;

int checkSolutions(char *filename);
int checkAgainstCmsRedFile(char *filename);
int importUnsatFile(char *filename);

int GetRing(int index)
{
        return(ring[combinations[index-1].Field]);
}


unsigned char string_starts_with(char *string, char *searchstring)
{
    if(strncmp(string, searchstring, strlen(searchstring)) == 0)
    {
        return 1;
    }
    return 0;
}

unsigned char string_ends_with(char *string, char *searchstring)
{
    if(strlen(searchstring) > strlen(string)) 
    {
        return 0;
    }
    if(strncmp(string+strlen(string)-strlen(searchstring), searchstring, strlen(searchstring)) == 0)
    {
        return 1;
    }
    return 0;
}

int main(void) 
{
    memset(usedCombinationsCounter, 0, sizeof(usedCombinationsCounter));

    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        usedCombinationsCounter[i] = malloc((i + 1) * sizeof(usedCombinationsCounter[0][0]));
        memset(&usedCombinationsCounter[i][0], 0, (i + 1)  * sizeof(usedCombinationsCounter[0][0]));
    }

    memset(diffInvalid, 0, sizeof(diffInvalid));

    memset(forbiddenCombinations, 0, sizeof(forbiddenCombinations));

    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        forbiddenCombinations[i] = malloc((i + 1) * sizeof(forbiddenCombinations[0][0]));
        memset(&forbiddenCombinations[i][0], 0, (i + 1)  * sizeof(forbiddenCombinations[0][0]));
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

    for(int i = 0; i < 5; i++)
    {
        printf("c Field %d: min=%ld max=%ld\n", i, fieldMin[i], fieldMax[i]);
    }

    // Calculate easy forbidden combinations (e.g. same field, not suitable neighbour)
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        if((i % 1000) == 0) printf("c i=%d\n", i);
        for(int j = i + 1; j <= NR_OF_SAT_VARIABLES; j++)
        {
            if(combinations[i-1].Field == combinations[j-1].Field);
            else if(combinations[i-1].Card == combinations[j-1].Card);
            else if((combinations[i-1].Field + 1) == combinations[j-1].Field && combinations[i-1].PatternEast != combinations[j-1].PatternWest);
            else if((combinations[i-1].Field + 16) == combinations[j-1].Field && combinations[i-1].PatternSouth != combinations[j-1].PatternNorth);
            else continue;
            forbiddenCombinations[j][i] = INFO_FORBIDDEN_EASY_COMBINATION;
            nrOfForbiddenEasyCombinations++;
        }
    }

    printf("c Memory initialized\n");

    setvbuf(stdout, (char*)NULL, _IONBF, 0);

#if 1 
    importUnsatFile("e2_found_invalid_clauses_round_1.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_2.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_3.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_4.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_5.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_6.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_7.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_8.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_9.cnf");
    // importUnsatFile("e2_unsat_202104151313.cnf");
    // importUnsatFile("e2_unsat_unred_202107050830_all_clauses.cnf");
    // importUnsatFile("e2_found_invalid_clauses_round_55.cnf");
    // importUnsatFile("e2_found_invalid_clauses_round_56.cnf");
    // importUnsatFile("e2_found_invalid_clauses_round_57.cnf");
    // importUnsatFile("e2_found_invalid_clauses_round_58.cnf");
    // importUnsatFile("e2_found_invalid_clauses_round_59.cnf");
    // importUnsatFile("e2_unsat_unred_202108011651.cnf");
    // importUnsatFile("e2_unsat_unred_202108141059.cnf");
    // importUnsatFile("e2_unsat_unred_202108141402.cnf");
    // importUnsatFile("e2_unsat_unred_202108141949.cnf");
    // importUnsatFile("e2_unsat_unred_202108150914.cnf");
    // importUnsatFile("e2_unsat_unred_202108151542.cnf");
    // importUnsatFile("e2_unsat_unred_202108152136.cnf");
    // importUnsatFile("e2_unsat_unred_202108160716.cnf");
    // importUnsatFile("e2_unsat_unred_202108162226.cnf");
    // importUnsatFile("e2_unsat_unred_202108170716.cnf");
    // importUnsatFile("e2_unsat_unred_202108180704.cnf");
    // importUnsatFile("e2_unsat_unred_202108181212.cnf");
    importUnsatFile("e2_unsat_unred_202108181919.cnf");
    importUnsatFile("e2_unsat_unred_202108182112.cnf");
    importUnsatFile("e2_unsat_unred_202108190440.cnf");
    importUnsatFile("e2_unsat_unred_202108191034.cnf");
    importUnsatFile("e2_unsat_unred_202108202148.cnf");
    importUnsatFile("e2_unsat_unred_202108210702.cnf");
    importUnsatFile("e2_unsat_unred_202108212214.cnf");
    importUnsatFile("e2_unsat_manually_found_202108221228.cnf");
    importUnsatFile("e2_unsat_unred_202108220834.cnf");
    importUnsatFile("e2_unsat_unred_202108221248.cnf");
    importUnsatFile("e2_unsat_unred_202108221942.cnf");
    importUnsatFile("e2_unsat_from_earlier_directory_202107280724.cnf");
    importUnsatFile("e2_unsat_unred_202108230600.cnf");
    importUnsatFile("e2_unsat_unred_202108231055.cnf");
    importUnsatFile("e2_unsat_unred_202108231928.cnf");
    importUnsatFile("e2_unsat_unred_202108240701.cnf");
    importUnsatFile("e2_unsat_unred_202108242129.cnf");
    importUnsatFile("e2_unsat_unred_202108251024.cnf");
    importUnsatFile("e2_unsat_unred_202108252255.cnf");
    importUnsatFile("e2_found_invalid_clauses_202108251032.cnf");
    importUnsatFile("e2_unsat_unred_202108260720.cnf");
    importUnsatFile("e2_unsat_unred_202108260720.cnf");
    importUnsatFile("e2_found_invalid_clauses_202108260727.cnf");
    importUnsatFile("e2_unsat_unred_202108262325.cnf");
    importUnsatFile("e2_unsat_unred_202108270833.cnf");
    importUnsatFile("e2_unsat_unred_202108281649.cnf");
    importUnsatFile("e2_unsat_unred_202108291127.cnf");
    importUnsatFile("e2_unsat_unred_202108292108.cnf");
    importUnsatFile("e2_unsat_unred_202108300628.cnf");
    importUnsatFile("e2_found_invalid_clauses_202108300628.cnf");
    importUnsatFile("e2_unsat_unred_202108302136.cnf");
    importUnsatFile("e2_unsat_unred_202108310649.cnf");
    importUnsatFile("e2_unsat_unred_202108312210.cnf");
    importUnsatFile("e2_unsat_unred_202109010839.cnf");
    importUnsatFile("e2_unsat_unred_202109012052.cnf");
    importUnsatFile("e2_unsat_unred_202109020737.cnf");
    importUnsatFile("e2_unsat_unred_202109022231.cnf");
    importUnsatFile("e2_unsat_unred_202109031643.cnf");
    importUnsatFile("e2_unsat_unred_202109041000.cnf");
    importUnsatFile("e2_found_invalid_clauses_202109041000.cnf");
    importUnsatFile("e2_unsat_unred_202109050650.cnf");
    importUnsatFile("e2_unsat_unred_202109051857.cnf");
    importUnsatFile("e2_unsat_unred_202109060748.cnf");
    importUnsatFile("e2_unsat_unred_202109062247.cnf");
    importUnsatFile("e2_unsat_unred_202109071430.cnf");
    importUnsatFile("e2_unsat_unred_202109080711.cnf");
    importUnsatFile("e2_unsat_unred_202109082200.cnf");
    importUnsatFile("e2_unsat_unred_202109090809.cnf");
    importUnsatFile("e2_unsat_unred_202109092258.cnf");
    importUnsatFile("e2_unsat_unred_202109102140.cnf");
    importUnsatFile("e2_unsat_unred_202109111208.cnf");
    importUnsatFile("e2_unsat_unred_202109120716.cnf");
    importUnsatFile("e2_unsat_unred_202109122155.cnf");
    // Each clause of the following file is manually verified
    importUnsatFile("e2_unsat_unred_duplicate_runs_202109122215.cnf");
    importUnsatFile("e2_unsat_unred_202109132136.cnf");
    importUnsatFile("e2_unsat_unred_202109150724.cnf");
#endif // 0

    // UNSATUNSATUNSAT

    printf("c Final %ld forbidden easy combinations (FILTERSTAT)\n", nrOfForbiddenEasyCombinations);
    printf("c Final %ld forbidden combinations (FILTERSTAT)\n", nrOfForbiddenCombinations);
    printf("c Final %ld previous used combinations (FILTERSTAT)\n", nrOfPreviouslyUsedCombinations);

    totalSolutions = 0;
    totalInvalidSolutions = 0;

    // SEARCH36 (=9*4)

#if 0 
    {
        #include "e2_partial_f0_f1_f2_f16_f17_f18_f32_f33_f34.c"
        #include "e2_partial_f13_f14_f15_f29_f30_f31_f45_f46_f47.c"
        #include "e2_partial_f208_f209_f210_f224_f225_f226_f240_f241_f242.c"
        #include "e2_partial_f221_f222_f223_f237_f238_f239_f253_f254_f255.c"

        for(int tl = 0; tl < 3084; tl++)
        {
            fprintf(stderr, "tl=%d\n", tl);
            for(int tr = 0; tr < 3782; tr++)
            {
                // fprintf(stderr, "tl=%d,tr=%d\n", tl, tr);
                fprintf(stderr, "tl=%d,tr=%d,totalSolutions=%ld\n", tl, tr, totalSolutions);

                if(tr == 800) exit(1);

                int abort = 0;
                for(int i = 0; i < 9 && !abort; i++)
                {
                    for(int j = 0; j < 9 && !abort; j++)
                    {
                        if(forbiddenCombinations[topright[tr][j]][topleft[tl][i]] != INFO_UNKNOWN_COMBINATION) abort = 1;
                    }
                }
                if(abort) continue;
                for(int bl = 0; bl < 3195; bl++)
                {
                    // fprintf(stderr, "tl=%d,tr=%d,bl=%d,totalSolutions=%ld\n", tl, tr, bl, totalSolutions);
                    int abort = 0;
                    for(int i = 0; i < 9 && !abort; i++)
                    {
                        for(int j = 0; j < 9 && !abort; j++)
                        {
                            if(     forbiddenCombinations[topright[tr][j]][topleft[tl][i]] != INFO_UNKNOWN_COMBINATION) abort = 1;
                            else if(forbiddenCombinations[bottomleft[bl][j]][topleft[tl][i]] != INFO_UNKNOWN_COMBINATION) abort = 1;
                        }
                    }
                    if(abort) continue;
                    for(int br = 0; br < 2897; br++)
                    {
                        // fprintf(stderr, "tl=%d,tr=%d,bl=%d,br=%d\n", tl, tr, bl, br);
                        int abort = 0;
                        for(int i = 0; i < 9 && !abort; i++)
                        {
                            for(int j = 0; j < 9 && !abort; j++)
                            {
                                if(     forbiddenCombinations[topright[tr][j]][topleft[tl][i]] != INFO_UNKNOWN_COMBINATION) abort = 1;
                                else if(forbiddenCombinations[bottomleft[bl][j]][topleft[tl][i]] != INFO_UNKNOWN_COMBINATION) abort = 1;
                                else if(forbiddenCombinations[bottomright[br][j]][topleft[tl][i]] != INFO_UNKNOWN_COMBINATION) abort = 1;
                            }
                        }
                        if(abort) continue;

#if 0 
                        printf("SAT\n");
                        for(int i = 0; i < 9; i++)
                        {
                            printf("%ld ", topleft[tl][i]);
                        }
                        for(int i = 0; i < 9; i++)
                        {
                            printf("%ld ", topright[tr][i]);
                        }
                        for(int i = 0; i < 9; i++)
                        {
                            printf("%ld ", bottomleft[bl][i]);
                        }
                        for(int i = 0; i < 9; i++)
                        {
                            printf("%ld ", bottomright[br][i]);
                        }
                        printf("0\n"); 
#endif // 0
                        totalSolutions++;
                        // if(totalSolutions == 1000) exit(1);
                    }
                }
            }
        }
    }
#endif // 0

    // checkSolutions("e2_new_2021_only_field_0_to_3_compressed.res");
    // checkSolutions("e2_new_2021_only_field_0_to_4_compressed.res");

    // checkSolutions("e2_new_2021_only_ring_0_with_e2_unsat_202102271051_only_sol_0.res");
    // checkSolutions("e2_new_2021_only_ring_0_with_e2_unsat_202102271051.res");

    // checkSolutions("e2_only_second_inside_set_8956_set_9009.res");
#if 0    
    checkSolutions("e2_only_second_inside_set_8956_set_9010.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9013.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9014.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9016.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9017.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9019.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9020.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9022.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9025.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9028.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9033.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9036.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9037.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9038.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9040.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9041.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9043.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9045.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9047.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9049.res");
    checkSolutions("e2_only_second_inside_set_8956_set_9054.res");
#endif // 0    

#if 0 
    checkSolutions("e2_new_2019_only_second_inside.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24830.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24831.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24832.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24833.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24834.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24835.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24836.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24837.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24838.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24839.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24840.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24841.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24842.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24843.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24844.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24845.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24846.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24847.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24848.res");
    checkSolutions("e2_new_2019_only_second_inside_set_24849.res");
#endif // 0    

#if 0    
    checkSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101130728.res");
    checkSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101120924.res");
    checkSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101110932.res");
    checkSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101062023_transred_0_implicitmanip_0.res");
    checkSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101061118_transred_0_implicitmanip_0.res");
    checkSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101042256_transred_0_implicitmanip_0.res");

    checkSolutions("e2_partial_field_231_fields_filled_with_e2_unsat_202012170954.res");
    checkSolutions("e2_partial_field_229_fields_filled_with_e2_unsat_202012170954.res");
#endif // 0    

#if 0    
    checkSolutions("e2_test_partial_211_fields_set_1.res");
    checkSolutions("e2_test_partial_211_fields_set_2.res");
    checkSolutions("e2_test_partial_211_fields_set_3.res");
    checkSolutions("e2_test_partial_211_fields_set_4.res");
#endif // 0    

    // checkSolutions("e2_partial_field_233_fields_filled_with_e2_unsat_202012120750.res");
    // checkSolutions("e2_partial_field_231_fields_filled_with_e2_unsat_202012120750.res");
    // checkSolutions("e2_partial_field_229_fields_filled_with_e2_unsat_202012120750.res");

#if 0
    checkSolutions("e2_partial_field_231_fields_filled_with_e2_unsat_202012090857.res");
    checkSolutions("e2_partial_field_229_fields_filled_with_e2_unsat_202012090857.res");

    checkSolutions("e2_partial_field_233_fields_filled_with_e2_unsat_202012012203.res");
    checkSolutions("e2_partial_field_231_fields_filled_with_e2_unsat_202012012203.res");
    checkSolutions("e2_partial_field_229_fields_filled_with_e2_unsat_202012012203.res");

    checkSolutions("e2_partial_field_235_fields_filled_with_e2_unsat_202011080724.res");

    // exit(1);
    
    checkSolutions("e2_partial_field_231_fields_filled_with_e2_unsat_202011221231.res");
    checkSolutions("e2_partial_field_229_fields_filled_with_e2_unsat_202011221231.res");

    // exit(1);

    checkSolutions("e2_partial_field_233_fields_filled_with_e2_unsat_202011170001.res");
    checkSolutions("e2_partial_field_231_fields_filled_with_e2_unsat_202011170001.res");
    checkSolutions("e2_partial_field_229_fields_filled_with_e2_unsat_202011170001.res");

    // exit(1);
   
    checkSolutions("e2_partial_field_229_fields_filled_with_filter_4_unsat_5081_run1_set_9_run2.res");
    checkSolutions("e2_partial_field_231_fields_filled_with_filter_4_unsat_5081_run1_set_9_run2.res");
    checkSolutions("e2_partial_field_233_fields_filled_with_filter_4_unsat_5081_run1_set_9_run2.res");
    checkSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_9_run2.res");
#endif // 0

    // checkSolutions("e2_test_partial_229_fields_5081_solutions.res"); 

#if 1    
    // checkSolutions("e2_partial_field_229_fields_filled_with_unsat_202008151148_and_202008151329.res");
    // checkSolutions("e2_partial_field_229_fields_filled_with_unsat_202008231948_and_202008151329.res");
    // checkSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_22_run2.res");
    // checkSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_23_run2.res");
#endif // 0

    // Invalidated!
    // checkSolutions("e2_partial_field_lower_left_12_square_remaining_filled_edge_and_ring_2_and_ring_4_and_ring_6_completed_resextract_0_filter_4.res");

    // checkSolutions("e2_new_2021_only_ring_0_and_ring_1_202108020807.res");
    // checkSolutions("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_202108020807.res");
    // checkSolutions("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_202108020807.res");

    // checkSolutions("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_202108020807_resextract_0_ring_0_search_all_ring_0_ring_1.res");
    // checkSolutions("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_202108020807_resextract_11_ring_0_search_all_ring_0_ring_1.res");
    // checkSolutions("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_202108020807_resextract_21_ring_0_search_all_ring_0_ring_1.res");
 
    // checkSolutions("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_202108020807_resextract_0_ring_0_search_all_ring_0_ring_1_ring_2_ring_3.res");

    // checkSolutions("e2_new_2021_half_filled.res");

#if 0 
    for(int i = 0; i < NR_OF_SAT_VARIABLES; i++)
    {
        for(int j = i + 1; j < NR_OF_SAT_VARIABLES; j++)
        {
            if(usedCombinationsCounter[j][i] > 0)
            {
                printf("./runme_256_set_set_timeout.sh %6d %6d 180 OCCURANCE%03d\n", i, j, usedCombinationsCounter[j][i]);
            }
        }
    }
#endif // 0

#if 0 
    // CMS 5.0.2
    checkAgainstCmsRedFile("e2_red_202104051251.cnf");
    checkAgainstCmsRedFile("e2_red_202104110748.cnf");
    checkAgainstCmsRedFile("e2_red_202104151313.cnf");
    checkAgainstCmsRedFile("e2_red_202104161001.cnf");
    checkAgainstCmsRedFile("e2_red_202104170807.cnf");
    checkAgainstCmsRedFile("e2_red_202104172221.cnf");
    checkAgainstCmsRedFile("e2_red_202104180849.cnf");

    // CMS 20210418
    checkAgainstCmsRedFile("e2_red_current_cms_202104181055.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202104190643.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202104200653.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202104202251.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202104210937.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202104220721.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202104232109.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202104242120.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202104261432.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202104290815.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105011710.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105021926.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105061934.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105082148.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105112041.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105131322.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105150724.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105180805.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105210818.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105221238.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105250117.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105261906.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105272128.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105290828.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202105311113.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106011056.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106021058.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106022218.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106031030.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106032305.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106041132.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106051124.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106061216.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106062051.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106071055.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106081101.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106090009.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106090848.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106100711.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106101653.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106111141.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106112218.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106121132_marie_ubuntu.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106131108.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106131108_sls_16000.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106131108_transred_1_implicitmanip_1.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106140848.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106140848_sls_16000.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106140848_transred_1_implicitmanip_1.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106151227.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106151227_sls_16000.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106151227_transred_1_implicitmanip_1.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106170946.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106170946_transred_1_implicitmanip_1.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106181700.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106181700_transred_1_implicitmanip_1.cnf");
#endif // 0    

    // checkAgainstCmsRedFile("e2_red_current_cms_202106191557.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106191557_sls_16000.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106191557_transred_1_implicitmanip_1.cnf");

    // checkAgainstCmsRedFile("e2_red_current_cms_202106202218_all_clauses.cnf");
    
    // checkAgainstCmsRedFile("e2_red_current_cms_202106210628_all_clauses.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106210634_all_clauses_transred_1_implicitmanip_1.cnf");

    // checkAgainstCmsRedFile("e2_red_current_cms_202106220755_all_clauses.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106220755_all_clauses_transred_1_implicitmanip_1.cnf");

    // checkAgainstCmsRedFile("e2_red_current_cms_202106222139_all_clauses.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106222139_all_clauses_transred_1_implicitmanip_1.cnf");

    // checkAgainstCmsRedFile("e2_red_current_cms_202106230800_all_clauses.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106230800_all_clauses_transred_1_implicitmanip_1.cnf");

    // checkAgainstCmsRedFile("e2_red_current_cms_202106231633_all_clauses.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106231633_all_clauses_transred_1_implicitmanip_1.cnf");

    // checkAgainstCmsRedFile("e2_red_current_cms_202106241029_all_clauses.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106241029_all_clauses_transred_1_implicitmanip_1.cnf");

    // checkAgainstCmsRedFile("e2_red_current_cms_202106242033_all_clauses.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106242033_all_clauses_transred_1_implicitmanip_1.cnf");

    // checkAgainstCmsRedFile("e2_red_current_cms_202106250706_all_clauses.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106250706_all_clauses_transred_1_implicitmanip_1.cnf");

    // checkAgainstCmsRedFile("e2_red_current_cms_202106260830_all_clauses.cnf");
    // checkAgainstCmsRedFile("e2_red_current_cms_202106260830_all_clauses_transred_1_implicitmanip_1.cnf");

#if 0
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_0_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_1_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_2_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_3_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_5_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_6_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_7_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_8_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_9_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_10_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_12_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_13_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_14_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_15_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_16_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_17_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_100_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_101_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_102_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_103_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_104_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_105_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_106_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_108_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_109_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_110_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_111_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_112_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_114_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_115_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_116_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106260921_r_117_all_clauses.cnf");
#endif // 0

#if 0
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_0_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_1_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_2_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_4_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_5_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_6_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_7_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_8_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_9_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_10_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_12_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_13_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_14_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_15_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_16_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_17_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_100_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_102_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_103_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_104_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_105_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_106_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_107_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_108_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_109_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_111_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_112_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_113_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_114_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_115_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_116_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106270841_r_117_all_clauses.cnf");
#endif // 0

#if 0
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_0_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_100_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_101_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_102_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_103_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_104_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_105_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_106_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_107_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_108_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_109_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_10_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_110_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_111_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_112_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_113_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_114_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_115_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_11_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_12_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_13_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_14_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_15_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_1_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_2_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_3_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_4_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_5_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_6_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_7_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_8_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106280938_r_9_all_clauses.cnf");
#endif // 0

#if 0
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_0_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_100_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_101_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_102_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_103_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_104_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_105_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_106_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_107_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_108_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_109_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_10_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_110_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_111_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_112_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_113_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_115_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_12_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_13_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_14_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_15_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_1_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_2_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_3_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_4_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_5_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_6_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_7_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_8_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_9_all_clauses.cnf");
#endif // 0

#if 0
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_0_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_100_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_101_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_102_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_103_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_104_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_105_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_106_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_107_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_108_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_109_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_10_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_110_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_111_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_112_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_113_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_115_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_12_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_13_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_14_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_15_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_1_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_2_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_3_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_4_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_5_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_6_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_7_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_8_all_clauses.cnf");
    checkAgainstCmsRedFile("e2_red_current_cms_202106290838_r_9_all_clauses.cnf");
#endif // 0

    // checkAgainstCmsRedFile("e2_test_202108011013_red.cnf");

    

    // checkAgainstCmsRedFile("e2_unsat_unred_202105311102.cnf");

#if 0 
    checkAgainstCmsRedFile("e2_full_plus_arc_red_sky_concat.cnf");
    checkAgainstCmsRedFile("e2_full_plus_arc_red_80_to_150_sky_concat.cnf");
    checkAgainstCmsRedFile("e2_partial_0_to_4_red_0_to_1112_sky_concat.cnf");
    checkAgainstCmsRedFile("e2_new_2019_with_e2_unsat_202101110932_red.cnf");
#endif // 0    

    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108141402_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108141402_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108141402_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108141949_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108141949_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108141949_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108150914_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108150914_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108150914_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108150914_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108151542_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108151542_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108151542_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108151542_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108152136_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108152136_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108152136_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108152136_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108160716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_holes_202108160716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108160716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108160716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108160716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_two_holes_left_right_202108160716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_two_holes_top_bottom_202108160716_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108162226_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108162226_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108162226_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108170716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_epyc_202108170716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108170716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108170716_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108180704_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108180704_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108180704_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108180704_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108181212_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108181212_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108181212_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108181212_red.cnf");

    // Ab hier Neustart der RED-Files, weil Problem mit allclauses!?!
    // Wenn man nicht volles Feld mit allclauses nimmt, kommt es zu fehlerhaften Berechnungen!?!

    // checkAgainstCmsRedFile("e2_new_2019_202108181919_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_holes_202108181919_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108181919_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108181919_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108181919_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108182112_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108182112_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108182112_red.cnf");

    // This is old file with 1,4 million entries
    // But it results only in 12000 new clauses
    // Danger too big, to introduce new error
    // we get same amount of new clauses by using CMS again and again
    // checkAgainstCmsRedFile("e2_unsat_from_earlier_directory_202107280724_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108190440_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108190440_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108191034_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108191034_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108191034_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108191034_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108191749_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108191749_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108190440_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108191749_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108191749_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108192049_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108192049_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108192049_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108192049_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108192049_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108200653_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108200653_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108200653_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108200653_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108200653_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108201333_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108201333_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108201333_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108201333_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108201333_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108202148_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108202148_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108202148_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108202148_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108202148_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108210702_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108210702_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108210702_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108210702_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108210702_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108212214_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108212214_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108212214_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108212214_red.cnf");

    // checkAgainstCmsRedFile("e2_unsat_manually_found_202108221228_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108220834_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108220834_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108220834_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108220834_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108220834_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108221248_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108221248_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108221248_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108221248_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108221248_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108221942_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108221942_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108221942_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108221942_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108221942_red.cnf");

    // checkAgainstCmsRedFile("e2_unsat_found_earlier_202108231044_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108230600_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108230600_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108230600_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108230600_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108230600_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108231055_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108231055_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108231055_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108231055_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108231055_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108231928_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108231928_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108231928_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108231928_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108240701_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108240701_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108240701_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108240701_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108242129_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108242129_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108242129_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108242129_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108251024_epyc_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108252255_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108252255_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108252255_red.cnf");

    // checkAgainstCmsRedFile("e2_found_invalid_clauses_202108251032_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108260720_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108260720_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108260720_red.cnf");

    // checkAgainstCmsRedFile("e2_found_invalid_clauses_202108260727_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108262325_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108262325_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108262325_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108270833_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108270833_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108270833_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108281649_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108281649_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108281649_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108291127_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108291127_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108291127_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108291127_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108291127_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_202108291127_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108292108_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108292108_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108292108_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108292108_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_202108292108_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108300628_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108300628_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108300628_red.cnf");

    // checkAgainstCmsRedFile("e2_found_invalid_clauses_202108300628_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108302136_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108302136_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108302136_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108310649_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108310649_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108310649_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108310649_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202108312210_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202108312210_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202108312210_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202108312210_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109010839_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109010839_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109010839_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109010839_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109012052_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109012052_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109012052_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109012052_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109020737_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109020737_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109020737_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109020737_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109022231_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109022231_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109022231_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109022231_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109031643_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109031643_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109031643_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109031643_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109041000_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109041000_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109041000_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109041000_red.cnf");

    // checkAgainstCmsRedFile("e2_found_invalid_clauses_202109041000_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109050650_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109050650_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109050650_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109051857_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109051857_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109051857_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109060748_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109060748_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109060748_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109060748_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109062247_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109062247_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109062247_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109062247_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109071430_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109071430_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109071430_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109071430_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109080711_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109080711_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109080711_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109080711_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109082200_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109082200_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109082200_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109090809_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109090809_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109090809_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109092258_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109092258_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109092258_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109102140_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109102140_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109102140_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109111208_epyc_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109120716_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2019_202109120716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109120716_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_202109120716_red.cnf");

    // checkAgainstCmsRedFile("mytest2_red.cnf");
    // checkAgainstCmsRedFile("mytest_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109122155_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109122155_red.cnf");

    // checkAgainstCmsRedFile("e2_unsat_unred_duplicate_runs_202109122215_red.cnf");

    // checkAgainstCmsRedFile("e2_new_2019_202109132136_epyc_red.cnf");
    // checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109132136_red.cnf");

    checkAgainstCmsRedFile("e2_new_2019_202109150724_epyc_red.cnf");
    checkAgainstCmsRedFile("e2_new_2021_four_smaller_holes_202109150724_red.cnf");

    // End of REDREDRED

    printf("c totalSolutions=%ld totalInvalidSolutions=%ld\n", totalSolutions, totalInvalidSolutions);

#if 0    
    for(int i = 1; i < NR_OF_FIELDS; i++)
    {
        if(diffInvalid[i] != 0) printf("diffInvalid[%d]=%ld\n", i, diffInvalid[i]);
    }
#endif // 0    

    exit(1);

#if 0
    d = opendir(".");
    if (d) 
    {
        while ((dir = readdir(d)) != NULL) 
        {
            printf("c %s\n", dir->d_name);
            if(string_starts_with(dir->d_name, "e2_partial_field_2") &&
               string_starts_with(dir->d_name+20, "_fields_filled_with_filter_4_unsat_5081_run1_set"))
            {
                printf("c 1\n");
                if(string_ends_with(dir->d_name, ".res"))
                {
                    unsigned long filledFields;
                    unsigned long z1, z2;

                    printf("c 2\n");

                    sscanf(dir->d_name, "e2_partial_field_%ld_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", &filledFields, &z1, &z2);

                    printf("c filledFields=%ld,z1=%ld,z2=%ld\n", filledFields, z1, z2);

		    if(filledFields == 229 && (enabled_fields[combinations[z1-1].Field] == '.' || enabled_fields[combinations[z2-1].Field] == '.'))
                    {
                        printf("./delete229disabledfield_set_set.sh %ld %ld DUMMY\n", z1, z2);
                    } 

                    struct stat statbuf;
                    if(stat(dir->d_name, &statbuf) == 0)
                    {
                        printf("c # size=%ld\n", statbuf.st_size);

                        if(statbuf.st_size == 6)
                        {
                            unsigned long fz1 = z1;
                            unsigned long fz2 = z2;
                            if(z1 > z2)
                            {
                                fz1 = z2;
                                fz2 = z1;
                            }
                            else if(z1 == z2)
                            {
                                printf("Same numbers %ld !!!\n", z1);
                                exit(1);
                            }
                            if(forbiddenCombinations[z2][z1] == INFO_FORBIDDEN_EASY_COMBINATION)
                            {
                                printf("c Hint: %ld %ld already forbidden by easy combination\n", z1, z2);
                                printf("./removeme.sh %ld %ld\n", z1, z2);
                            }
                            forbiddenCombinations[z2][z1] = INFO_FORBIDDEN_COMBINATION;
                            nrOfForbiddenCombinations++;
                        }
                        else if(statbuf.st_size == 930112 || statbuf.st_size == 930110 || 
                                statbuf.st_size == 930108 || statbuf.st_size == 930106)
                        {
                            importSolutions(dir->d_name);
                        }
                    }
                }
            }
        }
        closedir(d);
    }

    setvbuf(stdout, (char*)NULL, _IONBF, 0);

    printf("c %ld forbidden easy combinations (FILTERSTAT)\n", nrOfForbiddenEasyCombinations);
    printf("c %ld forbidden combinations (FILTERSTAT)\n", nrOfForbiddenCombinations);
    printf("c %ld previous used combinations (FILTERSTAT)\n", nrOfPreviouslyUsedCombinations);

#if 0  
    // Print collected UNSATs (but not the easy combinations)
    for(int i = 1; i <= 130180; i++)
    {
        for(int j = i + 1; j <= 130180; j++)
        {
            if(forbiddenCombinations[j][i] == INFO_FORBIDDEN_COMBINATION)
            {
                printf("-%d -%d 0\n", i, j);
            }
        }
    }
    exit(1);
#endif // 0

#if 0 
    int shellnr = 1;
    // Print combination, where we have no info, so second server can do some work in parallel 
    for(int i = 1; i <= 130180; i++)
    {
	// Don't output clauses from disabled fields (e.g. only 229)
        // This would be an error, leads to UNSAT and hard to find!!!
        if(enabled_fields[combinations[i-1].Field] == '.') continue;

        // if(i != 1226) continue;

	int fi = combinations[i-1].Field; 

        for(int j = i + 1; j <= 130180; j++)
        {
            // Don't output clauses from disabled fields (e.g. only 229)
            // This would be an error, leads to UNSAT and hard to find!!!
            if(enabled_fields[combinations[j-1].Field] == '.') continue;

	    int fj = combinations[j-1].Field;

	    // Same field
	    if(fi == fj) continue;

	    if((fi + 1) == fj);
	    else if((fi + 2) == fj);
	    else if((fi + 16) == fj);
	    else if((fi + 16 + 1) == fj);
	    else if((fi + 16 + 2) == fj);
	    else if((fi + 32) == fj);
	    else if((fi + 32 + 1) == fj);
	    else if((fi + 32 + 2) == fj);
	    else continue;

            if(forbiddenCombinations[j][i] == INFO_UNKNOWN_COMBINATION)
            {
                printf("./runme_229_set_set.sh %d %d PROCESS%02d\n", i, j, shellnr);
                shellnr++;
                if(shellnr > 25) shellnr = 1;
            }
        }
    }
    exit(1);
#endif // 0

#if 0 
    int shellnr = 1;
    // Print combination, where we have no info, so second server can do some work in parallel
    for(int i = 1; i <= 130180; i++)
    {
        // Don't output clauses from disabled fields (e.g. only 229)
        // This would be an error, leads to UNSAT and hard to find!!!
        if(enabled_fields[combinations[i-1].Field] == '.') continue;

        // if(i != 1226) continue;

        int fi = combinations[i-1].Field;

        for(int j = i + 1; j <= 130180; j++)
        {
            // Don't output clauses from disabled fields (e.g. only 229)
            // This would be an error, leads to UNSAT and hard to find!!!
            if(enabled_fields[combinations[j-1].Field] == '.') continue;

            int fj = combinations[j-1].Field;

            // Same field
            if(fi == fj) continue;

            if((fi + 1) == fj);
            else if((fi + 2) == fj);
            else if((fi + 16) == fj);
            else if((fi + 16 + 1) == fj);
            else if((fi + 16 + 2) == fj);
            else if((fi + 32) == fj);
            else if((fi + 32 + 1) == fj);
            else if((fi + 32 + 2) == fj);
            else continue;

            if(forbiddenCombinations[j][i] == INFO_UNKNOWN_COMBINATION)
            {
                printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d\n", i, j, shellnr);
                shellnr++;
                if(shellnr > 25) shellnr = 1;
            }
        }
    }
    exit(1);
#endif // 0

#if 0 
    // Print combination as UNSAT clause, which were not used yet (just for testing, perhaps we find 256er solution faster...?) 
    for(int i = 1; i <= 130180; i++)
    {
        if(i > 4) continue;
        for(int j = i + 1; j <= 130180; j++)
        {
            if(forbiddenCombinations[j][i] == INFO_UNKNOWN_COMBINATION)
            {
                    printf("-%d -%d 0\n", i, j);
            }
        }
    }
    exit(1);
#endif // 0

#if 0 
    unsigned long nrOfImpossibleClausesOfTwoFields[256][256];
    memset(nrOfImpossibleClausesOfTwoFields, 0, sizeof(nrOfImpossibleClausesOfTwoFields));
    // Print statistic how many forbidden clauses (not easy ones) are collected till now
    for(int i = 1; i <= 130180; i++)
    {
        for(int j = i + 1; j <= 130180; j++)
        {
            if(forbiddenCombinations[j][i] == INFO_FORBIDDEN_COMBINATION)
            {
                unsigned long field_i = combinations[i-1].Field;
		unsigned long field_j = combinations[j-1].Field;
		nrOfImpossibleClausesOfTwoFields[field_i][field_j]++;
            }
        }
    }
    for(int f0 = 0; f0 < 256; f0++)
    {
        printf("Field %d:\n\n", f0);
        for(int f1 = 0; f1 < 256; f1++)
        {
            if(f0 == f1)
	    {
                printf("o");
	    }
	    else if(f1 == ( 2*16 +  2) || f1 == ( 2*16 + 13) ||
                    f1 == 135 ||
                    f1 == (13*16 +  2) || f1 == (13*16 + 13))
            {
                printf("H");
            }
	    else if(nrOfImpossibleClausesOfTwoFields[f0][f1] != 0 || nrOfImpossibleClausesOfTwoFields[f1][f0] != 0)
            {
                printf("X");
            }
            else
            {
                printf(".");
            }
	    if((f1 % 16) == 15) printf("\n");
        }
	printf("\n");
    }

    exit(1);
#endif // 0

    static unsigned char Abort;

    pathWay[0] = fieldMin[0] + 0;
    step = 0;

    while(1)
    {
#if 0        
        printf("TEST (%ld):", step);
        for(int i = 0; i <= step; i++)
        {
            printf(" %ld", pathWay[i]);
        }
        printf("\n");

        getchar();
#endif // 0

        Abort = 0;

        if(step >= 16)
        {
            if(forbiddenCombinations[pathWay[step]][pathWay[step-16]] == INFO_FORBIDDEN_COMBINATION ||
               forbiddenCombinations[pathWay[step]][pathWay[step-16]] == INFO_FORBIDDEN_EASY_COMBINATION)
            {
                Abort = 1;
            }
        }

        if(Abort == 0 && step >= 1)
        {
            for(int sc = step - 1; sc >= 0; sc--)
            {
                if(forbiddenCombinations[pathWay[step]][pathWay[sc]] == INFO_FORBIDDEN_COMBINATION ||
                   forbiddenCombinations[pathWay[step]][pathWay[sc]] == INFO_FORBIDDEN_EASY_COMBINATION)
                {
                    Abort = 1;
                    break;
                }
                else if(enabled_fields[combinations[pathWay[sc]-1].Field] != '.' &&
                        enabled_fields[combinations[pathWay[step]-1].Field] != '.')
                {
                    char name229[1024];
                    struct stat statbuf229;
                    sprintf(name229, "e2_partial_field_229_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", pathWay[sc], pathWay[step]);
                    printf("name229: %s\n", name229);
                    if(stat(name229, &statbuf229) == 0)
                    {
                        printf("# size(229)=%ld\n", statbuf229.st_size);
                        if(statbuf229.st_size == 6)
                        {
                            // Case: File meanwhile exists, but not yet in forbiddenCombinations
                            Abort = 1;
                            break;
                        }
                    }
                }

		if(!Abort)
                {
                    char name256[1024];
                    struct stat statbuf256;
                    sprintf(name256, "e2_partial_field_256_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", pathWay[sc], pathWay[step]);
                    printf("name256: %s\n", name256);
                    if(stat(name256, &statbuf256) == 0)
                    {
                        printf("# size(256)=%ld\n", statbuf256.st_size);
                        if(statbuf256.st_size == 6)
                        {
                            // Case: File meanwhile exists, but not yet in forbiddenCombinations
                            Abort = 1;
                            break;
                        }
                    }
                }
            }

#if defined RUN_FAST
#else        
            if(Abort == 0)
            {
                for(int sc = step - 1; sc >= 0; sc--)
                {
                    if(forbiddenCombinations[pathWay[step]][pathWay[sc]] == INFO_FORBIDDEN_COMBINATION ||
                       forbiddenCombinations[pathWay[step]][pathWay[sc]] == INFO_FORBIDDEN_EASY_COMBINATION)
                    {
                        Abort = 1;
                        break;
                    }
                    else if(forbiddenCombinations[pathWay[step]][pathWay[sc]] == INFO_PREVIOUSLY_USED_COMBINATION)
                    {
                        // We have found a solution earlier, so no need to search for a new one 
                    }
#if defined CHECK_WITH_229		    
                    else if(enabled_fields[combinations[pathWay[sc]-1].Field] != '.' &&
                            enabled_fields[combinations[pathWay[step]-1].Field] != '.')
                    {
                        char name229[1024];
                        struct stat statbuf229;
                        sprintf(name229, "e2_partial_field_229_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", pathWay[sc], pathWay[step]);
                        printf("name229: %s\n", name229);
                        if(stat(name229, &statbuf229) == 0)
                        {
                            printf("# size(229)=%ld\n", statbuf229.st_size);
                            if(statbuf229.st_size == 6)
                            {
                                // Case: File meanwhile exists, but not yet in forbiddenCombinations
                                Abort = 1;
                                break;
                            }
                            else if(statbuf229.st_size == 0)
                            {
                                // Case: Seems there is currently a check running
                                // To avoid two tasks working in same path, just skip it for the moment
                                // Next run, we will fill holes 
                                Abort = 1;
                                break;
                            }
                        }
                        else
                        {
                            // File does not exists
                            char cmd229[1024];
                            int rv;
                            // printf("errno=%d", errno);
                            if(pathWay[sc] >= pathWay[step])
                            {
                                printf("Internal error\n");
                                printf("%ld %ld", pathWay[sc], pathWay[step]);
                                exit(1);
                            }
                            sprintf(cmd229, "./runme_229_set_set.sh %ld %ld", pathWay[sc], pathWay[step]);
                            printf("CMD: %s\n", cmd229);
                            rv = system(cmd229);
                            printf("rv=%d\n", rv);
#if 0
                            if(rv == 2)
                            {
                                printf("Remove unfinished file\n");
                                sprintf(cmd229, "rm e2_partial_field_229_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", pathWay[sc], pathWay[step]);
                                printf("CMD: %s\n", cmd229);
                                system(cmd229);
                                exit(1);
                            }
                            if(rv == 20)
                            {
                                // Detected, this is UNSAT
                                Abort = 1;
                                break;
                            }
#endif // 0
                            char name229again[1024];
                            struct stat statbuf229again;
                            sprintf(name229again, "e2_partial_field_229_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", pathWay[sc], pathWay[step]);
                            printf("name229again: %s\n", name229again);
                            if(stat(name229again, &statbuf229again) == 0)
                            {
                                printf("# size(229again)=%ld\n", statbuf229again.st_size);
                                if(statbuf229again.st_size == 6)
                                {
                                    // Case: UNSAT 
                                    Abort = 1;
                                    break;
                                }
                            }
                        }
                    }
#endif // defined CHECK_WITH_229		    
#if defined CHECK_WITH_256
                    else
                    {
                        unsigned char check_set_set = 1;

                        char name229[1024];
                        struct stat statbuf229;
                        sprintf(name229, "e2_partial_field_229_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", pathWay[sc], pathWay[step]);
                        printf("name229: %s\n", name229);
                        if(stat(name229, &statbuf229) == 0)
                        {
                            printf("# size(229)=%ld\n", statbuf229.st_size);
                            if(statbuf229.st_size == 6)
                            {
                                // Case: File meanwhile exists, but not yet in forbiddenCombinations
                                Abort = 1;
                                break;
                            }
                            else if(statbuf229.st_size == 0)
                            {
                                // Case: Seems there is currently a check running
                                // To avoid two tasks working in same path, just skip it for the moment
                                // Next run, we will fill holes
                                Abort = 1;
                                break;
                            }
                            check_set_set = 0;    
                        }

                        char name256[1024];
                        struct stat statbuf256;
                        sprintf(name256, "e2_partial_field_256_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", pathWay[sc], pathWay[step]);
                        printf("name256: %s\n", name256);
                        if(stat(name256, &statbuf256) == 0)
                        {
                            printf("# size(256)=%ld\n", statbuf256.st_size);
                            if(statbuf256.st_size == 6)
                            {
                                // Case: File meanwhile exists, but not yet in forbiddenCombinations
                                Abort = 1;
                                break;
                            }
                            else if(statbuf256.st_size == 0)
                            {
                                // Case: Seems there is currently a check running
                                // To avoid two tasks working in same path, just skip it for the moment
                                // Next run, we will fill holes
                                Abort = 1;
                                break;
                            }
			    check_set_set = 0;
                        }

			if(check_set_set)
			{
                            // File does not exists, not 229 nor 256
                            char cmd256[1024];
                            int rv;
                            // printf("errno=%d", errno);
                            if(pathWay[sc] >= pathWay[step])
                            {
                                printf("Internal error\n");
                                printf("%ld %ld", pathWay[sc], pathWay[step]);
                                exit(1);
                            }
                            sprintf(cmd256, "./runme_256_set_set_timeout.sh %ld %ld %d", pathWay[sc], pathWay[step], 180);
                            printf("CMD256: %s\n", cmd256);
                            rv = system(cmd256);
                            printf("rv=%d\n", rv);
                            char name256again[1024];
                            struct stat statbuf256again;
                            sprintf(name256again, "e2_partial_field_256_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", pathWay[sc], pathWay[step]);
                            printf("name256again: %s\n", name256again);
                            if(stat(name256again, &statbuf256again) == 0)
                            {
                                printf("# size(256again)=%ld\n", statbuf256again.st_size);
                                if(statbuf256again.st_size == 6)
                                {
                                    // Case: UNSAT
                                    Abort = 1;
                                    break;
                                }
                            }
                        }
                    }
#endif // defined CHECK_WITH_256
                }
            }
#endif // defined RUN_FAST
        }

        if(!Abort)
        {
            char filename[2048];
	    char filename_org[2048];
            char filename_res[2048+4];
            char filename_cnf[2048+4];
            int anz = 0;

            anz = sprintf(filename, "e2_256_multiset");

            for(int sc = 0; sc <= step; sc++)
            {
                anz += sprintf(filename + anz, "_%ld", pathWay[sc]);
            }

	    sprintf(filename_org, "%s", filename);

	    if(strlen(filename) >= 240)
            {
                SHA256_CTX ctx;
		BYTE buf[SHA256_BLOCK_SIZE];

		printf("Filename2Hash: %s\n", filename);

                sha256_init(&ctx);
                sha256_update(&ctx, filename, strlen(filename));
                sha256_final(&ctx, buf);

		anz = sprintf(filename, "e2_256_multiset_hash_");

		for(int i = 0; i < SHA256_BLOCK_SIZE; i++)
		{
                    anz += sprintf(filename + anz, "%02X", buf[i]);
		}
            }

            sprintf(filename_res, "%s.res", filename);
            sprintf(filename_cnf, "%s.cnf", filename);

            struct stat statbuf256;
            printf("name256res: %s\n", filename_res);
            if(stat(filename_res, &statbuf256) == 0)
            {
                printf("# size(256)=%ld\n", statbuf256.st_size);
                if(statbuf256.st_size == 6)
                {
                    // Case: UNSAT
                    Abort = 1;
                }
                else if(statbuf256.st_size == 0)
                {
                    // Case: Someone else is already checking 
                    // We can do something else. Going deeper is useless.
                    // the other will do the same
                    Abort = 1;
                }
            }
            else
            {
                char cmd256[2048+64];
                FILE *fpcnf;

                fpcnf = fopen(filename_cnf, "w+");
		if(fpcnf == NULL)
		{
                    printf("c Can't open file %s (filename too long?)\n", filename_cnf);
                    Abort = 1;	
		}
		else
		{
                    fprintf(fpcnf, "c %s\n", filename_org);
                    for(int sc = 0; sc <= step; sc++)
                    {
                        // printf("c sc=%d\n", sc);
                        // printf("c pathWay[%d]=%ld\n", sc, pathWay[sc]);
                        fprintf(fpcnf, "%ld 0\n", pathWay[sc]);
                    }

                    fclose(fpcnf);

                    sprintf(cmd256, "./runme_256_multiset_time.sh %s %ld", filename, 180+2*step);
                    printf("CMD256 %s\n", cmd256);
                    system(cmd256);

                    if(stat(filename_res, &statbuf256) == 0)
                        {
                        printf("# size(256again)=%ld\n", statbuf256.st_size);
                        if(statbuf256.st_size == 6)
                        {
                            // Case: Now we have UNSAT
                            Abort = 1;
                        }
                    }
                    else
                    {
                        printf("There is something wrong checking 256\n");
                        exit(1);
                    }
                }
            }
	}

        if(Abort)
        {
            while(1)
            {
                if(pathWay[step] < fieldMax[step])
                {
                    pathWay[step]++;
                    break;
                }
                if(step > 0)
                {
                    step--;
                }
                else
                {
                    printf("End reached\n");
                    exit(1);
                }
            }
        }
        else
        {
            step++;
            pathWay[step] = fieldMin[step];
            if(step > stepMax)
            {
                stepMax = step;
                printf("(%ld):", step);
                for(int i = 0; i < step; i++)
                {
                    printf(" %ld", pathWay[i]);
                }
                printf("\n");            
                if(step >= 211)
                {
                    for(int i = 0; i < step; i++)
                    {
                        printf("%ld 0\n", pathWay[i]);
                    }
                }
            }
        }
    }
#endif // 0

    return(0);
}


int checkSolutions(char *filename)
{
    int var;
    int rv;
    FILE *fpin;
    unsigned long nrOfSolution = 0;
    unsigned long nrOfStillValidSolutions = 0;

    unsigned long validBinaries = 0;
    unsigned long invalidBinaries = 0;
    unsigned long unknownBinaries = 0;

    // int invalid_i0 = 0;
    // int invalid_i1 = 0;

    int processnr = 1;

    printf("c Check solution file %s\n", filename);
   
    fpin = fopen(filename, "r");

    if(fpin == NULL) 
    {
        printf("Can't open solution file %s\n", filename);
        exit(1);
    }

    fseek ( fpin, 0 , SEEK_SET );

    nrOfUsedIndizes = 0;

    while(1)
    {
        if(feof(fpin))
        {
            printf("c eof reached\n");
            break;
        }

	char string_at_or_unsat[1024];

        if(fscanf(fpin, "%s\n", string_at_or_unsat) != 1)
        {
            fprintf(stderr, "error: expected more input\n");
            exit(1);
        }

	if(strcmp(string_at_or_unsat, "SAT") == 0);
	else if(strcmp(string_at_or_unsat, "UNSAT") == 0)
	{
            break;
	}
	else
	{
            fprintf(stderr, "error: expected: SAT or UNSAT\n");
            exit(1);
	}
        
        while(1)
        {
            rv = fscanf(fpin, "%d", &var);
            if(rv == -1)
            {
            printf("fscanf returned -1\n");
                exit(1);
            }
            else if(rv != 1)
            {
                fprintf(stderr, "error: var=%d rv=%d\n", var, rv);
                exit(1);
            }

            if(var == 0) 
            {
                unsigned char solution_found_invalid = 0;

                for(int step = 1; step <= 2; step++)
                {
                    // 1 solution imported completely
                    for(int i = 0; i < nrOfUsedIndizes; i++)
                    {
                        if(step == 1 && solution_found_invalid) break;
                        for(int j = i + 1; j < nrOfUsedIndizes; j++)
                        {
                            if(step == 1 && solution_found_invalid) break;
                            if(step == 1)
                            {
                                if(forbiddenCombinations[usedIndizes[j]][usedIndizes[i]] == INFO_FORBIDDEN_COMBINATION)
                                {
                                    solution_found_invalid = 1;
                                }
                                else if(forbiddenCombinations[usedIndizes[j]][usedIndizes[i]] == INFO_FORBIDDEN_EASY_COMBINATION)
                                {
                                    printf("There seems to be something wrong with solution: %ld %ld\n", usedIndizes[i], usedIndizes[j]);
                                    exit(1);
                                }
                            }
                            else if(step == 2)
                            {
                                if(solution_found_invalid)
                                {
                                    if(forbiddenCombinations[usedIndizes[j]][usedIndizes[i]] == INFO_FORBIDDEN_COMBINATION)
                                    {
                                        int invalid_i0 = usedIndizes[i];
                                        int invalid_i1 = usedIndizes[j];
                                        printf("c %s nr %ld Found invalid(%d/%d)(F%dC%dR%d/F%dC%dR%d)(Ring%d/Ring%d)\n", filename, nrOfSolution, invalid_i0, invalid_i1,
                                            combinations[invalid_i0-1].Field, combinations[invalid_i0-1].Card, combinations[invalid_i0-1].Rotation,
                                            combinations[invalid_i1-1].Field, combinations[invalid_i1-1].Card, combinations[invalid_i1-1].Rotation,
                                            GetRing(invalid_i0), GetRing(invalid_i1));
                                    }
                                }
                                else
                                {
                                    if(forbiddenCombinations[usedIndizes[j]][usedIndizes[i]] == INFO_UNKNOWN_COMBINATION)
                                    {
                                        int invalid_i0 = usedIndizes[i];
                                        int invalid_i1 = usedIndizes[j];
                                        if(combinations[invalid_i0-1].Field == 52 && combinations[invalid_i1-1].Field == 67)
                                        {
                                            printf("./runme_256_set_set_timeout.sh %d %d 180\n", invalid_i0, invalid_i1);
                                        }
                                    }
                                }
                            }
                        }
                    }
		}

		if(solution_found_invalid)
                {
#if 0			
                    printf("c %s nr %ld Found invalid(%d/%d)(F%dC%dR%d/F%dC%dR%d)\n", filename, nrOfSolution, invalid_i0, invalid_i1,
                        combinations[invalid_i0-1].Field, combinations[invalid_i0-1].Card, combinations[invalid_i0-1].Rotation,
			combinations[invalid_i1-1].Field, combinations[invalid_i1-1].Card, combinations[invalid_i1-1].Rotation);
#endif // 0		    
		    totalInvalidSolutions++;
		}
		else
		{
                    nrOfStillValidSolutions++;
		}

		totalSolutions++;
                        
                nrOfUsedIndizes = 0;
                nrOfSolution++;
                break;
            }

            if(var > 0)
            {
                if(nrOfUsedIndizes < 256)
                {
                    // printf("usedIndizes[%ld]=%d\n", nrOfUsedIndizes, var);
                    usedIndizes[nrOfUsedIndizes++] = var;
                }
                else
                {
                    printf("More than 256 placed pieces!?!");
                    exit(1);
                }
            }
        }
        if(fscanf(fpin, "\n") != 0)
        {
            fprintf(stderr, "error: expected: \\n");
            exit(1);
        }
    }

    printf("c %s %ld solutions imported, %ld still valid (validBinaries=%ld,invalidBinaries=%ld,unknownBinaries=%ld)\n", filename, nrOfSolution, nrOfStillValidSolutions, validBinaries, invalidBinaries, unknownBinaries);
    fclose(fpin);
}


int checkAgainstCmsRedFile(char *filename)
{
    int var;
    int rv;
    int clausenumber;
    int resnr;
    int resnrcurrent = 0;
    FILE *fp;

    int nrOfIndizes = 0;

    fp = fopen(filename, "r");

    while(1)
    {
        if(feof(fp))
        {
            return 0;
        }

        while(1)
        {
            rv = fscanf(fp, "%d", &var);
            if(rv == -1)
            {
		printf("exit me\n");
		return 0;
            }
            else if(rv != 1)
            {
                fprintf(stderr, "error: var=%d rv=%d\n", var, rv);
		return 0;
            }

            if(var == 0)
	    {
		// printf("nrOfIndizes=%d\n", nrOfIndizes);
                if(nrOfIndizes == 1 && nrOfUsedIndizes == 1)
                {
                    printf("REDFILE usedIndizes[0]=%ld\n", usedIndizes[0]);
                }
                else if(nrOfIndizes == 2 && nrOfUsedIndizes == 2)
		{
                    // printf("REDFILE usedIndizes[0]=%ld,usedIndizes[1]=%ld\n", usedIndizes[0], usedIndizes[1]);

                    long i;
                    long j;

                    i = usedIndizes[0];
                    j = usedIndizes[1];

                    if(i < j) 
                    {
                        i = usedIndizes[1];
                        j = usedIndizes[0];
                    }

		    if(i < 0 && j < 0)
                    {
                        if(forbiddenCombinations[-j][-i] != INFO_FORBIDDEN_EASY_COMBINATION && forbiddenCombinations[-j][-i] != INFO_FORBIDDEN_COMBINATION)
                        {

#if 0 
                            // if(GetRing(-i) == 0 && GetRing(-j) == 0 && usedCombinationsCounter[-j][-i] == 0)
		            {
			        char name229[1024];
                                struct stat statbuf229;
                                char name256[1024];
                                struct stat statbuf256;
			        char name998[1024];
                                struct stat statbuf998;
                                char name999[1024];
                                struct stat statbuf999;

                                sprintf(name229, "e2_partial_field_229_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", -i, -j);
                                sprintf(name256, "e2_partial_field_256_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", -i, -j);
                                sprintf(name998, "e2_partial_field_998_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", -i, -j);
			        sprintf(name999, "e2_partial_field_999_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", -i, -j);

                                if(stat(name229, &statbuf229) != 0 && stat(name256, &statbuf256) != 0 && stat(name998, &statbuf998) != 0 && stat(name999, &statbuf999) != 0)
#endif // 0
                                {
                                    // printf("./runme_256_set_set_timeout.sh %ld %ld 180 REDFILE\n", -i, -j);
			            // printf("./fixme_999_set_set.sh %ld %ld 180 REDFILE\n", -i, -j);
			            // printf("./fixme_998_set_set.sh %ld %ld 180 REDFILE\n", -i, -j);
				    printf("%ld %ld 0\n", i, j);
			        }
#if 0
		            }
#endif // 0		    

#if 0		    
                            if(usedCombinationsCounter[j][i] > 0)
                            {
                                printf("./runme_256_set_set_timeout.sh %6d %6d 180 REDFILE%03d\n", i, j, usedCombinationsCounter[j][i]);
                            }
#endif // 0		
                        }
		    }
	            else
		    {
                        // printf("%ld %ld 0\n", i, j);
		    }	    
		}

		nrOfIndizes = 0;
                nrOfUsedIndizes = 0;

                break;
            }

            if(var != 0)
            {
                if(nrOfUsedIndizes < 4) // Place for 256, but 4 is enough, we use only max 2
		{
                    // printf("usedIndizes[%ld]=%d\n", nrOfUsedIndizes, var);
	            usedIndizes[nrOfUsedIndizes++] = var;
		}
		else
		{
                    // printf("More than 256 placed pieces!?!");
		    // exit(1);
		}
            }

	    nrOfIndizes++;
        }
        if(fscanf(fp, "\n") != 0)
        {
            fprintf(stderr, "error: expected: \\n");
	    return 0;
        }
        
        resnrcurrent++;
    }
}

int importUnsatFile(char *filename)
{
    int var;
    int rv;
    int clausenumber;
    int resnr;
    int resnrcurrent = 0;
    FILE *fp;

    int nrOfIndizes = 0;

    fp = fopen(filename, "r");

    while(1)
    {
        if(feof(fp))
        {
            return 0;
        }

        while(1)
        {
            rv = fscanf(fp, "%d", &var);
            if(rv == -1)
            {
                printf("exit me\n");
                return 0;
            }
            else if(rv != 1)
            {
                fprintf(stderr, "error: var=%d rv=%d\n", var, rv);
                return 0;
            }

            if(var == 0)
            {
                // printf("REDFILE nrOfIndizes=%d\n", nrOfIndizes);
                if(nrOfIndizes == 2 && nrOfUsedIndizes == 2)
                {
                    // printf("REDFILE resnrcurrent=%d usedIndizes[0]=%ld,usedIndizes[1]=%ld\n", resnrcurrent, usedIndizes[0], usedIndizes[1]);
                    int i;
                    int j;

                    i = usedIndizes[0];
                    j = usedIndizes[1];

                    if(i < j)
                    {
                        i = usedIndizes[1];
                        j = usedIndizes[0];
                    }

                    if(i > NR_OF_SAT_VARIABLES || j > NR_OF_SAT_VARIABLES || i < -NR_OF_SAT_VARIABLES || j < -NR_OF_SAT_VARIABLES)
                    {
                        printf("i=%d, j=%d\n", i, j);
                        exit(1);
                    }

                    if(i < 0 && j < 0)
                    {
                        if(-j > -i)
                        {
                            if(forbiddenCombinations[-j][-i] == INFO_FORBIDDEN_EASY_COMBINATION)
                            {
                            }
                            else if(forbiddenCombinations[-j][-i] != INFO_UNKNOWN_COMBINATION && forbiddenCombinations[-j][-i] != INFO_FORBIDDEN_COMBINATION)
                            {
                                printf("c Clause %d %d -> %d\n", i, j, forbiddenCombinations[-j][-i]);
                            }
                            else
                            {
                                if(forbiddenCombinations[-j][-i] != INFO_FORBIDDEN_COMBINATION)
                                {
                                    forbiddenCombinations[-j][-i] = INFO_FORBIDDEN_COMBINATION;
                                    nrOfForbiddenCombinations++;
                                }
                            }
                        }
                    }
                }

                nrOfIndizes = 0;
                nrOfUsedIndizes = 0;

                break;
            }

            if(var != 0)
            {
                if(nrOfUsedIndizes < 4) // 4 is enough for current work, else NR_OF_FIELDS)
                {
                    // printf("REDFILE usedIndizes[%ld]=%d\n", nrOfUsedIndizes, var);
                    usedIndizes[nrOfUsedIndizes++] = var;
                }
                else
                {
                    // printf("More than 256 placed pieces!?!");
                    // exit(1);
                }
            }

            nrOfIndizes++;
        }
        if(fscanf(fp, "\n") != 0)
        {
            fprintf(stderr, "error: expected: \\n");
            return 0;
        }

        resnrcurrent++;
    }
}


