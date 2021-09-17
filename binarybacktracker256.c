#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "sha256.h"

#include "eter172c.c"

#include "e2_info.c"

// #define RUN_FAST 1
#define CHECK_WITH_256

#if 1
// 256 fields filled
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

unsigned char *forbiddenCombinations[1 + 130180];

unsigned long usedIndizes[256];
unsigned long nrOfUsedIndizes = 0;

unsigned long nrOfForbiddenCombinations = 0;
unsigned long nrOfForbiddenEasyCombinations = 0;
unsigned long nrOfPreviouslyUsedCombinations = 0;

unsigned long fieldMin[256];
unsigned long fieldMax[256];

unsigned long pathWay[256];
unsigned long step = 0;
unsigned long stepMax = 0;

int importSolutions(char *filename);
int GetIndex(int f, int c, int r);
int importUnsatFile(char *filename);

int GetIndex(int f, int c, int r)
{
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        if(combinations[i-1].Field == f && combinations[i-1].Card == c && combinations[i-1].Rotation == r)
        {
            return i;
        }
    }

    return 0;
}

int GetRing(int index)
{
	return(ring[combinations[index-1].Field]);
}


int sha256_test()
{
	BYTE text1[] = {"abc"};
	BYTE text2[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
	BYTE text3[] = {"aaaaaaaaaa"};
	BYTE hash1[SHA256_BLOCK_SIZE] = {0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
	                                 0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
	BYTE hash2[SHA256_BLOCK_SIZE] = {0x24,0x8d,0x6a,0x61,0xd2,0x06,0x38,0xb8,0xe5,0xc0,0x26,0x93,0x0c,0x3e,0x60,0x39,
	                                 0xa3,0x3c,0xe4,0x59,0x64,0xff,0x21,0x67,0xf6,0xec,0xed,0xd4,0x19,0xdb,0x06,0xc1};
	BYTE hash3[SHA256_BLOCK_SIZE] = {0xcd,0xc7,0x6e,0x5c,0x99,0x14,0xfb,0x92,0x81,0xa1,0xc7,0xe2,0x84,0xd7,0x3e,0x67,
	                                 0xf1,0x80,0x9a,0x48,0xa4,0x97,0x20,0x0e,0x04,0x6d,0x39,0xcc,0xc7,0x11,0x2c,0xd0};
	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int idx;
	int pass = 1;

	sha256_init(&ctx);
	sha256_update(&ctx, text1, strlen(text1));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash1, buf, SHA256_BLOCK_SIZE);

	sha256_init(&ctx);
	sha256_update(&ctx, text2, strlen(text2));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash2, buf, SHA256_BLOCK_SIZE);

	sha256_init(&ctx);
	for (idx = 0; idx < 100000; ++idx)
	   sha256_update(&ctx, text3, strlen(text3));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash3, buf, SHA256_BLOCK_SIZE);

	return(pass);
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
    DIR *d;
    struct dirent *dir;

#if 0 
    printf("SHA-256 tests: %s\n", sha256_test() ? "SUCCEEDED" : "FAILED");
    exit(1);
#endif // 0    

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
            else if((combinations[i-1].Field + NR_OF_FIELDS_X) == combinations[j-1].Field && combinations[i-1].PatternSouth != combinations[j-1].PatternNorth);
            else continue;
            forbiddenCombinations[j][i] = INFO_FORBIDDEN_EASY_COMBINATION;
            nrOfForbiddenEasyCombinations++;
        }
    }

    printf("c forbiddenCombinations[4][1]=%d (SOLL %d)\n", forbiddenCombinations[4][1], INFO_FORBIDDEN_EASY_COMBINATION);
    if(forbiddenCombinations[4][1] != INFO_FORBIDDEN_EASY_COMBINATION) exit(1);

    printf("c forbiddenCombinations[5][1]=%d (SOLL %d)\n", forbiddenCombinations[5][1], INFO_UNKNOWN_COMBINATION);
    if(forbiddenCombinations[5][1] != INFO_UNKNOWN_COMBINATION) exit(1);

    printf("c forbiddenCombinations[38][5]=%d (SOLL %d)\n", forbiddenCombinations[38][5], INFO_FORBIDDEN_EASY_COMBINATION);
    if(forbiddenCombinations[38][5] != INFO_FORBIDDEN_EASY_COMBINATION) exit(1);

