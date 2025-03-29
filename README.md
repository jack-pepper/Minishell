### TO DO

[Y] Currently reviewing Minishell documentations to fully understand the MS project. Then, I will go through your code to understand it better and will refactor my Pipex so it works with Minishell's main.

[M] I am working on implementing the commands and improving the parsing for them


### ISSUES

28/03 [M] parsing: should handle case like "cd  /  home" or "echo -   n" aswell

28/03 [M] we will need to compare output with BASH. ZSH has a different behavior (ex: see with cmd echo)

28/03 [M] segfault if the input is composed only of whitespace

26/03 [M] check env arg to main for portability issues

19/03 [M] make sure readline is correctly included (an installation during compilation may be needed)


### IDEAS FOR AN OUTSTANDING PROJECT

20/03 [M] we could save the history to a file before exit (so it is available on next startup)

19/03 [M] prompt: include later the prompt symbol as an env variable at startup? It would allow to modify it easily.
