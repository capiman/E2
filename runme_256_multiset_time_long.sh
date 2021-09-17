#
# $1 filename of set variable
#
nohup ../cryptominisat-20210418/build/cryptominisat5 --dumpres=$1_long.res --input=$1.cnf --input=e2_new_2019.cnf > $1_long.log & 