#if 0 
    importSolutions("e2_new_2019_only_second_inside.res");
    importSolutions("e2_new_2019_only_second_inside_set_24830.res");
    importSolutions("e2_new_2019_only_second_inside_set_24831.res");
    importSolutions("e2_new_2019_only_second_inside_set_24832.res");
    importSolutions("e2_new_2019_only_second_inside_set_24833.res");
    importSolutions("e2_new_2019_only_second_inside_set_24834.res");
    importSolutions("e2_new_2019_only_second_inside_set_24835.res");
    importSolutions("e2_new_2019_only_second_inside_set_24836.res");
    importSolutions("e2_new_2019_only_second_inside_set_24837.res");
    importSolutions("e2_new_2019_only_second_inside_set_24838.res");
    importSolutions("e2_new_2019_only_second_inside_set_24839.res");
    importSolutions("e2_new_2019_only_second_inside_set_24840.res");
    importSolutions("e2_new_2019_only_second_inside_set_24841.res");
    importSolutions("e2_new_2019_only_second_inside_set_24842.res");
    importSolutions("e2_new_2019_only_second_inside_set_24843.res");
    importSolutions("e2_new_2019_only_second_inside_set_24844.res");
    importSolutions("e2_new_2019_only_second_inside_set_24845.res");
    importSolutions("e2_new_2019_only_second_inside_set_24846.res");
    importSolutions("e2_new_2019_only_second_inside_set_24847.res");
    importSolutions("e2_new_2019_only_second_inside_set_24848.res");
    importSolutions("e2_new_2019_only_second_inside_set_24849.res");

    importSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101130728.res");
    importSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101120924.res");
    importSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101110932.res");
    importSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101062023_transred_0_implicitmanip_0.res");
    importSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101061118_transred_0_implicitmanip_0.res");
    importSolutions("e2_new_2019_only_second_inside_with_e2_unsat_202101042256_transred_0_implicitmanip_0.res");
#endif // 1

#if 0    
    importSolutions("e2_partial_field_229_fields_filled_with_filter_4_unsat_5081_run1_set_9_run2.res");
    importSolutions("e2_partial_field_231_fields_filled_with_filter_4_unsat_5081_run1_set_9_run2.res");
    importSolutions("e2_partial_field_233_fields_filled_with_filter_4_unsat_5081_run1_set_9_run2.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_9_run2.res");
    importSolutions("e2_test_partial_229_fields_5081_solutions.res"); 
    importSolutions("e2_partial_field_229_fields_filled_with_unsat_202008151148_and_202008151329.res");
    importSolutions("e2_partial_field_229_fields_filled_with_unsat_202008231948_and_202008151329.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_22_run2.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_23_run2.res");
    importSolutions("e2_partial_field_lower_left_12_square_remaining_filled_edge_and_ring_2_and_ring_4_and_ring_6_completed_resextract_0_filter_4.res");

    importSolutions("e2_partial_field_235_fields_filled_with_e2_unsat_202011080724.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_10_run2_set_39.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_10_run2_set_44.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_10_run2_set_52.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_10_run2_set_53.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_10_run2_set_59.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_10_run2_set_66.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_10_run2_set_73.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_10_run2_set_76.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_11_run2_set_101.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_11_run2_set_103.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_11_run2_set_104.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_11_run2_set_106.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_11_run2_set_107.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_11_run2_set_94.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_11_run2_set_98.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_11_run2_set_99.res");
    importSolutions("e2_partial_field_235_fields_filled_with_filter_4_unsat_5081_run1_set_782_run2_set_1241.res");
#endif // 0

