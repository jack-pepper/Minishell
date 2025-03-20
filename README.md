### CHAT (last first)

[M]: Nice! I am trying to set the global architecture atm. Found some useful doc: 
     - on readline: https://web.mit.edu/gnu/doc/html/rlman_2.html
     - how to use a function pointer in a struct: https://www.codeproject.com/Tips/800474/Function-Pointer-in-C-Struct

[Y]: I use a Linux VM (Ubuntu), but my host system is MacOS. I tested the make file and it looks to be working correctly on my end. I am currently working on setting up a webhook so we can be notified in slack every time one of us pushes an update on Git Hub.

[M]: I'm working on Linux. Let's check whether there are compatibility issues with MacOS. You might have to install readline (brew install readline).

[M]: Here we can keep track of what we did and update the README later to explain how we worked

### HISTORY (last first)

20/03 [M] add history, init commands files in a folder [TODO] Decide if the t_command approach would fit our needs and how
20/03 [M] add a t_shell struct. [TODO] find out why free(line) won't free / init history (cf usr/include/readline/history.h) (= because the signal doesn´t handle it yet)
19/03 [M] add a simple prompt ("one-shot", simply returns the given string)
19/03 [M] include readline (added both -lreadline -lncurses to Makefile, it *should* work on all systems).
11/03 [M] try to implement a prompt ([TODO] debug compil error)
11/03 [M] init project (create main folders and files)

### IDEAS
19/03 [M] prompt: include later the prompt symbol as an env variable at startup? It would allow to modify it easily.

### POSTPONED ISSUES (MUST CHECK BEFORE VALIDATING THE PROJECT)
19/03 [M] make sure readline is correctly included (an installation during compilation may be needed)


