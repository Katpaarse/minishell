The best way to start is not to dive straight into coding the whole shell, but to break the project down into small, testable pieces.

I’ll walk you through how to start and what order to tackle things in.
1. # Understand What Minishell Must Do

Minishell is basically a simplified Bash:

    Reads a command line from the user (with readline).

    Parses it into commands and arguments.

    Handles pipes (|), redirections (<, >, >>).

    Executes builtins (cd, echo, pwd, export, unset, env, exit) without forking.

    Executes external commands by forking and using execve.

    Manages signals (Ctrl+C, Ctrl+D, Ctrl+) like Bash.

    Manages environment variables.

2. # Suggested Step-by-Step Plan
Step 1: Set up your loop

The core shell loop will:

    Print a prompt.

    Read input using readline.

    Add it to history (add_history).

    Parse and execute the command.

    Repeat until exit.

Test this first before anything else:
```C
int main(int argc, char **argv, char **envp)
{
    char *line;

    (void)argc;
    (void)argv;
    while (1)
    {
        line = readline("minishell$ ");
        if (!line) // Ctrl+D
        {
            printf("exit\n");
            break;
        }
        if (*line)
            add_history(line);
        printf("You typed: %s\n", line); // test
        free(line);
    }
    return (0);
}
```

Step 2: Parse the input

You don’t have to handle pipes and redirections right away.
First:

    Split the input into command + arguments (you can start with strtok or ft_split).

    Store it in your t_cmd struct.

Example:
```C
char **args = ft_split(line, ' '); // simple split (later you'll handle quotes)
```

Once you can parse "ls -l /home" into:

```C
argv[0] = "ls"
argv[1] = "-l"
argv[2] = "/home"
```

… you can move on to execution.
Step 3: Execute simple commands

    For external commands: fork() and execve() with envp.

    For builtins: write separate functions and call them without forking.

Example for external commands:
```C
pid_t pid = fork();
if (pid == 0)
{
    execve(args[0], args, envp);
    perror("execve");
    exit(1);
}
else
    waitpid(pid, NULL, 0);
```

At this point, you should be able to type:
```Shell
minishell$ /bin/ls -l
```

…and get output.
Step 4: Add Builtins

Builtins must be handled in the main process (no fork):

    cd

    pwd

    echo

    export

    unset

    env

    exit

Step 5: Handle Redirections (<, >, >>)

    < file → open file with O_RDONLY, dup2(fd, STDIN_FILENO).

    > file → open with O_WRONLY | O_CREAT | O_TRUNC.

    >> file → open with O_WRONLY | O_CREAT | O_APPEND.

Step 6: Handle Pipes

    Split commands by |.

    Use pipe() and dup2() to connect STDOUT of one process to STDIN of the next.

Step 7: Handle Environment Variables

    Keep a copy of envp inside t_minishell.

    Implement variable expansion: $HOME, $?.

    Implement export and unset.

Step 8: Handle Signals

    Ctrl+C → print newline and reprint prompt.

    Ctrl+D → exit shell.

    Ctrl+\ → ignore or handle like Bash.

3. # About Your Header File

Your header is a good start — you’ll just want to add:
```C
// Function prototypes
void    execute_command(t_minishell *shell, t_cmd *cmd);
t_cmd   *parse_input(char *line);
void    free_cmds(t_cmd *cmd);
void    handle_signals(void);
```

And maybe:
```C
typedef enum e_bool { false, true } t_bool;
```

4. Recommended Starting Order

    Readline + prompt → test reading input.

    Simple parsing (no quotes/pipes yet).

    Simple execution of external commands.

    Builtins.

    Redirections.

    Pipes.

    Environment variables & expansions.

    Signals & final polish.

If you want, I can make you a minimal working Minishell skeleton that compiles and runs external commands so you can start testing right away.
That way, you’d have something to build upon instead of starting from zero.