#if 0
    d = opendir(".");
    if (d) 
    {
        while ((dir = readdir(d)) != NULL) 
        {
            // printf("c %s\n", dir->d_name);
            if(string_starts_with(dir->d_name, "e2_partial_field_") &&
               string_starts_with(dir->d_name+20, "_fields_filled_with_filter_4_unsat_5081_run1_set"))
            {
                // printf("c 1\n");
                if(string_ends_with(dir->d_name, ".res"))
                {
                    unsigned long filledFields;
                    unsigned long z1, z2;

                    // printf("c 2\n");

                    int rw = sscanf(dir->d_name, "e2_partial_field_%ld_fields_filled_with_filter_4_unsat_5081_run1_set_%ld_run2_set_%ld.res", &filledFields, &z1, &z2);

                    // printf("c rw=%d filledFields=%ld,z1=%ld,z2=%ld\n", rw, filledFields, z1, z2);

		    if(rw != 3) continue;

		    if(filledFields == 229 && (enabled_fields[combinations[z1-1].Field] == '.' || enabled_fields[combinations[z2-1].Field] == '.'))
                    {
                        printf("./delete229disabledfield_set_set.sh %ld %ld DUMMY\n", z1, z2);
                    } 

                    struct stat statbuf;
                    if(stat(dir->d_name, &statbuf) == 0)
                    {
                        // printf("c # size=%ld\n", statbuf.st_size);

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
                            // importSolutions(dir->d_name);
                        }
			else if(statbuf.st_size == 16)
			{
                            if(forbiddenCombinations[z2][z1] == INFO_UNKNOWN_COMBINATION)
                            {
                                forbiddenCombinations[z2][z1] = INFO_PREVIOUSLY_USED_COMBINATION;
                            }
                            else if(forbiddenCombinations[z2][z1] == INFO_FORBIDDEN_COMBINATION)
			    {
				    // Seems we have another file where it was already marked as forbidden 
                            }
			    else if(forbiddenCombinations[z2][z1] == INFO_PREVIOUSLY_USED_COMBINATION)
                            {
                                    // Perhaps found in a prviously found solution file
                            }
			    else
                            {
                                printf("c Hint: %ld %ld found but not unknown\n", z1, z2);
                            }
			}
                    }
                }
            }
        }
        closedir(d);
    }
#endif // 0

    setvbuf(stdout, (char*)NULL, _IONBF, 0);

    importUnsatFile("e2_found_invalid_clauses_round_1.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_2.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_3.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_4.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_5.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_6.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_7.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_8.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_9.cnf");
    importUnsatFile("e2_unsat_202104151313.cnf");
    importUnsatFile("e2_unsat_unred_202107050830_all_clauses.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_55.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_56.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_57.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_58.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_59.cnf");
    importUnsatFile("e2_unsat_unred_202108011651.cnf");

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
    // Print number of UNSATs per variable 
    unsigned long counter_forbidden_combinations[NR_OF_SAT_VARIABLES + 1];
    unsigned long counter_forbidden_easy_combinations[NR_OF_SAT_VARIABLES + 1];
    unsigned long counter_unknown_combinations[NR_OF_SAT_VARIABLES + 1];
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        counter_forbidden_combinations[i] = 0;
	counter_forbidden_easy_combinations[i] = 0;
	counter_unknown_combinations[i] = 0;
    }
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        for(int j = 1; j <= NR_OF_SAT_VARIABLES; j++)
        {
            if(i == j) continue;
	    if(j > i)
	    {
                if(forbiddenCombinations[j][i] == INFO_FORBIDDEN_COMBINATION)
                {
                    counter_forbidden_combinations[i]++; 
		}
		else if(forbiddenCombinations[j][i] == INFO_FORBIDDEN_EASY_COMBINATION)
                {
                    counter_forbidden_easy_combinations[i]++;
                }
		else if(forbiddenCombinations[j][i] == INFO_UNKNOWN_COMBINATION)
                {
                    counter_unknown_combinations[i]++;
                }
            }
	    else
            {
                if(forbiddenCombinations[i][j] == INFO_FORBIDDEN_COMBINATION)
                {
                    counter_forbidden_combinations[i]++;
                }
		else if(forbiddenCombinations[i][j] == INFO_FORBIDDEN_EASY_COMBINATION)
                {
                    counter_forbidden_easy_combinations[i]++;
                }
                else if(forbiddenCombinations[i][j] == INFO_UNKNOWN_COMBINATION)
                {
                    counter_unknown_combinations[i]++;
                }
            }		    
        }
    }
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        printf("%6d: %6ld %6ld %6ld\n", i, counter_forbidden_combinations[i], counter_forbidden_easy_combinations[i], counter_unknown_combinations[i]);
    }
    exit(1);
