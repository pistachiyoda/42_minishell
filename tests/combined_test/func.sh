RESET="\033[0m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
WHITE="\033[37m"

# $1 TEST_COMMAND
# $2 EXPECTED_TEXT
# $3 ACTUAL_TEXT
# $4 EXPECTED_STATUS
# $5 ACTUAL_STATUS
compare_result() {
	TEST_COMMAND=$1
	EXPECTED_TEXT=$2
	ACTUAL_TEXT=$3
	EXPECTED_STATUS=$4
	ACTUAL_STATUS=$5

	# 確認用
	# printf "EXPECTED_TEXT = %s\n" "$EXPECTED_TEXT"
	# printf "ACTUAL_TEXT = %s\n" "$ACTUAL_TEXT"
	# printf "EXPECTED_STATUS = %d\n" "$EXPECTED_STATUS"
	# printf "ACTUAL_STATUS = %d\n" "$ACTUAL_STATUS"

	if [ "$EXPECTED_STATUS" == "$ACTUAL_STATUS" ] && [ "$EXPECTED_TEXT" == "$ACTUAL_TEXT" ]; then
		printf " $GREEN$1 ✓ \n$RESET"
	else
		printf " $RED$1 ✗ \n$RESET"
	fi

	# 標準出力比較
	if [ "$EXPECTED_TEXT" != "$ACTUAL_TEXT" ]; then
		printf "\n"
		printf $RED" Your output : \n%.20s\n $RED$ACTUAL_TEXT\n%.20s$RESET\n" " -----------------------------------------" " -----------------------------------------"
		printf $YELLOW" Expected output : \n%.20s\n $YELLOW$EXPECTED_TEXT\n%.20s$RESET\n" " -----------------------------------------" " -----------------------------------------"
	fi

	# 終了ステータス比較
	if [ "$EXPECTED_STATUS" != "$ACTUAL_STATUS" ]; then
		printf "\n"
		printf $RED" Your exit status : $RED$ACTUAL_STATUS$RESET\n"
		printf $YELLOW" Expected exit status : $YELLOW$EXPECTED_STATUS$RESET\n"
	fi
	
	echo "---------------------------------------------------------------"
}

exec_test ()
{
	# bashでコマンド実行
	BASH_OUT=$(echo -e "$@" | bash 2> /dev/null)
	BASH_STATUS=$(echo $?)

	# minishellでコマンド実行
	MINISHELL_OUT=$(echo -e "$@" | ../../minishell 2> /dev/null | grep -v "^minishell$ " ) 
	echo -e "$@ \n" | ../../minishell > /dev/null 2>&1
	MINISHELL_STATUS=$(echo $?)

	# 標準出力、エラー出力、終了ステータスが同じであれば緑色でテストコマンドが出力される
	compare_result "$@" "$BASH_OUT" "$MINISHELL_OUT" $BASH_STATUS $MINISHELL_STATUS
}

exec_test_error () {
	# bashでコマンド実行
	BASH_ERR_OUT=$(echo -e "$@" | bash 2>&1)
	BASH_STATUS=$(echo $?)

	# minishellでコマンド実行
	MINISHELL_ERR_OUT=$(echo -e "$@" | ../../minishell 2>&1 | grep -v "^minishell$ " ) 
	echo -e "$@ \n" | ../../minishell > /dev/null 2>&1
	MINISHELL_STATUS=$(echo $?)

	# エラー出力のtrim処理（先頭部分（実行シェル）の出力が異なるため、その部分をトリム）
	# trimしたものをdiffで比較
	TRIMMED_BASH_ERR_OUT=`echo "$BASH_ERR_OUT" | cut -d " " -f "4-"`
	TRIMMED_MINISHELL_ERR_OUT=`echo "$MINISHELL_ERR_OUT" | cut -d " " -f "2-"`

	# 標準出力、エラー出力、終了ステータスが同じであれば緑色でテストコマンドが出力される
	compare_result "$@" "$TRIMMED_BASH_ERR_OUT" "$TRIMMED_MINISHELL_ERR_OUT" $BASH_STATUS $MINISHELL_STATUS
}

