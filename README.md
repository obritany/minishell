# minishell

Minishell is a minimum viable version of a real shell.
The main goal is to have a good understanding of process creation and synchronisation using the C programmming language.

## Installation & Usage

### Building the program

1. Download/Clone this repo

        git clone https://github.com/obritany/minishell
2. `cd` into the root directory and run `make`

        cd minishell
        make

### Running the program

After building the source, run `./minishell` from the project root.

## Main Project Instructions

### Mandatory

- Can only use C
- No memory leaks
- Implement a series of builtins: `echo`, `cd`, `setenv`, `unsetenv`, `env`, `exit`
- Manage the errors without using `errno`, by displaying a message adapted to the error output
- Can only use these standard library functions:
    - malloc, free
    - access
    - open, close, read, write
    - opendir, readdir, closedir
    - getcwd, chdir
    - stat, lstat, fstat
    - fork, execve
    - wait, waitpid, wait3, wait4
    - signal, kill
    - exit
- Must have a Makefile to build the program
- The binary file must be named `minishell`
- Handle program interruption (Ctrl + D)

### Bonuses

- Signal management (specifically Ctrl + C)
- PATH's right management (error handling)
- Multiple commands (semi colons)
