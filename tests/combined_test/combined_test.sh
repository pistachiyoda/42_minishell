RESET="\033[0m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
WHITE="\033[37m"

cd ../../
make

function exec_test()
{
	# エラー出力の出力先ファイル
	BASH_ERR_OUT=./tests/combined_test/bash_err_out.txt
	MINISHELL_ERR_OUT=./tests/combined_test/minishell_err_out.txt 
	IS_ERR_OUT_DIFF=0

	# bashでコマンド実行
	BASH_OUT=$(echo -e "$@" | bash 2> $BASH_ERR_OUT)
	BASH_STATUS=$(echo $?)

	# minishellでコマンド実行
	MINISHELL_OUT=$(echo -e "$@" | ./minishell 2> $MINISHELL_ERR_OUT | grep -v "minishell$ " ) 
	echo -e "$@ \n" | ./minishell > /dev/null 2>&1
	MINISHELL_STATUS=$(echo $?)

	# エラー出力のtrim処理（先頭部分（実行シェル）の出力が異なるため、その部分をトリム）
	# trimしたものをdiffで比較
	TRIMMED_BASH_ERR_OUT=`cat $BASH_ERR_OUT | cut -d " " -f "4-"`
	TRIMMED_MINISHELL_ERR_OUT=`cat $MINISHELL_ERR_OUT | cut -d " " -f "2-"`
	diff <(echo $TRIMMED_BASH_ERR_OUT) <(echo $TRIMMED_MINISHELL_ERR_OUT)
	if [ $? -eq 1 ]; then 
		IS_ERR_OUT_DIFF=1
	fi

	# 確認用
	# printf "MINISHELL_STATUS = %d\n" "$MINISHELL_STATUS"
	# printf "BASH_STATUS = %d\n" "$BASH_STATUS"
	# printf "MINISHELL_OUT = %s\n" "$MINISHELL_OUT"
	# printf "BASH_OUT = %s\n" "$BASH_OUT"
	# printf "TRIMMED_MINISHELL_ERR_OUT = %s\n" "$TRIMMED_MINISHELL_ERR_OUT"
	# printf "TRIMMED_BASH_ERR_OUT = %s\n" "$TRIMMED_BASH_ERR_OUT"

	# 標準出力、エラー出力、終了ステータスが同じであれば緑色でテストコマンドが出力される
	if [ $IS_ERR_OUT_DIFF -eq 0 ] && [ "$BASH_STATUS" == "$MINISHELL_STATUS" ] && [ "$BASH_OUT" == "$MINISHELL_OUT" ]; then
		printf " $GREEN$@ ✓ \n$RESET"
	else
		printf " $RED$@ ✗ \n$RESET"
	fi

	# 標準出力比較
	if [ "$BASH_OUT" != "$MINISHELL_OUT" ]; then
		printf "\n"
		printf $RED" Your output : \n%.20s\n $RED$MINISHELL_OUT\n%.20s$RESET\n" " -----------------------------------------" " -----------------------------------------"
		printf $YELLOW" Expected output : \n%.20s\n $YELLOW$BASH_OUT\n%.20s$RESET\n" " -----------------------------------------" " -----------------------------------------"
	fi
	# 終了ステータス比較
	if [ "$BASH_STATUS" != "$MINISHELL_STATUS" ]; then
		printf "\n"
		printf $RED" Your exit status : $RED$MINISHELL_STATUS$RESET\n"
		printf $YELLOW" Expected exit status : $YELLOW$BASH_STATUS$RESET\n"
	fi
	# エラー出力比較
	if [ "$TRIMMED_BASH_ERR_OUT" != "$TRIMMED_MINISHELL_ERR_OUT" ]; then
		printf "\n"
		printf $RED" Your output : \n%.20s\n $RED$TRIMMED_MINISHELL_ERR_OUT\n%.20s$RESET\n" " -----------------------------------------" " -----------------------------------------"
		printf $YELLOW" Expected output : \n%.20s\n $YELLOW$TRIMMED_BASH_ERR_OUT\n%.20s$RESET\n" " -----------------------------------------" " -----------------------------------------"
	fi
	
	echo "---------------------------------------------------------------"
}

## BUILTINS
# ECHO
exec_test "echo test"
exec_test "echo           test" 
exec_test "echo test tout"
exec_test "echo test      tout"
# exec_test "echo -n test tout"		    ## 別途テスト関数書く必要あり
# exec_test "echo -n -n -n test tout"	## 別途テスト関数書く必要あり

