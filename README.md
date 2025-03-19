### @Yahya: here we can keep track of what we did and update the README later to explain how we worked

### HISTORY

11/03 [m] init project (create main folders and files)
11/03 [m] try to implement a prompt ([TODO] debug compil error)
19/03 [m] include readline (added both -lreadline -lncurses to Makefile, it *should* work on all systems).
	/!\ I'm working on Linux. Let's check whether there are compatibility issues with MacOS.
          You might have to install readline (brew install readline).
19/03 [m] add a simple prompt ("one-shot", simply returns the given string)


### IDEAS
19/03 [m] prompt: include later the prompt symbol as an env variable at startup? It would allow to modify it easily.

### POSTPONED ISSUES (MUST CHECK BEFORE VALIDATING THE PROJECT)
19/03 [m] make sure readline is correctly included (an installation during compilation may be needed)

### @Morgan: I use a Linux VM (Ubuntu), but my host system is MacOS. I tested the make file and it looks to be working correctly on my end.
I am currently working on setting up a webhook so we can be notified in slack every time one of us pushes an update on Git Hub.
