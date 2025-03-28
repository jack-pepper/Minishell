### CHAT
[Y]: Here is another option to setup the notifications:
Go to this link: https://slack.github.com/ and click “Add to Slack”
Go to the Github, Yel-bouk Conversation and type "/github subscribe jack-pepper/Minishell"
When GitHub asks you to install the app, select Only select repositories and choose Minishell
Complete the install

[M]: Doesn't seem to be possible anymore: https://github.com/orgs/community/discussions/108527

[Y]: Morgan, could you give me admin access to the Minishell repo?
     I want to hook up the Slack integration so we both get commit notifications and stay in sync via slack. Thanks!
You need to:
→ Go to the repo settings → Collaborators
→ Set my user to Admin

[M]: Two approaches for handling env: global var extern char environ (man environ) or passing a third arg (env) to main that contains the list of env variables. I've settled for the second option. But we must keep in mind possible portability issues.

[M]: The t_command approach seems efficient enough, simple commands (exit, pwd) are working (no proccess management atm though, bonus will depend on that too).  

[Y]: Sounds good! I'll check out the links you provided and get back to you. I'm currently still troubleshooting my PIPEX to fix all the leaks. Technically, it's part of Minishell, so once it's done, a good chunk of Minishell will be completed too.

[M]: Nice! I am trying to set the global architecture atm. Found some useful doc: 
     - on readline: https://web.mit.edu/gnu/doc/html/rlman_2.html
     - how to use a function pointer in a struct: https://www.codeproject.com/Tips/800474/Function-Pointer-in-C-Struct
     I was told today that "the bonus should be planned from the very beginning". Let's think about it ;-)

[Y]: I use a Linux VM (Ubuntu), but my host system is MacOS. I tested the make file and it looks to be working correctly on my end. I am currently working on setting up a webhook so we can be notified in slack every time one of us pushes an update on Git Hub.

[M]: I'm working on Linux. Let's check whether there are compatibility issues with MacOS. You might have to install readline (brew install readline).

[M]: Here we can keep track of what we did and update the README later to explain how we worked

### HISTORY

28/03 [M] start working on parsing

26/03 [M] add env cmd and env arg to main (will be needed elsewhere)

25/03 [M] add cmd exit and pwd. [TODO] conceive the parsing to test cmd cd

20/03 [M] add history, init commands files in a folder, split functions in minishell and handle CTRL+D (exit) 
          [TODO] Decide if the t_command approach would fit our needs and how

20/03 [M] add a t_shell struct, make prompt a loop) NB: readline() leak does NOT have to be handled as per the subject

19/03 [M] add a simple prompt ("one-shot", simply returns the given string)

19/03 [M] include readline (added both -lreadline -lncurses to Makefile, it *should* work on all systems).

11/03 [M] try to implement a prompt ([TODO] debug compil error)

11/03 [M] init project (create main folders and files)

### IDEAS FOR AN OUTSTANDING PROJECT

20/03 [M] we could save the history to a file before exit (so it is available on next startup)

19/03 [M] prompt: include later the prompt symbol as an env variable at startup? It would allow to modify it easily.

### POSTPONED ISSUES (MUST CHECK BEFORE VALIDATING THE PROJECT)

26/03 [M] check env arg to main for portability issues

19/03 [M] make sure readline is correctly included (an installation during compilation may be needed)

