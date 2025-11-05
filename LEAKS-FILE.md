valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=minishell.supp ./minishell
valgrind --trace-children=yes --leak-check=full --show-leak-kinds=all --suppressions=minishell.supp ./minishell

PARSING
- cat << EOF
hi 
EOF

EXECUTION
- cat << EOF
hi 
EOF


---
~/Codam/r3/minishell $: valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=minishell.supp ./minishell
==171832== Memcheck, a memory error detector
==171832== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==171832== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==171832== Command: ./minishell
==171832== 
minishell > cat << EOF
> hi
> there
> EOF
==172876== 
==172876== HEAP SUMMARY:
==172876==     in use at exit: 214,433 bytes in 377 blocks
==172876==   total heap usage: 643 allocs, 266 frees, 241,888 bytes allocated
==172876== 
==172876== 4 bytes in 1 blocks are still reachable in loss record 3 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x406CB0: ft_strdup (in /home/lavan-de/Codam/r3/minishell/minishell)
==172876==    by 0x405409: tokens_into_cmds (parser.c:142)
==172876==    by 0x405004: main (main.c:78)
==172876== 
==172876== 4 bytes in 1 blocks are still reachable in loss record 4 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x4057DD: remove_quotes (process_token.c:27)
==172876==    by 0x4055B0: handle_heredoc_token (parser.c:32)
==172876==    by 0x40526A: tokens_into_cmds (parser.c:110)
==172876==    by 0x405004: main (main.c:78)
==172876== 
==172876== 11 bytes in 3 blocks are still reachable in loss record 9 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x4057DD: remove_quotes (process_token.c:27)
==172876==    by 0x4059BC: process_token (process_token.c:67)
==172876==    by 0x404FB9: main (main.c:72)
==172876== 
==172876== 16 bytes in 1 blocks are still reachable in loss record 16 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x4041D1: add_argument (cmd_utils.c:40)
==172876==    by 0x405415: tokens_into_cmds (parser.c:142)
==172876==    by 0x405004: main (main.c:78)
==172876== 
==172876== 24 bytes in 1 blocks are still reachable in loss record 20 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x4040F1: cmd_into_new_node (cmd_utils.c:20)
==172876==    by 0x40516A: tokens_into_cmds (parser.c:93)
==172876==    by 0x405004: main (main.c:78)
==172876== 
==172876== 25 bytes in 1 blocks are still reachable in loss record 21 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x40735A: ft_strjoin (in /home/lavan-de/Codam/r3/minishell/minishell)
==172876==    by 0x404E21: make_tmp_heredoc_filename (heredoc.c:47)
==172876==    by 0x404C20: process_heredoc (heredoc.c:99)
==172876==    by 0x4055C7: handle_heredoc_token (parser.c:33)
==172876==    by 0x40526A: tokens_into_cmds (parser.c:110)
==172876==    by 0x405004: main (main.c:78)
==172876== 
==172876== 32 bytes in 1 blocks are still reachable in loss record 23 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x405DF9: tokenize_input (tokenizer.c:73)
==172876==    by 0x404F66: main (main.c:55)
==172876== 
==172876== 568 bytes in 1 blocks are still reachable in loss record 34 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x4064A4: copy_envp (utils.c:38)
==172876==    by 0x404E76: main (main.c:26)
==172876== 
==172876== 568 bytes in 1 blocks are still reachable in loss record 35 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x4064E8: copy_envp (utils.c:44)
==172876==    by 0x404E76: main (main.c:26)
==172876== 
==172876== 2,480 bytes in 70 blocks are still reachable in loss record 52 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x406CB0: ft_strdup (in /home/lavan-de/Codam/r3/minishell/minishell)
==172876==    by 0x40655E: copy_envp (utils.c:57)
==172876==    by 0x404E76: main (main.c:26)
==172876== 
==172876== 2,480 bytes in 70 blocks are still reachable in loss record 53 of 78
==172876==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==172876==    by 0x406CB0: ft_strdup (in /home/lavan-de/Codam/r3/minishell/minishell)
==172876==    by 0x4065CB: copy_envp (utils.c:67)
==172876==    by 0x404E76: main (main.c:26)
==172876== 
==172876== LEAK SUMMARY:
==172876==    definitely lost: 0 bytes in 0 blocks
==172876==    indirectly lost: 0 bytes in 0 blocks
==172876==      possibly lost: 0 bytes in 0 blocks
==172876==    still reachable: 6,212 bytes in 151 blocks
==172876==         suppressed: 208,221 bytes in 226 blocks
==172876== 
==172876== For lists of detected and suppressed errors, rerun with: -s
==172876== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
hi
there
---