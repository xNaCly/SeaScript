# SeaScript

SeaScript is a small language that compiles to C.

## Compiler usage

```
$ cat main.sea
put "Hello (Sea)World!"
$ seacc main.sea
$ cat main.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  puts("Hello (SeaWorld)");
  return EXIT_SUCCESS;
}
```

## Syntax

```seascript
:: variables
age = 24
money = 129.01
name = "anon"

:: lists
list = [5, 4, 3, 2, 1]
friends = ["jonathan", "mike"]

:: structs
person = {
    age: 24
    money: 102_910.99
    name: "seal"
}

:: accessing structs
put person.age
person.age = 14

:: functions
square x -> {
    squared = x * x
    :: implicit return
    squared
}


:: variable arguments
printer a... -> put a
```
