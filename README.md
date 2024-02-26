# simple shell

Simple shell based on readline library.

## Build

Use `make`.

```bash
red@DESKTOP-ATO322N:~/projects/readline-simple-shell$ make
cc -Wall -O -c main.c
cc -o readline-simple-shell main.o -lreadline
```

## Example

```bash
red@DESKTOP-ATO322N:~/projects/readline-simple-shell$ ./readline-simple-shell
> /bin/ls -l
total 40
-rw-r--r-- 1 red red   283 Feb 27 00:26 Makefile
-rw-r--r-- 1 red red   863 Feb 27 00:27 README.md
-rw-r--r-- 1 red red  2107 Feb 27 00:17 main.c
-rw-r--r-- 1 red red  5592 Feb 27 00:28 main.o
-rwxr-xr-x 1 red red 18024 Feb 27 00:28 readline-simple-shell
```

## TODO
- Get binary location (like [which](https://github.com/clibs/which/blob/master/src/which.c)).

## References

- [readline library documentation](https://tiswww.case.edu/php/chet/readline/readline.html)
- [execv(3)](https://linux.die.net/man/3/execv)
- [pipe(2)](https://linux.die.net/man/2/pipe)