#endif // 0

#if 0 
    int shellnr = 1;
    int maxlines = 0;
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        // if(i > 10) continue;

        // int ringi = GetRing(i);

        // if(ringi == 0 || ringi == 1) continue;

        // int fi = combinations[i-1].Field;

        // if(fi != 219) continue;

        for(int j = 1; j < i; j++)
        {
            // int ringj = GetRing(j);

            // if(ringj == 0 || ringj == 1) continue;

            // int fj = combinations[j-1].Field;

            // if(fj < 237) continue;

	    // if((ringi == 0 && ringj == 1) || (ringi == 1 && ringj == 0));
	    // else continue;

	    // if((fi + 1) == fj || (fi + 16) == fj);
	    // else if((fj + 1) == fi || (fj + 16) == fi);
	    // else continue;

            if(forbiddenCombinations[i][j] == INFO_UNKNOWN_COMBINATION)
            {
                printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d UNKNOWN\n", j, i, shellnr);
                shellnr++;
                if(shellnr > (26*3)) shellnr = 1;
		maxlines++;
		if(maxlines > (26*3)*10000) exit(1);
            }
        }
    }
    exit(1);
#endif // 0

#if 0 
    int shellnr = 1;
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        // if(i > 10) continue;

        // int ringi = GetRing(i);

        // if(ringi == 0 || ringi == 1) continue;

        int fi = combinations[i-1].Field;

        // if(fi != 219) continue;

        for(int j = 1; j < i; j++)
        {
            // int ringj = GetRing(j);

            // if(ringj == 0 || ringj == 1) continue;

            int fj = combinations[j-1].Field;

            // if(fj < 237) continue;

            if(forbiddenCombinations[i][j] == INFO_UNKNOWN_COMBINATION)
            {
                printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d UNKNOWN\n", j, i, shellnr);
                shellnr++;
                if(shellnr > (26*3)) shellnr = 1;
            }
        }
    }
    exit(1);
#endif // 0    

#if 0 
    int shellnr = 1;
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        // if(i > 4) continue;

        // int ringi = GetRing(i);

	// if(ringi == 0 || ringi == 1) continue;
	
	int fi = combinations[i-1].Field;

	if(fi != 219) continue;

        for(int j = i + 1; j <= NR_OF_SAT_VARIABLES; j++)
        {
            // int ringj = GetRing(j);

	    // if(ringj == 0 || ringj == 1) continue;
	    
	    int fj = combinations[j-1].Field;

	    if(fj < 237) continue;

            if(forbiddenCombinations[j][i] == INFO_UNKNOWN_COMBINATION)
            {
                printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d UNKNOWN\n", i, j, shellnr);
                shellnr++;
                if(shellnr > 25) shellnr = 1;
            }
	}
    }
    exit(1);
#endif // 0    

#if 0 
    int shellnr = 1;
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        if(i > 4) continue;
        for(int j = i + 1; j <= NR_OF_SAT_VARIABLES; j++)
        {
            if(forbiddenCombinations[j][i] == INFO_PREVIOUSLY_USED_COMBINATION)
            {
                char name256[1024];
                struct stat statbuf256;
                sprintf(name256, "e2_partial_field_256_fields_filled_with_filter_4_unsat_5081_run1_set_%d_run2_set_%d.res", i, j);
                if(stat(name256, &statbuf256) != 0)
                {
                    printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d PREVIOUSLY_USED\n", i, j, shellnr);
                    shellnr++;
                    if(shellnr > 25) shellnr = 1;
                }
            }
        }
    }
    exit(1);
#endif // 0

