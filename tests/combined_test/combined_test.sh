cd ../../
make
cd tests/combined_test

source ./func.sh

## BUILTINS
source ./builtins_single.sh
## EXTERNAL COMMAND
# source ./external.sh
## PIPE
source ./pipe.sh
## REDIRECT
source ./redirect.sh
## ENV EXPANSION
# source ./tests/combined_test/env_expansion.sh
## COMBINED
# source ./tests/combined_test/combined.sh
## SYNTAX
# source ./tests/combined_test/syntax.sh

# MULTI TESTS(PIPE and REIDRECTIONS)
# exec_test 'echo testing multi \n echo "test 1 \n | and 2" \n cat ./tests/combined_test/hoge.txt | grep hogehoge'