exec_test_error_single_line () {
	# bashでコマンド実行
	echo -e "$@" | bash > /dev/null 2>&1
	BASH_STATUS=$(echo $?)
	BASH_ERR_OUT=$(echo -e "$@" | bash 2>&1 | head -n 1)

	# minishellでコマンド実行
	MINISHELL_ERR_OUT=$(echo -e "$@" | ../../minishell 2>&1 | grep -v "^minishell$ " ) 
	echo -e "$@ \n" | ../../minishell > /dev/null 2>&1
	MINISHELL_STATUS=$(echo $?)

	# エラー出力のtrim処理（先頭部分（実行シェル）の出力が異なるため、その部分をトリム）
	# trimしたものをdiffで比較
	TRIMMED_BASH_ERR_OUT=`echo -e "$BASH_ERR_OUT" | cut -d " " -f "4-"`
	TRIMMED_MINISHELL_ERR_OUT=`echo -e "$MINISHELL_ERR_OUT" | cut -d " " -f "2-"`

	# 標準出力、エラー出力、終了ステータスが同じであれば緑色でテストコマンドが出力される
	compare_result "$@" "$TRIMMED_BASH_ERR_OUT" "$TRIMMED_MINISHELL_ERR_OUT" $BASH_STATUS $MINISHELL_STATUS

}

exec_test_with_expected_text () {
	TEST_COMMAND=$1
	EXPECTED_TEXT=$2

	# bashでコマンド実行
	echo -e "$TEST_COMMAND" | bash 2>&1 /dev/null
	BASH_STATUS=$(echo $?)

	# minishellでコマンド実行
	MINISHELL_OUT=$(echo -e "$TEST_COMMAND" | ../../minishell 2> /dev/null | grep -v "^minishell$ " ) 
	echo -e "$TEST_COMMAND \n" | ../../minishell > /dev/null 2>&1
	MINISHELL_STATUS=$(echo $?)

	compare_result "$TEST_COMMAND" "$EXPECTED_TEXT" "$MINISHELL_OUT" $BASH_STATUS $MINISHELL_STATUS
}

exec_error_test_with_expected_text () {
	TEST_COMMAND=$1
	EXPECTED_TEXT=$2

	# bashでコマンド実行
	echo -e "$TEST_COMMAND" | bash 2> /dev/null
	BASH_STATUS=$(echo $?)

	# minishellでコマンド実行
	MINISHELL_OUT=$(echo -e "$TEST_COMMAND" | ../../minishell 2>&1 | grep -v "^minishell$ " ) 
	echo -e "$TEST_COMMAND \n" | ../../minishell > /dev/null 2>&1
	MINISHELL_STATUS=$(echo $?)

	compare_result "$TEST_COMMAND" "$EXPECTED_TEXT" "$MINISHELL_OUT" $BASH_STATUS $MINISHELL_STATUS
}

exec_test_with_redirect () {
	TEST_COMMAND=$1
	FILE_NAME=$2

	# bashでコマンド実行
	echo -e "$TEST_COMMAND" | bash
	BASH_STATUS=$(echo $?)

	mv $FILE_NAME "expected_$FILE_NAME"

	# minishellでコマンド実行
	echo -e "$TEST_COMMAND" | ../../minishell
	MINISHELL_STATUS=$(echo $?)

	mv $FILE_NAME "actual_$FILE_NAME"

	EXPECTED_TEXT=`cat expected_$FILE_NAME`
	ACTUAL_TEXT=`cat actual_$FILE_NAME`

	compare_result "$TEST_COMMAND" "$EXPECTED_TEXT" "$MINISHELL_OUT" $BASH_STATUS $MINISHELL_STATUS
}
