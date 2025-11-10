# 🩺 Minishell Health Check – November 2025

Your minishell now covers the 42 mandatory scope (prompt, history, redirects, heredoc, pipelines, env expansion, builtins) with recent guardrails added around redirections, heredocs, and builtin execution paths. Memory remains leak-free under normal runs. Here’s the current health snapshot.

---

## ✅ Strengths

- **Redirect error handling**  
  `handle_redirects` prints simple, Bash-like messages (`"Permission denied"`, `"No such file or directory"`) and exits the child early on failure. Parent-only builtins restore stdio after redirects, preventing lingering FD changes.

- **Heredoc robustness**  
  Forked heredocs respect signals (`SIGINT` → exit 130) and remove temp files for discarded delimiters. Allocation failures bubble up cleanly via `add_redirect` checks.

- **Environment duplication & teardown**  
  `copy_envp` NULL-terminates per step and frees partially built arrays on errors, keeping Valgrind clean even with ENOMEM injected.

- **Variable expansion**  
  `expand_variables` consistently uses `ft_strjoin_and_free`, checks each allocation, and frees intermediate buffers, avoiding leaks with tricky `$` cases.

- **`exit` builtin**  
  Overflow and non-numeric arguments trigger the Bash-compatible “numeric argument required” path (exit 2), while clean exits free shell state first.

---

## ❗ Critical Issues

None outstanding at this time.

---

## ⚠️ Follow-Ups

- Re-run malloc-failure/ASAN stress tests to ensure the parser still cleans up gracefully.  
- Validate pipeline status propagation with `false | true`, `export X=1 | cat`, etc., watching `$?` for regressions.  
- Maintain `_exit` usage for any new forked helpers to avoid double-flush surprises.

---

## 🔍 Observations

- `handle_redirects` no longer relies on `strerror`; other helpers should also decide what to print before closing FDs.  
- `g_minishell_is_executing` toggles correctly across execution paths—keep testing `Ctrl-C` during heredocs/pipelines.  
- Optional polish: trim lingering verbose comments in `main.c` or other legacy parts to keep reviewers focused on behavior.

---

## 🧪 Suggested Regression Tests

1. **Parser OOM:** Inject malloc failures for `add_argument` (ASAN or LD_PRELOAD) and confirm no crashes/leaks.  
2. **Pipeline status:** `false | true`, `export X=1 | cat`, `exit 7 | cat` → verify `$?` matches expectations.  
3. **Parent builtin redirects:** `cd /tmp >out` then `pwd` to ensure STDIN/STDOUT restoration.  
4. **Heredoc interrupt:** `cat <<EOF` + `Ctrl-C` → exit 130 and prompt reset.  
5. **Exit overflow:** `exit 9223372036854775808` and negative counterpart → “numeric argument required”.

---

## Summary

- ✅ Core behavior aligns with the subject; robustness around redirects/heredocs built up nicely.  
- ⚙️ Keep running the regression suite (malloc-failure + pipelines) after any tweak.  
- 🔁 Finish with a Valgrind/ASAN pass before submission.

Ping me if you spot anything new or need another review. Good work! 💪