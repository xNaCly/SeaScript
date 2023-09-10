# SeaScript

SeaScript is a small language that compiles to C. SeaScript is intended to improve the c experience drastically with the following features:

- result types
- functions attached to structs
- omitting most of the typ declarations
- providing useful data structures, such as growing arrays and hash maps
- 0 cost abstractions for functional approaches, such as filter, map, forEach
- template strings
- booleans
- 0 cost interoperability with the c ecosystem

SeaScript is designed to automatically provide allocation and deallocation for
data structures and abstractions. The goal is to compute valid, performant and
safe c while allowing the developer to use a language that's minimal, nice to
read and that provides the base line of features required for modern
applications.

## Compiler usage

```
$ cat main.sea
#include <stdio.h>
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
:: include header files
#include <stdio.h>

:: variables
age = 24
money = 129.01
name = "anon"

:: lists
list = [5 4 3 2 1]
friends = ["jonathan" "mike"]

:: functions
square [n: number]: number {
    return n*n
}

:: single line function
printer2n [a: number b: number]: void {
    printf("%d %d\n" a b)
}

:: calling functions
printer2n(square(12) square(2))

:: iterating over lists
list.each(n {
    printf("%f\n" n)
})

:: c block
@c{
    void print(int a, int b) {
        printf("%d %d\n", a, b);
    }
}

:: using functions defined in c block
print(1 2)

:: results
sqrt [n: number]: Result[number] {
    if n < 0 {
        return Result{error: "can't compute sqrt for negative numbers"}
    }
    res = sqrt(n)
    if res < 0 {
        return Result{error: "couldn't compute sqrt of {n}"}
    }
    return Result{value: res}
}

:: consuming results
res = sqrt(-5).unwrap() :: this will fail and stop the process
print(res)

:: structs
person = {
    age: 24,
    money: 102_910.99,
    name: "seal",
    toString [s: self]: string {
        return "{s.name} | {s.age} | {s.money}",
    }
}

:: accessing struct properties
isTeenager = person.age > 13 && person.age < 20

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
