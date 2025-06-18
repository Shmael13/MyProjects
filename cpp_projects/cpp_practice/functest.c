//C behaves really differently than C++
//The functest.c and functest.cpp show how.
//Nested funcs aren't allowed in C++, and you can't call main from foo either!
#include <stdbool.h>
#include <stdio.h>

bool main();
void foo();

void foo(){
  int nested_function(){
    printf("Nesting Works\n");
  }
  printf("Inside Foo\n");
  nested_function();
  main();
}

bool main(){
  printf("Inside Main\n");
  foo();
}
