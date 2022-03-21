## BUILTINS SINGLE

# ECHO
exec_test_error "echo -n $USE"
exec_test "echo test"
exec_test "echo           test"
exec_test "echo test tout"
exec_test "echo test      tout"
exec_test "echo -p"
exec_test_with_expected_text "echo -n test tout" "test toutminishell$ "
exec_test_with_expected_text "echo -n -n -n test tout" "test toutminishell$ "
exec_test_with_expected_text "echo -n -n -n -n   -n     -n -n-n test tout" "-n-n test toutminishell$ "
exec_test_with_expected_text "echo -n -x -n test tout" "-x -n test toutminishell$ "
exec_test_with_expected_text "echo -x -n test tout" "-x -n test tout"
# exec_test 'echo "$$$$$$$"'
# exec_test 'echo "?????"'

# CD
exec_test "cd \n pwd"
exec_test "cd . \n pwd"
exec_test "cd .. \n pwd"
exec_test_error "cd ... \n pwd"
exec_test "cd ../\n pwd"
exec_test "cd ../ \n pwd"
exec_test "cd ../../ \n pwd"
exec_test "cd /Users \n pwd"
exec_test "cd // \n pwd"
exec_test "cd '//' \n pwd"
exec_test "cd ////// \n pwd"
exec_test "cd ./././ \n pwd"
exec_test "cd / \n pwd"
exec_test "cd '/////' 2>/dev/null \n pwd"
# exec_test "cd '/etc' \n pwd"
# exec_test "cd '/var' \n pwd"
exec_test_error "cd "doesntexist" \n pwd"
exec_test "cd "doesntexist" 2>/dev/null \n pwd"
exec_test "cd ../../.. \n pwd"
exec_test "cd "wtf" 2>/dev/null \n pwd"
exec_test_error "cd woof\n pwd"
exec_test_error "cd bark bark\n pwd"
exec_test "cd '/' \n pwd"
exec_test "cd $PWD \n pwd"
exec_test 'echo "$PWD" \n pwd'
exec_test 'cd ../ \n cd "$OLDPWD" \n pwd'

# PWD
exec_test "pwd"
exec_test "pwd test"

# ENV EXPANSIONS
exec_test 'echo $TEST'
exec_test 'echo "$TEST"'
exec_test "echo '$TEST'"
exec_test 'echo "$TEST$TEST$TEST"' 
# exec_test 'echo "$TEST$TEST=lol$TEST"'
exec_test 'echo "   $TEST lol $TEST"'
exec_test 'echo $TEST$TEST$TEST'
exec_test 'echo    $TEST lol $TEST'
exec_test 'echo test "" test "" test'
exec_test 'echo "$"'
exec_test 'echo "$?TEST"'
exec_test 'echo $TEST $TEST'
exec_test 'echo "$T1TEST"'

# EXPORT and ENV EXPANSIONS
ENV_SHOW="env | sort | grep -v SHLVL | grep -v _= | grep -v OLDPWD"
EXPORT_SHOW="export | sort | grep -v SHLVL | grep -v _= | grep -v OLDPWD"
exec_test_error 'export ='
exec_test_error "export 1TEST= \n $ENV_SHOW"
exec_test "export TEST \n $EXPORT_SHOW"
exec_test_error "export \"\"=\"\" \n $ENV_SHOW"
exec_test "export TES=T=\"\" \n $ENV_SHOW"
exec_test "export TEST=LOL \n echo \$TEST \n $ENV_SHOW"
# exec_test 'export TEST=LOL \n echo $TEST$TEST$TEST=lol$TEST'
exec_test "$ENV_SHOW"
exec_test "$EXPORT_SHOW"
exec_test "export TEST\n export TEST\n $EXPORT_SHOW"
exec_test "export TEST=a TEST=b TEST=c\n $EXPORT_SHOW"
exec_test "export TEST=a TEST1=b TEST=c\n $EXPORT_SHOW"
# exec_test_error "export TEST=\"ls       -l     - a\" \n echo \$TEST \n \$LS \n $ENV_SHOW"
exec_test 'echo hoge > "aaa"$a \n cat aaa \n rm aaa'
# exec_test 'export a=aaa \n export b=bbb \n echo hoge > $a" "$b \n ls \n cat "aaa bbb" \n rm "aaa bbb"'

# EXPORT and UNSET and ENV EXPANSIONS
exec_test "export TEST=\"hoge\" \n unset TEST \n $ENV_SHOW"
exec_test "unset TEST \n $ENV_SHOW"
exec_test "unset TEST1 TEST2 TEST3 \n $ENV_SHOW"

# EXIT ## ステータスコードのみ確認。出力結果は手動テストの必要あり！！！！
exec_test "exit 42"
exec_test "exit 42 53 68"
exec_test "exit 259"
exec_test "exit 9223372036854775807"
exec_test "exit -9223372036854775808"
exec_test "exit 9223372036854775808"
exec_test "exit -9223372036854775810"
exec_test "exit -4"
exec_test "exit wrong"
exec_test "exit wrong_command"
