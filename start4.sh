#!/bin/bash

DATETIMECURRENTRUN="202109162148"

UNSAT_FILES="--input=e2_found_invalid_clauses_round_1.cnf --input=e2_found_invalid_clauses_round_2.cnf --input=e2_found_invalid_clauses_round_3.cnf --input=e2_found_invalid_clauses_round_4.cnf --input=e2_found_invalid_clauses_round_5.cnf --input=e2_found_invalid_clauses_round_6.cnf --input=e2_found_invalid_clauses_round_7.cnf --input=e2_found_invalid_clauses_round_8.cnf --input=e2_found_invalid_clauses_round_9.cnf --input=e2_unsat_unred_202108181919.cnf --input=e2_unsat_unred_202108182112.cnf --input=e2_unsat_unred_202108190440.cnf --input=e2_unsat_unred_202108191034.cnf --input=e2_unsat_unred_202108191749.cnf --input=e2_unsat_unred_202108192049.cnf --input=e2_unsat_unred_202108200653.cnf --input=e2_unsat_unred_202108201333.cnf --input=e2_unsat_unred_202108202148.cnf --input=e2_unsat_unred_202108210702.cnf --input=e2_unsat_unred_202108212214.cnf --input=e2_unsat_manually_found_202108221228.cnf --input=e2_unsat_unred_202108220834.cnf --input=e2_unsat_unred_202108221248.cnf --input=e2_unsat_unred_202108221942.cnf --input=e2_unsat_unred_202108230600.cnf --input=e2_unsat_unred_202108231055.cnf --input=e2_unsat_unred_202108231928.cnf --input=e2_unsat_unred_202108240701.cnf --input=e2_unsat_unred_202108251024.cnf --input=e2_unsat_unred_202108252255.cnf --input=e2_found_invalid_clauses_202108251032.cnf --input=e2_unsat_unred_202108260720.cnf --input=e2_found_invalid_clauses_202108260727.cnf --input=e2_unsat_unred_202108262325.cnf --input=e2_unsat_unred_202108270833.cnf --input=e2_unsat_unred_202108281649.cnf --input=e2_unsat_unred_202108291127.cnf --input=e2_unsat_unred_202108292108.cnf --input=e2_unsat_unred_202108300628.cnf --input=e2_found_invalid_clauses_202108300628.cnf --input=e2_unsat_unred_202108302136.cnf --input=e2_unsat_unred_202108310649.cnf --input=e2_unsat_unred_202108312210.cnf --input=e2_unsat_unred_202109010839.cnf --input=e2_unsat_unred_202109012052.cnf --input=e2_unsat_unred_202109020737.cnf --input=e2_unsat_unred_202109022231.cnf --input=e2_unsat_unred_202109031643.cnf --input=e2_unsat_unred_202109041000.cnf --input=e2_found_invalid_clauses_202109041000.cnf --input=e2_unsat_unred_202109050650.cnf --input=e2_unsat_unred_202109051857.cnf --input=e2_unsat_unred_202109060748.cnf --input=e2_unsat_unred_202109062247.cnf --input=e2_unsat_unred_202109071430.cnf --input=e2_unsat_unred_202109080711.cnf --input=e2_unsat_unred_202109082200.cnf --input=e2_unsat_unred_202109090809.cnf --input=e2_unsat_unred_202109092258.cnf --input=e2_unsat_unred_202109102140.cnf --input=e2_unsat_unred_202109111208.cnf --input=e2_unsat_unred_202109120716.cnf --input=e2_unsat_unred_202109122155.cnf --input=e2_unsat_unred_duplicate_runs_202109122215.cnf --input=e2_unsat_unred_202109132136.cnf --input=e2_unsat_unred_202109150724.cnf"

# nohup ../cryptominisat-20210418/build/cryptominisat5 --printsol=0 --dumpres=e2_new_2019_$DATETIMECURRENTRUN.res --input=e2_new_2019_sort_uniq.cnf $UNSAT_FILES --dumpredmaxlen=2 --dumpred="e2_new_2019_$DATETIMECURRENTRUN""_red.cnf" > e2_new_2019_$DATETIMECURRENTRUN.log &

# nohup ../cryptominisat-20210418/build/cryptominisat5 --printsol=0 --dumpres=e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_$DATETIMECURRENTRUN.res --input=e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6.cnf $UNSAT_FILES --dumpredmaxlen=2 --dumpred="e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_$DATETIMECURRENTRUN""_red.cnf" > e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_and_ring_6_$DATETIMECURRENTRUN.log &

# nohup ../cryptominisat-20210418/build/cryptominisat5 --printsol=0 --dumpres=e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_$DATETIMECURRENTRUN.res --input=e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5.cnf $UNSAT_FILES --dumpredmaxlen=2 --dumpred="e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_$DATETIMECURRENTRUN""_red.cnf" > e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_and_ring_5_$DATETIMECURRENTRUN.log &

# nohup ../cryptominisat-20210418/build/cryptominisat5 --printsol=0 --dumpres=e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_$DATETIMECURRENTRUN.res --input=e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_sort_uniq.cnf $UNSAT_FILES --dumpredmaxlen=2 --dumpred="e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_$DATETIMECURRENTRUN""_red.cnf" > e2_new_2021_only_ring_0_and_ring_1_and_ring_2_and_ring_3_and_ring_4_$DATETIMECURRENTRUN.log &

nohup ../cryptominisat-20210418/build/cryptominisat5 --printsol=0 --dumpres=e2_new_2021_four_smaller_holes_$DATETIMECURRENTRUN.res --input=e2_new_2021_four_smaller_holes_sort_uniq.cnf $UNSAT_FILES --dumpredmaxlen=2 --dumpred="e2_new_2021_four_smaller_holes_$DATETIMECURRENTRUN""_red.cnf" > e2_new_2021_four_smaller_holes_$DATETIMECURRENTRUN.log &