#if 0 
    // Rotate and perhaps print combination to check 
    int shellnr = 1;
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        int fi0 = combinations[i-1].Field;
        int fix0 = fi0 % NR_OF_FIELDS_X;
        int fiy0 = fi0 / NR_OF_FIELDS_X;
        int fic0 = combinations[i-1].Card;
        int fir0 = combinations[i-1].Rotation;
        int indexi0 = GetIndex(fi0, fic0, fir0);

        int fix1 = NR_OF_FIELDS_X - 1 - fiy0;
        int fiy1 = fix0;
        int fi1  = fiy1 * NR_OF_FIELDS_X + fix1;
        int fic1 = fic0;
        int fir1 = (fir0 + 1) % 4;
        int indexi1 = GetIndex(fi1, fic1, fir1);

        int fix2 = NR_OF_FIELDS_X - 1 - fiy1;
        int fiy2 = fix1;
        int fi2  = fiy2 * NR_OF_FIELDS_X + fix2;
        int fic2 = fic1;
        int fir2 = (fir1 + 1) % 4;
        int indexi2 = GetIndex(fi2, fic2, fir2);

        int fix3 = NR_OF_FIELDS_X - 1 - fiy2;
        int fiy3 = fix2;
        int fi3  = fiy3 * NR_OF_FIELDS_X + fix3;
        int fic3 = fic2;
        int fir3 = (fir2 + 1) % 4;
        int indexi3 = GetIndex(fi3, fic3, fir3);
        int fix4 = NR_OF_FIELDS_X - 1 - fiy3;
        int fiy4 = fix3;
        int fi4  = fiy4 * NR_OF_FIELDS_X + fix4;
        int fic4 = fic3;
        int fir4 = (fir3 + 1) % 4;
        int indexi4 = GetIndex(fi4, fic4, fir4);

        // printf("i) %d/%d/%d/%d/%d(%d) -> %d/%d/%d/%d/%d(%d) -> %d/%d/%d/%d/%d(%d) -> %d/%d/%d/%d/%d(%d) -> %d/%d/%d/%d/%d(%d)\n", fi0, fix0, fiy0, fic0, fir0, indexi0, fi1, fix1, fiy1, fic1, fir1, indexi1, fi2, fix2, fiy2, fic2, fir2, indexi2, fi3, fix3, fiy3, fic3, fir3, indexi3, fi4, fix4, fiy4, fic4, fir4, indexi4);

        for(int j = i + 1; j <= NR_OF_SAT_VARIABLES; j++)
        {
            if(forbiddenCombinations[j][i] == INFO_FORBIDDEN_COMBINATION)
            {
                int fj0 = combinations[j-1].Field;
                int fjx0 = fj0 % NR_OF_FIELDS_X;
                int fjy0 = fj0 / NR_OF_FIELDS_X;
                int fjc0 = combinations[j-1].Card;
                int fjr0 = combinations[j-1].Rotation;
                int indexj0 = GetIndex(fj0, fjc0, fjr0);

                int fjx1 = NR_OF_FIELDS_X - 1 - fjy0;
                int fjy1 = fjx0;
                int fj1  = fjy1 * NR_OF_FIELDS_X + fjx1;
                int fjc1 = fjc0;
                int fjr1 = (fjr0 + 1) % 4;
                int indexj1 = GetIndex(fj1, fjc1, fjr1);
                int fjx2 = NR_OF_FIELDS_X - 1 - fjy1;
                int fjy2 = fjx1;
                int fj2  = fjy2 * NR_OF_FIELDS_X + fjx2;
                int fjc2 = fjc1;
                int fjr2 = (fjr1 + 1) % 4;
                int indexj2 = GetIndex(fj2, fjc2, fjr2);

                int fjx3 = NR_OF_FIELDS_X - 1 - fjy2;
                int fjy3 = fjx2;
                int fj3  = fjy3 * NR_OF_FIELDS_X + fjx3;
                int fjc3 = fjc2;
                int fjr3 = (fjr2 + 1) % 4;
                int indexj3 = GetIndex(fj3, fjc3, fjr3);

                int fjx4 = NR_OF_FIELDS_X - 1 - fjy3;
                int fjy4 = fjx3;
                int fj4  = fjy4 * NR_OF_FIELDS_X + fjx4;
                int fjc4 = fjc3;
                int fjr4 = (fjr3 + 1) % 4;
                int indexj4 = GetIndex(fj4, fjc4, fjr4);

                // printf("j) %d/%d/%d/%d/%d(%d) -> %d/%d/%d/%d/%d(%d) -> %d/%d/%d/%d/%d(%d) -> %d/%d/%d/%d/%d(%d) -> %d/%d/%d/%d/%d(%d)\n", fj0, fjx0, fjy0, fjc0, fjr0, indexj0, fj1, fjx1, fjy1, fjc1, fjr1, indexj1, fj2, fjx2, fjy2, fjc2, fjr2, indexj2, fj3, fjx3, fjy3, fjc3, fjr3, indexj3, fj4, fjx4, fjy4, fjc4, fjr4, indexj4);

                // printf("P1\n");
		if(indexi1 != 0 && indexj1 != 0)
		{
                    if(indexj1 > indexi1)
                    {
                        if(forbiddenCombinations[indexj1][indexi1] == INFO_UNKNOWN_COMBINATION)
	    	        {
			    printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d\n", indexi1, indexj1, shellnr);
                            shellnr++;
                            if(shellnr > 25) shellnr = 1;
		        }
                    }
                    else if(indexj1 < indexi1)
                    {
                        if(forbiddenCombinations[indexi1][indexj1] == INFO_UNKNOWN_COMBINATION)
		        {
                            printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d\n", indexj1, indexi1, shellnr);
                            shellnr++;
                            if(shellnr > 25) shellnr = 1;
		        }
                    }
		}

                // printf("P2\n");

		if(indexi2 != 0 && indexj2 != 0)
		{
                    if(indexj2 > indexi2)
                    {
                        if(forbiddenCombinations[indexj2][indexi2] == INFO_UNKNOWN_COMBINATION)
		        {
                            printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d\n", indexi2, indexj2, shellnr);
                            shellnr++;
                            if(shellnr > 25) shellnr = 1;
		        }
                    }
                    else if(indexj2 < indexi2)
                    {
                        if(forbiddenCombinations[indexi2][indexj2] == INFO_UNKNOWN_COMBINATION)
		        {
                            printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d\n", indexj2, indexi2, shellnr);
                            shellnr++;
                            if(shellnr > 25) shellnr = 1;
		        }
                    }
		}

                // printf("P3\n");
                if(indexi3 != 0 && indexj3 != 0)
		{
                    if(indexj3 > indexi3)
                    {
                        if(forbiddenCombinations[indexj3][indexi3] == INFO_UNKNOWN_COMBINATION)
		        {
                            printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d\n", indexi3, indexj3, shellnr);
                            shellnr++;
                            if(shellnr > 25) shellnr = 1;
		        }
                    }
                    else if(indexj3 < indexi3)
                    {
                        if(forbiddenCombinations[indexi3][indexj3] == INFO_UNKNOWN_COMBINATION)
		        {
                            printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d\n", indexj3, indexi3, shellnr);
                            shellnr++;
                            if(shellnr > 25) shellnr = 1;
		        }
                    }
		}
              // printf("P4\n");

                // exit(1);
            }
        }
        // printf("\n");
    }

    exit(1);
