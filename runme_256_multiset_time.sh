#
# $1 filename of set variable
# $2 timeout in seconds
#
../cryptominisat-20210418/build/cryptominisat5 --maxtime=$2 --dumpres=$1.res --input=$1.cnf --input=e2_new_2019.cnf > /dev/null 

