
Builtins (you must implement these in your own code)

These are executed inside your minishell process without calling execve():

    echo — print arguments (with optional -n to suppress newline)

    cd — change current directory (chdir() in C)

    pwd — print current working directory

    export — set environment variables

    unset — remove environment variables

    env — list environment variables

    exit — exit the shell

External commands (you do not implement them)

These already exist in your system as executables in /bin, /usr/bin, etc.
You find them using PATH from envp and run them with fork() + execve().

Common examples:

    /bin/ls → list files

    /bin/cat → print file contents

    /usr/bin/grep → search text

    /usr/bin/wc → count lines, words, characters

    /usr/bin/sort → sort text

    /bin/mkdir → make directories

    /bin/rm → remove files

    /bin/date → show system date

    /usr/bin/head, /usr/bin/tail → view start/end of files

    /bin/echo (yes, system has its own version, but your builtin overrides it)