#endif // 0

#if 0 
    // Shift and perhaps print combination to check
    int shellnr = 1;
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
    {
        int fi0 = combinations[i-1].Field;
        int fix0 = fi0 % NR_OF_FIELDS_X;
        int fiy0 = fi0 / NR_OF_FIELDS_X;
        int fic0 = combinations[i-1].Card;
        int fir0 = combinations[i-1].Rotation;
        int indexi0 = GetIndex(fi0, fic0, fir0);

	if(fix0 == 0 || fix0 == (NR_OF_FIELDS_X - 1)) continue;
	if(fiy0 == 0 || fiy0 == (NR_OF_FIELDS_Y - 1)) continue;

        for(int j = i + 1; j <= NR_OF_SAT_VARIABLES; j++)
        {
            if(forbiddenCombinations[j][i] == INFO_FORBIDDEN_COMBINATION)
            {
                int fj0 = combinations[j-1].Field;
                int fjx0 = fj0 % NR_OF_FIELDS_X;
                int fjy0 = fj0 / NR_OF_FIELDS_X;
                int fjc0 = combinations[j-1].Card;
                int fjr0 = combinations[j-1].Rotation;
                int indexj0 = GetIndex(fj0, fjc0, fjr0);

		if(fjx0 == 0 || fjx0 == (NR_OF_FIELDS_X - 1)) continue;
                if(fjy0 == 0 || fjy0 == (NR_OF_FIELDS_Y - 1)) continue;

#if 0 
		if(indexi0 ==         950 && indexj0 ==  24104); // (F17C133R2/F65C69R2)
		else if(indexi0 ==   8806 && indexj0 ==  34359); // (F30C240R3/F78C153R2)
		else if(indexi0 ==  10084 && indexj0 ==  27672); // (F37C151R2/F69C201R1)
		else if(indexi0 ==   8934 && indexj0 ==  25313); // 
		else if(indexi0 ==   1017 && indexj0 ==  25387); // (F17C163R0/F66C202R3) 
		else if(indexi0 ==    661 && indexj0 ==  15043); // (F13C12R2/F44C164R0) 
		else if(indexi0 ==    687 && indexj0 ==  15033); // (F13C38R2/F44C128R2) 
		else if(indexi0 ==    889 && indexj0 ==  24607); // 
		else if(indexi0 ==   1032 && indexj0 ==  15202); // (F17C170R2/F48C40R1)  
		else if(indexi0 ==   1017 && indexj0 ==  24418); //
		else continue;
#endif // 0

#if 1 
                if(indexi0 ==       15167 && indexj0 ==  24995); // (F48C5R1/F66C101R1) 
                else if(indexi0 == 103565 && indexj0 == 105108); // (F187C162R0/F189C172R0)  
                else if(indexi0 ==    618 && indexj0 ==   8611); // (F12C25R2/F30C108R3) 
                else if(indexi0 ==    698 && indexj0 ==   7796); // (F13C49R2/F27C217R1) 
                else continue;
#endif // 0

                for(int offsetfield = 1; offsetfield <= NR_OF_FIELDS - 2 * NR_OF_FIELDS_X; offsetfield++)
                {
                    int fi1 = fi0 + offsetfield;
                    int fix1 = fi1 % NR_OF_FIELDS_X;
                    int fiy1 = fi1 / NR_OF_FIELDS_X;
                    int fic1 = fic0;
                    int fir1 = fir0;
                    int indexi1 = GetIndex(fi1, fic1, fir1);

		    if(indexi1 == 0) continue;

                    if(fix1 == 0 || fix1 == (NR_OF_FIELDS_X - 1)) continue;
                    if(fiy1 == 0 || fiy1 == (NR_OF_FIELDS_Y - 1)) continue;

                    int fj1 = fj0 + offsetfield;
                    int fjx1 = fj1 % NR_OF_FIELDS_X;
                    int fjy1 = fj1 / NR_OF_FIELDS_X;
                    int fjc1 = fjc0;
                    int fjr1 = fjr0;
                    int indexj1 = GetIndex(fj1, fjc1, fjr1);

		    if(indexj1 == 0) continue;

                    if(fjx1 == 0 || fjx1 == (NR_OF_FIELDS_X - 1)) continue;
                    if(fjy1 == 0 || fjy1 == (NR_OF_FIELDS_Y - 1)) continue;

                    if(indexj1 > indexi1)
                    {
                        if(forbiddenCombinations[indexj1][indexi1] == INFO_UNKNOWN_COMBINATION)
                        {
                            printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d\n", indexi1, indexj1, shellnr);
                            shellnr++;
                            if(shellnr > 25) shellnr = 1;
                        }
                    }
                    else if(indexj1 < indexi1)
                    {
                        if(forbiddenCombinations[indexi1][indexj1] == INFO_UNKNOWN_COMBINATION)
                        {
                            printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d\n", indexj1, indexi1, shellnr);
                            shellnr++;
                            if(shellnr > 25) shellnr = 1;
                        }
                    }
                }
            }
        }
    }

    exit(1);
