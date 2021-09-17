#
# $3 time till timeout
#
echo $1 0 > set_$1.cnf
echo $2 0 > set_$2.cnf
nohup ../cryptominisat-20210418/build/cryptominisat5 --maxtime=$3 --dumpres=e2_partial_field_256_fields_filled_with_filter_4_unsat_5081_run1_set_$1_run2_set_$2.res --input=set_$1.cnf --input=set_$2.cnf --input=e2_new_2019.cnf > /dev/null 

