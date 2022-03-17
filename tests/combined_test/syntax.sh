# SYNTAX ERROR
exec_test '\n test'     ## 別途テスト関数書く必要あり
exec_test '| test'      ## 別途テスト関数書く必要あり
exec_test 'echo > <'    ## 別途テスト関数書く必要あり
exec_test 'echo | |'    ## 別途テスト関数書く必要あり
exec_test '<'           ## 別途テスト関数書く必要あり
