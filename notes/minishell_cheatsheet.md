# Minishell Cheat Sheet
## Shell Basics

    Shell — A command-line program (like bash or zsh) that reads user input, interprets it, and runs commands.
    In Minishell, you’re building a simplified shell.

    Prompt — The text displayed to the user asking for input (e.g., minishell$). Usually comes from readline().

    Command — A program or instruction to execute.
    Example:

ls -l /home

    Command: ls

    Arguments: -l and /home

Builtin Command — Handled inside the shell without execve().
Must implement:

    cd — change directory

    echo — print text

    pwd — print working directory

    export — set environment variable

    unset — remove environment variable

    env — print environment variables

    exit — exit the shell

External Command — Executed by running an external binary using fork() + execve() (e.g., /bin/ls).

PATH — Environment variable containing directories where executables are searched.
Example:

    PATH=/usr/local/bin:/usr/bin:/bin

    Typing ls makes the shell search each directory for the ls binary.

Environment & Expansion

    Environment Variable — Key-value pairs affecting program execution.
    Example: HOME=/home/user
    Access in C: getenv("HOME").

    Variable Expansion — Replace variables in commands with their values.
    Example: echo $HOME → /home/user.

    Special Variable $? — Exit code of the last command.
    Example: if ls fails, $? might be 2.

Parsing Essentials

    Token — Smallest meaningful chunk after splitting input.
    Example: ls -l | grep ".c" → Tokens: ls, -l, |, grep, .c.

    Quoting — Controls how text is interpreted:

        'single quotes' → literal text (no expansion)

        "double quotes" → allows expansion but keeps spaces

    Escaping — Using \ to prevent special interpretation.
    Example: echo hello\ world → hello world.

    Redirection — Change input/output sources:

        > → stdout to file (overwrite)

        >> → stdout to file (append)

        < → stdin from file

        << → heredoc

    Heredoc — Reads input until a delimiter is found:

    cat << EOF
    Hello
    World
    EOF

    Pipe (|) — Connect output of one command to input of another.
    Example: ls | grep .c

Execution Flow

    Fork — Creates a copy of the process.

        Parent: shell continues running.

        Child: runs the command.

    Execve — Replaces process image with a new program (used in child after fork()).

    Wait / Waitpid — Parent waits for child to finish, collects exit status.

        WIFEXITED(status) → child exited normally.

        WEXITSTATUS(status) → exit code.

    Exit Code — Integer result of a command:

        0 = success

        Non-zero = error

    Signals — Asynchronous notifications:

        SIGINT (Ctrl+C) → interrupt

        SIGQUIT (Ctrl+) → quit with core dump

        EOF (Ctrl+D) → end of input

File Descriptors

    Standard FDs:

        0 → stdin (input)

        1 → stdout (output)

        2 → stderr (errors)

    dup2() — Duplicate file descriptors (used for redirection and pipes).

Internal Shell Architecture

    Lexer — Breaks input into tokens (respects quotes/escapes).

    Parser — Builds structured representation (linked list/tree) from tokens.

    Executor — Runs commands with proper redirection, pipes, and builtins.

Error Handling

    perror() — Prints error message based on errno (essential for debugging).

    Syntax Error — Command structure is invalid (e.g., ls || grep if || not supported).

## REPL Loop

(Read → Eval → Print → Loop)

    Read input (readline()).

    Parse into commands & arguments.

    Execute with builtins or fork/exec.

    Loop until exit.