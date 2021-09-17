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

#if ENABLE_ETERNITY2 
#include "e2_info.c"
#endif 
#if ENABLE_TESTBOARD_10X10 
#include "10x10_info.c"
#endif 

// #define RUN_FAST 1
#define CHECK_WITH_256

unsigned char ring[] =
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 2, 2, 2, 2, 2, 2, 1, 0,
  0, 1, 2, 3, 3, 3, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 3, 3, 3, 2, 1, 0,
  0, 1, 2, 2, 2, 2, 2, 2, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

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

unsigned char *usedCombinationsCounter[1 + NR_OF_SAT_VARIABLES];

unsigned long totalSolutions = 0;
unsigned long totalInvalidSolutions = 0;

unsigned long usedIndizes[NR_OF_FIELDS];
unsigned long nrOfUsedIndizes = 0;

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

    printf("c Memory initialized\n");
    
    setvbuf(stdout, (char*)NULL, _IONBF, 0);

#if ENABLE_TESTBOARD_10X10
    importUnsatFile("10x10_found_invalid_clauses.cnf");
    importUnsatFile("10x10_unsat_unred_202107132313.cnf");
#endif // ENABLE_TESTBOARD_10X10

#if 0
#if ENABLE_ETERNITY2 
    // marie@marieubuntu:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_1.cnf
    // -rw-rw-r-- 1 marie marie 868316936 Jul 25 16:42 e2_found_invalid_clauses_round_1.cnf
    // marie@marieubuntu:~/cnf_2019$ cat e2_found_invalid_clauses_round_1.cnf | wc -l
    // 53194585
    // marie@marieubuntu:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_1.cnf
    // b03f865e24f6411858327b03d8067d077b64d9adfefab0f25f99fb36a677d07a  e2_found_invalid_clauses_round_1.cnf
    importUnsatFile("e2_found_invalid_clauses_round_1.cnf");
    // marie@m4047:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_2.cnf
    // -rw-rw-r-- 1 marie marie 95572153 Jul 26 06:27 e2_found_invalid_clauses_round_2.cnf
    // marie@m4047:~/cnf_2019$ cat e2_found_invalid_clauses_round_2.cnf | wc -l
    // 5900527
    // marie@m4047:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_2.cnf
    // 0be2346bd667232be785231e6c7dd0ddcfe9c36c64f55684d0d4d2dce904636e  e2_found_invalid_clauses_round_2.cnf
    importUnsatFile("e2_found_invalid_clauses_round_2.cnf");
    // marie@m4047:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_3.cnf
    // -rw-rw-r-- 1 marie marie 101484518 Jul 26 13:54 e2_found_invalid_clauses_round_3.cnf
    // marie@m4047:~/cnf_2019$ cat e2_found_invalid_clauses_round_3.cnf | wc -l
    // 6270058
    // marie@m4047:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_3.cnf
    // 88422b54884cd1ed2dab42efcf21ab0c2ceb75a8bf3fb62c0afa15cea738cadd  e2_found_invalid_clauses_round_3.cnf
    importUnsatFile("e2_found_invalid_clauses_round_3.cnf");
    // marie@m4047:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_4.cnf
    // -rw-rw-r-- 1 marie marie 12151231 Jul 26 21:08 e2_found_invalid_clauses_round_4.cnf
    // marie@m4047:~/cnf_2019$ cat e2_found_invalid_clauses_round_4.cnf | wc -l
    // 753659
    // marie@m4047:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_4.cnf
    // 012a6c1eb0d42815f0d21e9fa2bc22dd1048d8ead3db077ffd311c433f6c48e7  e2_found_invalid_clauses_round_4.cnf
    importUnsatFile("e2_found_invalid_clauses_round_4.cnf");
    // marie@m4047:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_5.cnf
    // -rw-rw-r-- 1 marie marie 3053975 Jul 27 06:18 e2_found_invalid_clauses_round_5.cnf
    // marie@m4047:~/cnf_2019$ cat e2_found_invalid_clauses_round_5.cnf | wc -l
    // 189946
    // marie@m4047:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_5.cnf
    // ccc9728948c5cbe4abf52d2b7303b1a91bc42befbb03ce172235ff357c57e914  e2_found_invalid_clauses_round_5.cnf
    importUnsatFile("e2_found_invalid_clauses_round_5.cnf");
    // marie@m4047:~/cnf_2019$ cat e2_found_invalid_clauses_round_6.cnf | wc -l
    // 26583
    // marie@m4047:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_6.cnf
    // -rw-rw-r-- 1 marie marie 422448 Jul 27 14:46 e2_found_invalid_clauses_round_6.cnf
    // marie@m4047:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_6.cnf
    // 9893805459cb18040e111df0df5233a9373e57022edb1c0188e5a9cda0e6e152  e2_found_invalid_clauses_round_6.cnf
    importUnsatFile("e2_found_invalid_clauses_round_6.cnf");
    // marie@m4047:~/cnf_2019$ cat e2_found_invalid_clauses_round_7.cnf | wc -l
    // 5813
    // marie@m4047:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_7.cnf
    // -rw-rw-r-- 1 marie marie 99045 Jul 27 22:19 e2_found_invalid_clauses_round_7.cnf
    // marie@m4047:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_7.cnf
    // ddd1ceb6859fd688ac677baf9789a72095374f6f1e439e6e58cd2d166448d2a3  e2_found_invalid_clauses_round_7.cnf
    importUnsatFile("e2_found_invalid_clauses_round_7.cnf");
    // marie@m4047:~/cnf_2019$ cat e2_found_invalid_clauses_round_8.cnf | wc -l
    // 665
    // marie@m4047:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_8.cnf
    // -rw-rw-r-- 1 marie marie 11427 Jul 28 06:47 e2_found_invalid_clauses_round_8.cnf
    // marie@m4047:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_8.cnf
    // 15c91ba78a3ad0729d259784750aa2d76e09d666e01c08e086f72106f4be5a65  e2_found_invalid_clauses_round_8.cnf
    importUnsatFile("e2_found_invalid_clauses_round_8.cnf");
    // marie@m4047:~/cnf_2019$ cat e2_found_invalid_clauses_round_9.cnf | wc -l
    // 72
    // marie@m4047:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_9.cnf
    // -rw-rw-r-- 1 marie marie 1074 Jul 28 14:28 e2_found_invalid_clauses_round_9.cnf
    // marie@m4047:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_9.cnf
    // e6306cd41db1f64491cb948397c9db958c9e1bf2e5b0a4df93eaf03b381b0e39  e2_found_invalid_clauses_round_9.cnf
    importUnsatFile("e2_found_invalid_clauses_round_9.cnf");
    // Round 10 has not found new invalid clauses!!! So it seems we have all of them...

    printf("c Before %ld forbidden combinations (FILTERSTAT)\n", nrOfForbiddenCombinations);
    importUnsatFile("e2_unsat_202104151313.cnf");
    printf("c (1) %ld forbidden combinations (FILTERSTAT)\n", nrOfForbiddenCombinations);
    importUnsatFile("e2_unsat_unred_202107050830_all_clauses.cnf");
    printf("c (2) %ld forbidden combinations (FILTERSTAT)\n", nrOfForbiddenCombinations);

    // marie@marieubuntu:~/cnf_2019$ cat e2_found_invalid_clauses_round_55.cnf | wc -l
    // 53673
    // marie@marieubuntu:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_55.cnf
    // -rw-rw-r-- 1 marie marie 841542 Jul 29 05:57 e2_found_invalid_clauses_round_55.cnf
    // marie@marieubuntu:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_55.cnf
    // b4b16f82ee6059a6a71356dae3671b2ace82a2d43126dee03ebe6d17d114a8bb  e2_found_invalid_clauses_round_55.cnf
    importUnsatFile("e2_found_invalid_clauses_round_55.cnf");
    // marie@marieubuntu:~/cnf_2019$ cat e2_found_invalid_clauses_round_56.cnf | wc -l
    // 8705
    // marie@marieubuntu:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_56.cnf
    // -rw-rw-r-- 1 marie marie 137130 Jul 30 05:28 e2_found_invalid_clauses_round_56.cnf
    // marie@marieubuntu:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_56.cnf
    // cd485aeb886d998fc24681540bad0ef87ac0d1611aeb002b289ad4c1ca433fe6  e2_found_invalid_clauses_round_56.cnf
    importUnsatFile("e2_found_invalid_clauses_round_56.cnf");
    // marie@marieubuntu:~/cnf_2019$ cat e2_found_invalid_clauses_round_57.cnf | wc -l
    // 571
    // marie@marieubuntu:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_57.cnf
    // -rw-rw-r-- 1 marie marie 8955 Jul 30 17:13 e2_found_invalid_clauses_round_57.cnf
    // marie@marieubuntu:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_57.cnf
    // 13c2585d141b80733c3554037ec03ce8edbc5d37aacd36fc8b23e58a046faedc  e2_found_invalid_clauses_round_57.cnf
    importUnsatFile("e2_found_invalid_clauses_round_57.cnf");
    // marie@marieubuntu:~/cnf_2019$ cat e2_found_invalid_clauses_round_58.cnf | wc -l
    // 26
    // marie@marieubuntu:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_58.cnf
    // -rw-rw-r-- 1 marie marie 405 Jul 31 07:40 e2_found_invalid_clauses_round_58.cnf
    // marie@marieubuntu:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_58.cnf
    // 0742d2c81f1f262c013ef6291c611427d752865a997ff9e86289b510b8a4ebad  e2_found_invalid_clauses_round_58.cnf
    importUnsatFile("e2_found_invalid_clauses_round_58.cnf");
    // marie@marieubuntu:~/cnf_2019$ cat e2_found_invalid_clauses_round_59.cnf | wc -l
    // 1
    // marie@marieubuntu:~/cnf_2019$ ls -la e2_found_invalid_clauses_round_59.cnf
    // -rw-rw-r-- 1 marie marie 14 Jul 31 21:33 e2_found_invalid_clauses_round_59.cnf
    // marie@marieubuntu:~/cnf_2019$ sha256sum e2_found_invalid_clauses_round_59.cnf
    // afe9681f550cb364d0b009812471534a1282536d15ba796116a08df42adeebd4  e2_found_invalid_clauses_round_59.cnf
    importUnsatFile("e2_found_invalid_clauses_round_59.cnf");
    // Round 60 has not found new invalid clauses!!! So it seems we have all of them...
