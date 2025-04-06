## WEEK: 24/03 – 30/03
### 🔧 TO DO

[Y] I'm currently adding some parsing logic to my pipex to handle redirection '>>' '<' '>' and pipes '|'.

[M] Commands are (mostly) implemented. Parsing needs to be refined but should allow enough further testing at the moment.


### 🐞 ISSUES

06/04 [M] TODO: env variables should be interpreted in paths too! (ex: with cmd cd) 
	  TODO: unset cmd must handle several env var in a single command
	  TODO: need to process lines like: $USER $VAR ABC $GHI CDE

28/03 [M] parsing: should handle case like "cd  /  home" or "echo -   n" aswell

28/03 [M] we will need to compare output with BASH. ZSH has a different behavior (ex: see with cmd echo)

19/03 [M] make sure readline is correctly included (an installation during compilation may be needed)


### 💡 IDEAS FOR AN OUTSTANDING PROJECT

20/03 [M] we could save the history to a file before exit (so it is available on next startup)

19/03 [M] prompt: include later the prompt symbol as an env variable at startup? It would allow to modify it easily.

### ✅ Completed Tasks
(Nothing yet)
