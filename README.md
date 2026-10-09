# Simple C++ Shell 🖥️

A lightweight command-line shell implemented in C++ that demonstrates fundamental operating system concepts, including process management, command execution, and file operations.

## Features

* `cd` — Change the current working directory.
* `help` — Display available built-in commands.
* `exit` — Exit the shell.
* `rm` — Remove a file.
* `mv` — Move or rename files and directories.
* Execute external commands using `fork()`, `execvp()`, and `waitpid()`.
* Interactive command-line interface with basic error handling.

## Technologies

* C++
* Linux/Unix system calls
* POSIX process management

## Getting Started

Compile the program using:

```bash
g++ main.cpp -o myshell
```

Run the shell:

```bash
./myshell
```

**Note:** This project is designed for Unix-like systems. Command parsing is basic and does not currently support features such as quoted arguments, pipes, or input/output redirection.