# CD
exec_test "cd .. \n pwd"
exec_test "cd /Users \n pwd"
exec_test "cd \n pwd"
# exec_test "mkdir test_dir \n cd test_dir \n rm -rf ../test_dir \n cd . \n pwd \n cd . \n pwd \n cd .. \n pwd"

# PWD
exec_test "pwd"
exec_test "pwd test"

# ENV EXPANSIONS
exec_test 'echo $TEST'
exec_test 'echo "$TEST"'
exec_test "echo '$TEST'"
exec_test 'echo "$TEST$TEST$TEST"' # 対応した方が良さそう
exec_test 'echo "$TEST$TEST=lol$TEST"' # ダブルクオート内で未定義変数が複数回出現した場合のハンドリングがうまくできてなさそう
exec_test 'echo "   $TEST lol $TEST"' # これも同じく
exec_test 'echo $TEST$TEST$TEST'
exec_test 'echo    $TEST lol $TEST'
exec_test 'echo test "" test "" test'
exec_test 'echo "$"' # 対応してもいいかも
exec_test 'echo "$?TEST"'
exec_test 'echo $TEST $TEST'
exec_test 'echo "$1TEST"' # 対応してもいいかも
exec_test 'echo "$T1TEST"'

# EXPORT and ENV EXPANSIONS
ENV_SHOW="env | sort | grep -v SHLVL | grep -v _= | grep -v OLDPWD"
EXPORT_SHOW="export | sort | grep -v SHLVL | grep -v _= | grep -v OLDPWD"
exec_test 'export ='
exec_test 'export 1TEST= \n' $ENV_SHOW
exec_test 'export TEST \n' $EXPORT_SHOW
exec_test 'export ""="" \n ' $ENV_SHOW
exec_test 'export TES=T="" \n' $ENV_SHOW
exec_test 'export TEST=LOL \n echo $TEST \n' $ENV_SHOW
exec_test 'export TEST=LOL \n echo $TEST$TEST$TEST=lol$TEST'
exec_test $ENV_SHOW
exec_test $EXPORT_SHOW
exec_test 'export TEST="ls       -l     - a" \n echo $TEST \n $LS \n ' $ENV_SHOW

# EXPORT and UNSET and ENV EXPANSIONS


# PIPE and COMMAND
exec_test 'dog | dog | dog'
exec_test 'cat ./tests/combined_test/hoge.txt | grep hogehoge | cat -e'
exec_test 'echo test | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e'
exec_test 'cat /dev/random | head -c 100 | wc -c'
exec_test 'ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls'
exec_test 'ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls'

# PIPE and COMMAND and FILE

# REDIRECTIONS and COMMAND
exec_test 'echo test > ls \n cat ls'
exec_test 'echo test > ls >> ls >> ls \n echo test >> ls\n cat ls'
exec_test '> lol echo test lol\n cat lol'
exec_test '>lol echo > test>lol>test>>lol>test mdr >lol test >test\n cat test'
exec_test 'cat < ls'
exec_test 'cat < ls > ls'

# REDIRECTIONS and COMMAND and FILE

# MULTI TESTS(PIPE and REIDRECTIONS)
# exec_test 'echo testing multi \n echo "test 1 \n | and 2" \n cat ./tests/combined_test/hoge.txt | grep hogehoge'

# SYNTAX ERROR
# exec_test '\n test'     ## 別途テスト関数書く必要あり
# exec_test '| test'      ## 別途テスト関数書く必要あり
# exec_test 'echo > <'    ## 別途テスト関数書く必要あり
# exec_test 'echo | |'    ## 別途テスト関数書く必要あり
# exec_test '<'           ## 別途テスト関数書く必要あり

# EXIT                     ## 別途テスト関数書く必要あり
# exec_test "exit 42"
# exec_test "exit 42 53 68"
# exec_test "exit 259"
# exec_test "exit 9223372036854775807"
# exec_test "exit -9223372036854775808"
# exec_test "exit 9223372036854775808"
# exec_test "exit -9223372036854775810"
# exec_test "exit -4"
# exec_test "exit wrong"
# exec_test "exit wrong_command"
# exec_test "gdagadgag"
# exec_test "ls -Z"
# exec_test "cd gdhahahad"
# exec_test "ls -la | wtf"