#endif // 0

#if 0 
    int shellnr = 0;
    // Print combination, where we have no info, so second server can do some work in parallel 
    for(int i = 1; i <= 130180; i++)
    {
	// Don't output clauses from disabled fields (e.g. only 229)
        // This would be an error, leads to UNSAT and hard to find!!!
        if(enabled_fields[combinations[i-1].Field] == '.') continue;

        // if(i != 1226) continue;

	int fi = combinations[i-1].Field; 

	if(fi == 252 || fi == 253 || fi == 254);
	else continue;

        for(int j = i + 1; j <= 130180; j++)
        {
            // Don't output clauses from disabled fields (e.g. only 229)
            // This would be an error, leads to UNSAT and hard to find!!!
            if(enabled_fields[combinations[j-1].Field] == '.') continue;

	    int fj = combinations[j-1].Field;

	    if(fj == 252 || fj == 253 || fj == 254);
            else continue;

	    // Same field
	    if(fi == fj) continue;

	    // if((fi + 1) == fj);
#if 0	    
	    else if((fi + 2) == fj);
	    else if((fi + 16) == fj);
	    else if((fi + 16 + 1) == fj);
	    else if((fi + 16 + 2) == fj);
	    else if((fi + 32) == fj);
	    else if((fi + 32 + 1) == fj);
	    else if((fi + 32 + 2) == fj);
#endif // 0	    
	    // else continue;

            if(forbiddenCombinations[j][i] == INFO_UNKNOWN_COMBINATION)
            {
#if 0		    
                printf("./runme_229_set_set.sh %d %d PROCESS%02d\n", i, j, shellnr);
#endif // 0
#if 1 
                printf("./runme_256_set_set_timeout.sh %d %d 180 PROCESS%02d\n", i, j, shellnr);
#endif // 0
                shellnr++;
                if(shellnr > 99) shellnr = 0;
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

#if 1 
    unsigned long nrOfImpossibleClausesOfTwoFields[256][256];
    unsigned long nrOfImpossibleClausesOneField[256];
    memset(nrOfImpossibleClausesOfTwoFields, 0, sizeof(nrOfImpossibleClausesOfTwoFields));
    memset(nrOfImpossibleClausesOneField,    0, sizeof(nrOfImpossibleClausesOneField));
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
                nrOfImpossibleClausesOneField[field_i]++;
                nrOfImpossibleClausesOneField[field_j]++;
            }
        }
    }
    for(int f0 = 0; f0 < 256; f0++)
    {
        printf("Field %d: %ld\n\n", f0, nrOfImpossibleClausesOneField[f0]);
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

		    // Choosing 79 takes too long, currently running 5 instances, the oldest 11 days old
		    // and not finished
		    // Choosing 80 takes too long and not yet finished
		    // Selecting 81 for the next test -> also not better
		    // Selecting 82 for the next test -> also not better
		    // Selecting 83 for the next test
                    if((step+1) < 83)
		    {
                        sprintf(cmd256, "./runme_256_multiset_time.sh %s %ld", filename, 180+2*step);
                    }
		    else
                    {
                        sprintf(cmd256, "./runme_256_multiset_without_time_with_log.sh %s", filename);
                    }
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
    return(0);
}


