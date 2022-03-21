cd ../../
make
cd tests/combined_test

source ./func.sh

## BUILTINS
source ./builtins_single.sh
## PIPE
source ./pipe.sh
## REDIRECT
source ./redirect.sh
## SYNTAX
source ./syntax.sh
## OTHERS
source ./others.sh
