# Mini-Shell #

You are required to build a bash-like shell for the following requirements. Your program
should not use temporary files, popen(), system() library calls. It should only use system-call
wrappers from the library. It should not use sh or bash shells to execute a command.
* Shell should wait for the user to enter a command. User can enter a command with
multiple arguments. Program should parse these arguments and pass them to execv() call.
For every command, shell should search for the file in PATH and print any error. Shell
should also print the pid, status of the process before asking for another command.
* shell should support any number of commands in the pipeline. e.g. ls|wc|wc|wc. Print
details such as pipe fds, process pids and the steps. Redirection operators can be used in
combination with pipes.
* shell should support two new pipeline operators "||" and "|||". E.g.: ls -l || grep ^-,
grep ^d. It means that output of ls -l command is passed as input to two other
commands. Similarly, "|||" means, output of one command is passed as input to three
other commands separated by ","…
* shell should mask all signals except SIGQUIT and SIGINT. When SIGINT is received, it
should print last 10 commands executed by the user, along with status of each. When
SIGQUIT is pressed, it should ask user "Do you really want to exit?". If yes, it should exit
