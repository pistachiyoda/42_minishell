# SYNTAX ERROR
exec_test_error_single_line '\n test'  
exec_test_error_single_line '| test'   
exec_test_error_single_line 'echo > <' 
exec_test_error_single_line 'echo | |' 
exec_test_error_single_line '<' 

## ambiguous redirect
exec_test_error_single_line 'export TEST= \n echo test > $TEST'
exec_test_error_single_line 'export HOGE="" \n echo test > $HOGE'
exec_test_error_single_line 'export PIYO=" " \n echo test > $PIYO'
exec_test_error_single_line 'export TEST="file    echo world" \n echo hello > $TEST'
exec_test_error_single_line 'export a="   test" \n echo hoge > "aaa"$a'
exec_test_error_single_line 'export hoge="ho ge" \n echo hoge > $hoge'
