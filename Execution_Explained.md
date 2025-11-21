# Minishell Execution Explained

This document explains the execution logic flow of our minishell, detailing how the parsed `t_cmd` structures are executed.

## 1. High-Level Overview

The execution engine (`executor.c`) takes the `t_cmd` linked list and decides how to run each command.
- **Single Builtin**: Run directly in the parent process (e.g., `cd`, `export`).
- **Pipeline / External**: Run in child processes using `fork` and `execve`.

---

## 2. The Executor Hub (`executor.c`)

### `execute_command(t_minishell *shell)`
This is the main entry point. It makes a high-level decision:

1. **Single Builtin Case**:
   - If there is only **one command** AND it is a **builtin** (checked via `is_builtin`).
   - Call `run_builtin` -> `run_parent_builtin` (for parent-modifying builtins like `cd`, `exit`, `export`, `unset`).
   - **Why?** These commands must change the state of the *current* shell (e.g., change directory). Running them in a child would only change the child's state, which is lost when it exits.

2. **Pipeline / External Case**:
   - If there are multiple commands (pipeline) OR it's a single external command.
   - Call `execute_pipeline` (for pipes) or `run_external` (for single external).
   - **Why?** External commands (`ls`, `grep`) need a new process image via `execve`. Pipelines need parallel processes connected by pipes.

---

## 3. Handling Builtins

### `run_builtin` & `run_parent_builtin`
- **`is_parent_builtin`**: Checks if it's `cd`, `export`, `unset`, `exit`.
- **Redirection in Parent**:
  - Before running a parent builtin, we must handle redirects (e.g., `cd /tmp > log`).
  - `setup_parent_redirects`: Saves current `stdin`/`stdout` using `dup`.
  - Applies the new redirects using `dup2`.
  - Runs the builtin.
  - `restore_parent_redirects`: Restores original `stdin`/`stdout` and closes backups.
  - **Why?** If we didn't restore, the shell's stdout would remain redirected to the file forever.

### `run_child_builtin` (e.g., `echo`, `env`, `pwd`)
- These *can* run in a child (pipeline) or parent, but `executor.c` logic usually routes single instances to `run_builtin`.
- If part of a pipeline, they run in `child_redirects`.

---

## 4. External Commands (`run_external`)

1. **Path Resolution**:
   - Checks if command contains `/` (absolute/relative path).
   - If not, searches `PATH` environment variable using `find_cmd_path` (`external_utils.c`).
   - **Error Handling**:
     - Command not found -> Exit 127.
     - Permission denied / Is a directory -> Exit 126.

2. **Fork & Exec**:
   - Calls `cmd_fork`.
   - **Child Process**:
     - Sets up signals (default behavior).
     - Handles redirections (`handle_redirects`).
     - Calls `execve`.
   - **Parent Process**:
     - Waits for the child to finish (`wait_for_child`).
     - updates `exit_code`.

---

## 5. Pipelines (`pipeline_exec.c` & `pipeline_helpers.c`)

Pipelines (e.g., `ls | grep a | wc -l`) require creating multiple processes connected by pipes.

### Logic Flow:
1. **`execute_pipeline`**:
   - Loops through the `t_cmd` list.
   - Creates a pipe `pipe(fd)` for communication between current and next command.

2. **Forking**:
   - Forks a child for the current command.
   - **Child**:
     - Connects input to the *previous* pipe's read end (if not first).
     - Connects output to the *current* pipe's write end (if not last).
     - Closes unused pipe ends.
     - Executes command (builtin or external).
   - **Parent**:
     - Closes the write end of the current pipe (child has it).
     - Saves the read end to be the "input" for the next command.
     - Moves to next command.

3. **Waiting**:
   - After launching all children, the parent waits for *all* of them to finish.
   - The exit code of the *last* command becomes the shell's exit code.

**Why this way?**
- Closing unused file descriptors in parent and child is critical. If a write end is left open, the reader will never see EOF and hang forever.

---

## 6. Redirections (`redirects_exec.c`)

### `handle_redirects(t_cmd *cmd)`
Iterates through the `cmd->redir` list and applies them in order.

- **Input (`<`)**: Opens file `O_RDONLY`, uses `dup2(fd, STDIN_FILENO)`.
- **Output (`>`)**: Opens file `O_WRONLY | O_CREAT | O_TRUNC`, uses `dup2(fd, STDOUT_FILENO)`.
- **Append (`>>`)**: Opens file `O_WRONLY | O_CREAT | O_APPEND`, uses `dup2(fd, STDOUT_FILENO)`.
- **Heredoc (`<<`)**: The temp file created during parsing is opened as Input (`<`).

**Order Matters**: `ls > out1 > out2` creates empty `out1` and writes to `out2`. We process the list sequentially, so the last redirection "wins" for the FD, but side effects (file creation) happen for all.

---

## Summary of File Responsibilities

- **`executor.c`**: High-level decision making (builtin vs external).
- **`executor_utils.c`**: Helper functions for parent redirects and forking.
- **`pipeline_exec.c`**: Manages piping logic (pipe creation, connecting FDs).
- **`builtin_*.c`**: Implementation of specific builtins.
- **`external_cmd.c`**: Validates and runs external binaries.
- **`redirects_exec.c`**: Applies file redirections using `dup2`.

