Minishell

Minishell is a lightweight Unix shell built in C as part of the École 42 curriculum. It reproduces essential shell functionalities, offering a deeper understanding of how Unix environments manage processes, inputs/outputs, and command interpretation.

Developed by Pepper and Yahya, this project was designed to prioritize robustness, clarity, and adherence to shell behavior — all without shortcuts.

⸻

🚀 Features
	•	Interactive Prompt
Displays a custom prompt and reads user input with live updates (including signal handling).
	•	Command Execution
Runs both built-in commands and external binaries.
	•	Built-in Commands
Supports:
cd, echo, pwd, export, unset, env, exit
	•	Pipes
Handles commands connected via |, including chained pipes with proper file descriptor management.
	•	Redirections
Supports:
	•	Input <
	•	Output >
	•	Append >>
	•	Heredoc <<
	•	Environment Variable Management
Includes expansion and dynamic updates to the shell environment.
	•	Signal Handling
Gracefully handles SIGINT, SIGQUIT, EOF, and ensures clean behavior during prompts and execution.
	•	Error Feedback
Displays context-aware error messages (invalid syntax, missing files, permission errors, etc.)

⸻

🛠 Getting Started

Prerequisites
	•	GCC
	•	Make
	•	A Unix-like OS (Linux recommended)

 Build

 git clone https://github.com/jack-pepper/minishell.git
cd minishell
make

👥 Authors
	•	Pepper
	•	Yahya Elboukili