int importSolutions(char *filename)
{
    int var;
    int rv;
    FILE *fpin;
    unsigned long nrOfSolution = 0;

    printf("c Import solution file %s\n", filename);
   
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
        
        if(fscanf(fpin, "SAT\n") != 0)
        {
            fprintf(stderr, "error: expected: SAT\n");
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
                // 1 solution imported completely
                for(int i = 0; i < nrOfUsedIndizes; i++)
                {
                    for(int j = i + 1; j < nrOfUsedIndizes; j++)
                    {
                        if(forbiddenCombinations[usedIndizes[j]][usedIndizes[i]] == INFO_FORBIDDEN_EASY_COMBINATION)
                        {
                            printf("c Hint: %ld %ld is forbidden easy combination (nrOfSolution=%ld)\n", usedIndizes[i], usedIndizes[j], nrOfSolution);
                        }
                        else if(forbiddenCombinations[usedIndizes[j]][usedIndizes[i]] == INFO_FORBIDDEN_COMBINATION)
                        {
                            printf("c Hint: %ld %ld is forbidden combination (nrOfSolution=%ld)\n", usedIndizes[i], usedIndizes[j], nrOfSolution);
                        }
                        else if(forbiddenCombinations[usedIndizes[j]][usedIndizes[i]] == INFO_UNKNOWN_COMBINATION)
                        {
                            forbiddenCombinations[usedIndizes[j]][usedIndizes[i]] = INFO_PREVIOUSLY_USED_COMBINATION;
                            nrOfPreviouslyUsedCombinations++;
                        }
                    }
                }
                        
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

    printf("c %ld solutions imported\n", nrOfSolution);
    fclose(fpin);
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

    printf("c importUnsatFile %s\n", filename);

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

