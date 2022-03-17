#!/usr/bin/env bash
# minishell実行後、このファイルを実行しfor loopが回っている間に^C, ^D, ^\を押して手動検証
number=0
for number in {1..1000000}; do
    echo ${number}
done
