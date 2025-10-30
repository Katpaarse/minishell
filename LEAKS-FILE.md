valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions minishell.supp ./minishell

PARSING
- cat << EOF
hi 
EOF

EXECUTION
- cat << EOF
hi 
EOF