# 🩺 Minishell Health Check – November 2025 (Post-Fixes)

Your minishell now satisfies the 42 “mandatory” feature set: prompt, history, pipelines, redirects (`<`, `>`, `>>`, `<<`), environment expansion (including `$?`), and all required builtins. Recent updates tightened redirect handling, heredoc allocation checks, literal `$` expansion, and `exit` overflow detection. Memory usage remains leak-free in normal runs. Below is a detailed status report: strengths, remaining issues, and regression tests to run before hand-in.

---

## ✅ Strengths

- **Redirect safety**  
  `handle_redirects` returns `FAILURE` on `open`/`dup2` errors, prints `strerror(errno)`, and callers `_exit()` immediately with a non-zero status. Parent-only builtins in `run_builtin` dup/restore stdio when redirects are present, preventing persistent descriptor corruption.

- **Heredoc robustness**  
  `process_heredoc` forks with dedicated signal handlers, translates SIGINT into exit code `130`, and `cleanup_heredoc_files` removes superseded heredoc files. `handle_heredoc_token` now checks `add_redirect` success, so allocation failures abort parsing cleanly instead of leaving inconsistent state.

- **Environment duplication & teardown**  
  `copy_envp` NULL-terminates incrementally and frees both arrays on partial failure, keeping Valgrind output clean even under ENOMEM paths.

- **Literal `$` handling**  
  `expand_variables` uses `ft_strjoin_and_free` for every literal-dollar branch, checks every allocation, and frees `result` on error—no more leaks or NULL derefs.

- **`exit` numeric validation**  
  `builtin_exit` rejects overflow inputs via an explicit `LONG_MAX`/`LONG_MIN` check, printing “numeric argument required” like Bash.

---

## ❗ Critical Issues (Fix Before Submission)

1. **Double-free on argument allocation failure**  
   `handle_token` frees `token_copy` when `add_argument` fails, but `add_argument` already owns (and frees) that pointer. Under malloc failure this will crash.
```46:54:src/Parsing/tokens_into_cmds.c
p->current->args = add_argument(p->current->args, token_copy);
if (!p->current->args)
{
    free(token_copy);
    return (-1);
}
```
```29:43:src/cmd_utils.c
if (!new_args)
{
    free(new_arg);
    return (NULL);
}
```
   **Action:** Remove the extra `free(token_copy);` and simply return `-1`; `free_cmds` already releases the partially built command list.

2. **Pipeline builtins exit with stale `$?`**  
   In pipeline children, `_exit(shell->exit_code)` reuses the previous command’s status rather than the builtin’s result.
```98:104:src/Pipelines/pipeline_exec.c
if (run_builtin(current, shell) != FAILURE)
    _exit(shell->exit_code);
```
```61:100:src/Executor/executor_utils.c
/* non-parent builtins */
return (execute_builtin(cmd, shell));
```
   **Action:** Capture the return value (`int rc = run_builtin(...); shell->exit_code = rc; _exit(rc);`) or have `run_builtin` update `shell->exit_code` internally before returning.

3. **Child builtin helper uses `exit()` instead of `_exit()`**  
   `child_redirects` calls `exit(result);` after running a builtin with redirects, risking double-flush of stdio buffers.
```15:33:src/Executor/executor_utils.c
if (pid == 0)
{
    ...
    result = execute_builtin(cmd, shell);
    exit(result);
}
```
   **Action:** Replace with `_exit(result);` for the forked helper.

---

## ⚠️ High-Priority Follow-Ups

- After removing the double-free, re-test parser failure paths (malloc hooks / ASAN) to confirm `tokens_into_cmds` still frees partially built command lists correctly.
- Once pipeline exit codes are fixed, verify `$?` for cases like `false | true`, `export X=1 | cat`, and `exit 7 | cat`.
- Ensure `_exit` is used in every forked path (helpers, pipeline children) to keep stdio buffers consistent.

---

## 🔍 Additional Observations

- Error reporting preserves `errno` in `handle_redirects`; keep that pattern in other helpers (`execute_path_cmd`, `find_cmd_path`) when modifying them.
- `g_minishell_is_executing` toggles correctly around execution; continue testing `Ctrl-C` during heredoc input and active pipelines to confirm prompt behaviour.
- Optional cleanup: trim outdated comments or debug notes (e.g. in `main.c`) for clearer peer review.

---

## 🧪 Suggested Regression Tests

1. **Parser OOM** – Inject malloc failure for `add_argument`; expect graceful error with no double-free (ASAN/valgrind).
2. **Pipeline builtin status** – `false | true`, `export X=1 | cat`, `exit 7 | cat`; `$?` should match the builtin’s return.
3. **Parent builtin redirects** – `cd /tmp >out` then `pwd`; ensure descriptors are restored after execution.
4. **Heredoc interrupt** – `cat <<EOF` followed by `Ctrl-C`; expect cleanup and exit code `130`.
5. **Exit overflow** – `exit 9223372036854775808` and `exit -9223372036854775809`; expect “numeric argument required” and exit status `2`.

---

## Summary & Next Steps

- ✅ Core functionality matches the subject requirements; recent robustness improvements look strong.
- ❗ Address the outstanding critical items (parser double-free, pipeline builtin exit code, `_exit` in child helper) before evaluation.
- 🔁 Re-run the regression tests above and do a final Valgrind/ASAN sweep once those fixes land.

Ping me when the blockers are resolved and I’ll run a final sanity check. Great work—keep it up! 💪