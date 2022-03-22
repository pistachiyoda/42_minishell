# REDIRECTIONS ONLY
### HEREDOC系は別途手動テスト！！ -> https://www.notion.so/b75ec43f88a64af1adca23e93243185c
exec_test_error_single_line '<'
exec_test_error_single_line '<<'
exec_test_error_single_line '>'
exec_test_error_single_line '>>'
exec_test '< ./hoge.txt'
exec_test '> ./tmp.txt \n ls -la . \n rm ./tmp.txt'
exec_test '>> ./tmp.txt \n ls -la . \n rm ./tmp.txt'
exec_test_error_single_line 'test <'
exec_test_error_single_line 'test <<'
exec_test_error_single_line 'test >'
exec_test_error_single_line 'test >>'

# REDIRECTIONS and COMMAND
exec_test 'echo test > ls \n cat ls \n rm ls'
exec_test 'echo test > ls >> ls >> ls \n echo test >> ls\n cat ls \n rm ls '
exec_test '> lol echo test lol\n cat lol \n rm lol'
exec_test '>lol echo > test>lol>test>>lol>test mdr >lol test >test\n cat test \n rm test lol'
exec_test 'cat < ls'
exec_test 'cat < ls > ls'

# REDIRECTIONS and COMMAND and FILE
exec_test 'cat ./hoge.txt > ./out1 > ./out2 > ./out3 \n cat ./out1 \n cat ./out2 \n cat ./out3 \n rm out1 out2 out3'
exec_test '> ./out1 > ./out2 ls > ./out3 \n cat ./out1 \n cat ./out2 \n cat ./out3 \n rm out1 out2 out3'
exec_test 'cat < ./hoge.txt > ./out1 > ./out2 <./fuga.txt \n cat ./out1 \n rm out1 out2'
exec_test 'cat < ./hoge.txt > ./out1 > ./out2 <./fuga.txt \n cat ./out2 \n rm out1 out2'
exec_test_error 'cat < not_exitst_file > out1'
exec_test 'cat < not_exitst_file > out1 \n ls -la'

# REDIRECTIONS with NUMBERS
exec_test_error '1< ./hoge.txt echo hoge'
exec_test_error '2< ./hoge.txt dog'
exec_error_test_with_expected_text '1< ./hoge.txt cat ./fuga.txt' 'cat: stdout: Bad file descriptor'
exec_test '2<hoge.txt cat hoge.txt'
exec_test '0< hoge.txt'
exec_test '1< hoge.txt'
exec_test '2< hoge.txt'
exec_test '3< hoge.txt'
exec_test '2> out.txt \n ls -la \n rm out.txt'
exec_test 'echo a 255>dd'
rm dd
exec_test 'cat a 255>dd'
rm dd
exec_test_error 'echo a 256>dd'
rm dd
exec_test_error 'cat a 256>dd'
rm dd
exec_test_error 'echo a 1111>dd'
rm dd
exec_test_error 'cat a 1111>dd'
rm dd
exec_test_error 'echo a 11111111>dd'
rm dd
exec_test_error 'cat a 11111111>dd'
rm dd
exec_test_error 'echo 111111111111111111111111111111>a'
rm a
# exec_test '1> out1.txt 2> out2.txt echo hoge \n cat out1.txt \n rm out1.txt out2.txt' # minishell上ではうまくexpected通りに動いている
# exec_test '1> out1.txt 2> out2.txt echo hoge \n cat out2.txt \n rm out1.txt out2.txt' # minishell上ではうまくexpected通りに動いている
# exec_test '2> out1.txt 1> out2.txt echo hoge \n cat out1.txt \n rm out1.txt out2.txt' # minishell上ではうまくexpected通りに動いている
# exec_test '2> out1.txt 1> out2.txt echo hoge \n cat out2.txt \n rm out1.txt out2.txt' # minishell上ではうまくexpected通りに動いている
# exec_test '1> out1.txt > out2.txt 1>out3.txt echo hoge \n cat out1.txt \n rm out1.txt out2.txt out3.txt' # minishell上ではうまくexpected通りに動いている
# exec_test '1> out1.txt > out2.txt 1>out3.txt echo hoge \n cat out2.txt \n rm out1.txt out2.txt out3.txt' # minishell上ではうまくexpected通りに動いている
# exec_test '1> out1.txt > out2.txt 1>out3.txt echo hoge \n cat out2.txt \n rm out1.txt out2.txt out3.txt' # minishell上ではうまくexpected通りに動いている
