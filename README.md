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
square n [number:number] -> {
    n2 = n * n
    :: implicit return
    n2
}

:: single line function
printer argument [number:void] -> printf("%d\n", argument)

:: calling functions
printer(square(12))

:: structs
person = {
    age: 24,
    money: 102_910.99,
    name: "seal",
    toString this [self:string] -> {
        "${this.name} | ${this.age} | ${this.money}"
    },
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