#endif // ENABLE_ETERNITY2
#endif // 0

    importUnsatFile("e2_found_invalid_clauses_round_1.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_2.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_3.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_4.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_5.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_6.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_7.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_8.cnf");
    importUnsatFile("e2_found_invalid_clauses_round_9.cnf");
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

    printf("c Final %ld forbidden easy combinations (FILTERSTAT)\n", nrOfForbiddenEasyCombinations);
    printf("c Final %ld forbidden combinations (FILTERSTAT)\n", nrOfForbiddenCombinations);
    printf("c Final %ld previous used combinations (FILTERSTAT)\n", nrOfPreviouslyUsedCombinations);

#if defined LOOPSTART && defined LOOPEND
    for(int i = LOOPSTART; i < LOOPEND && i <= NR_OF_SAT_VARIABLES; i++)
#else
    for(int i = 1; i <= NR_OF_SAT_VARIABLES; i++)
#endif
    {
        // if(i != 1) continue;

        int fi = combinations[i-1].Field;
        int ci = combinations[i-1].Card;

        for(int j = i + 1; j < NR_OF_SAT_VARIABLES; j++)
        {
            // if(j != 42) continue;

            int fj = combinations[j-1].Field;

            // if(fj != 2) continue;

            if(fi == fj) continue;

            int cj = combinations[j-1].Card;

            if(ci == cj) continue;            

            if(forbiddenCombinations[j][i] == INFO_FORBIDDEN_EASY_COMBINATION) continue;
            if(forbiddenCombinations[j][i] == INFO_FORBIDDEN_COMBINATION) continue;

            for(int fieldToCheck = 0; fieldToCheck < NR_OF_FIELDS; fieldToCheck++)
            { 
                // printf("c fieldToCheck=%d\n", fieldToCheck);

                if(fi == fieldToCheck) continue;
                if(fj == fieldToCheck) continue;

                int foundOne = 0;

                for(int k = fieldMin[fieldToCheck]; k <= fieldMax[fieldToCheck]; k++)
                {
                    int fk = combinations[k-1].Field;

                    if(fieldToCheck != fk) continue;

                    int ck = combinations[k-1].Card;

                    if(ci == ck) continue;
                    if(cj == ck) continue;

                    if(k > i) 
                    { 
                        if(forbiddenCombinations[k][i] == INFO_FORBIDDEN_EASY_COMBINATION) continue;
                        if(forbiddenCombinations[k][i] == INFO_FORBIDDEN_COMBINATION) continue;
                    }
                    else
                    {
                        if(forbiddenCombinations[i][k] == INFO_FORBIDDEN_EASY_COMBINATION) continue;
                        if(forbiddenCombinations[i][k] == INFO_FORBIDDEN_COMBINATION) continue;
                    }

                    if(j > k)
                    {
                        if(forbiddenCombinations[j][k] == INFO_FORBIDDEN_EASY_COMBINATION) continue;
                        if(forbiddenCombinations[j][k] == INFO_FORBIDDEN_COMBINATION) continue;
                    }
                    else
                    {
                        if(forbiddenCombinations[k][j] == INFO_FORBIDDEN_EASY_COMBINATION) continue;
                        if(forbiddenCombinations[k][j] == INFO_FORBIDDEN_COMBINATION) continue;
                    }

                    // printf("c %d %d %d -> %d %d\n", i, k, j, forbiddenCombinations[k][i], forbiddenCombinations[j][k]); 

                    foundOne = 1;

                    break;
                }

                if(!foundOne) 
                {
                    printf("-%d -%d 0\n", i, j);
                    break;
                }
            }
        }
    }

    printf("c Done\n");

    return(0);
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

