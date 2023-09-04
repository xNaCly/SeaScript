# SeaScript

SeaScript is a small language that compiles to C.

## Compiler usage

```
$ cat main.sea
puts("Hello (Sea)World!")
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

:: functions
square [n: number]: number -> {
    n2 = n * n
    :: implicit return
    n2
}

:: single line function
printer2n [a: number, b: number]: void -> printf("%d %d\n", a, b)

:: calling functions
printer2n(square(12), square(2))

:: structs
person = {
    age: 24,
    money: 102_910.99,
    name: "seal",
    toString [s: self]: string -> "${s.name} | ${s.age} | ${s.money}",
}

:: accessing struct properties
isTeenager = person.age > 13 and person.age < 20

:: updating struct properties
if isTeenager {
    person.age = 14
} else if person.age == 20 {
    person.age = 0
} else {
    puts("unhandled age")
}

:: calling struct functions
puts(person.toString())
```
