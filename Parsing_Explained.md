# Minishell Parsing Explained

This document explains the parsing logic flow of our minishell, detailing how raw user input is transformed into structured command data.

## 1. High-Level Overview

The parsing process consists of two main stages:
1. **Tokenization (`tokenizer.c`)**: Splitting the raw input string into individual tokens (words, operators).
2. **Command Construction (`tokens_into_cmds.c`)**: Grouping tokens into a linked list of `t_cmd` structures, handling redirections, pipes, and heredocs.

---

## 2. Tokenization (`tokenizer.c` & `tokenizer_utils.c`)

The goal is to split a string like `echo "hello world" > file` into `["echo", "\"hello world\"", ">", "file"]`.

### Logic Flow:
1. **`tokenize_input`**: The entry point.
   - Calls `count_tokens` to allocate the correct size for the token array.
   - Loops through the input string again to extract each token using `extract_token`.

2. **`count_tokens`**:
   - Skips whitespace.
   - Identifies token boundaries:
     - **Special Operators**: `|`, `<`, `>`, `<<`, `>>` are treated as separate tokens.
     - **Quotes**: Everything inside single `'` or double `"` quotes is treated as a single token (preserving spaces).
     - **Normal Words**: Sequences of non-space, non-special characters.

3. **`extract_token`**:
   - Copies the substring corresponding to the identified token.
   - Uses helper functions like `skip_quoted_input` and `skip_special_operator` to determine the end of the token.

**Why this way?**
- We tokenize *before* expanding variables or removing quotes to ensure that spaces inside quotes don't split arguments (e.g., `echo "a b"` is one argument, not two).
- Separating operators makes it easier to identify redirections and pipes in the next step.

---

## 3. Command Construction (`tokens_into_cmds.c`)

This stage converts the array of tokens into a structured linked list of commands ready for execution.

### Key Structures:
- **`t_cmd`**: Represents a single command in a pipeline.
  - `args`: Array of strings (command name + arguments).
  - `redir`: Linked list of redirections for this command.
  - `next`: Pointer to the next command in the pipeline (if `|` is present).
- **`t_parsing`**: A helper struct to pass state (current command, token list, shell instance) around functions.

### Logic Flow (`tokens_into_cmds`):
1. **Initialization**: Creates the head of the `t_cmd` list.
2. **Token Loop**: Iterates through the token array.
   - **Pipe (`|`)**:
     - Triggers `handle_pipe_token` (`parser_helpers.c`).
     - Finishes the current `t_cmd` node and creates a new one linked via `next`.
   - **Heredoc (`<<`)**:
     - Triggers `handle_heredoc_wrapper` -> `handle_heredoc_token` (`heredoc_exec.c`).
     - Processes the heredoc immediately (reads input, saves to temp file).
     - Adds a `RED_HEREDOC` redirection node pointing to the temp file.
   - **Redirection (`<`, `>`, `>>`)**:
     - Triggers `handle_redirect_token` (`parser_helpers.c`).
     - Takes the *next* token as the filename.
     - Adds a node to the `redir` list of the current command.
   - **Arguments (Words)**:
     - Any other token is treated as a command argument.
     - Added to the `args` array of the current `t_cmd`.

**Why this way?**
- Processing tokens linearly allows us to handle complex commands like `ls > out -l | grep x` correctly.
- Handling heredocs during parsing ensures input is captured before execution starts, matching bash behavior.

---

## 4. Token Processing (`process_token.c` & `expand_variables.c`)

Tokens are further processed *after* or *during* command construction to handle variable expansion and quote removal.

### Logic Flow:
1. **`process_token`**:
   - Checks if the token is single-quoted (`'...'`). If so, it just removes quotes (no expansion).
   - Otherwise, calls `variable_expansion`.
   - Finally, calls `remove_quotes` to strip outer quotes.

2. **`variable_expansion`**:
   - Scans the string for `$`.
   - **Double Quotes**: Expand variables (`$VAR`), but keep spaces literal.
   - **No Quotes**: Expand variables.
   - **`$?`**: Replaced with `shell->exit_code`.
   - **`$VAR`**: Replaced with value from `envp`.

3. **`remove_quotes`**:
   - Removes the outermost layer of matching quotes (`'` or `"`).
   - Preserves inner quotes (e.g., `"'hello'"` -> `'hello'`).

**Why this way?**
- **Expansion rules**: Bash expands variables inside double quotes but not single quotes.
- **Order matters**: Expansion happens *before* quote removal so that `$VAR` inside `"` becomes the value, but `"$VAR"` inside `'` stays literal.

---

## Summary of File Responsibilities

- **`tokenizer.c`**: Raw string -> Array of tokens.
- **`tokens_into_cmds.c`**: Array of tokens -> Linked list of `t_cmd`.
- **`parser_helpers.c`**: specialized handlers for pipes and redirects.
- **`expand_variables.c`**: Handles `$VAR` and `$?` expansion.
- **`process_token.c`**: Orchestrates expansion and quote removal.

