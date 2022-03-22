
exec_test_error "unset PATH \n ls"
exec_test_error "unset PATH \n echo hoge | ls | date"
exec_test_error "unset PATH \n ls | date | echo hoge"
exec_test_error "' '"
exec_test_error "''"
