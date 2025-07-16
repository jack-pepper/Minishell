# Minishell

## Description
Minishell is a lightweight Unix shell built in C as part of the École 42 curriculum. It reproduces essential shell functionalities, offering a deeper understanding of how Unix environments manage processes, inputs/outputs, and command interpretation.

Developed with Yahya Elboukili (https://github.com/yahyaeb), this project was designed to prioritize robustness, clarity, and adherence to shell behavior (as much as we could!).

**Technologies Used:**
- C
- UNIX Signals

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Contributing](#contributing)
- [Acknowledgments](#acknowledgments)

## Installation
To test this project locally:

1. Clone this repository:
   ```bash
   git clone https://github.com/pepper/Minishell.git
2. Navigate into the project directory:
   ```bash
   cd Minishell
3. Compile the server and client by running:
    ```bash
    make

## Usage
1. Run... and try it out!
    ```bash
    ./minishell

## Features

- Interactive Prompt: Displays a custom prompt and reads user input with live updates (including signal handling).
- Command Execution: Runs both built-in commands and external binaries.
- Built-in Commands: supports cd, echo, pwd, export, unset, env, exit
- Pipes: handles commands connected via |, including chained pipes with proper file descriptor management.
- Redirections: Input <, Output >, Append >>, Heredoc <<
- Environment Variable Management: includes expansion and dynamic updates to the shell environment, as well as a stash (case VAR=val anterior to the command export)
- Signal Handling: gracefully handles SIGINT, SIGQUIT, EOF, and ensures clean behavior during prompts and execution.
- Error Feedback: Displays context-aware error messages (invalid syntax, missing files, permission errors, etc.)

## Contributing
If you have suggestions for improvements, bug fixes, or interesting features, feel free to contribute.

### How to Contribute:
- Fork the repository.
- Create a new branch (git checkout -b feature-branch).
- Make your changes and commit them (git commit -am 'Add new feature').
- Push to your branch (git push origin feature-branch).
- Open a pull request.

## Acknowledgments
Thanks to my partner Yahya for this experience!

