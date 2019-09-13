#include <stdio.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

/*
Wasm does not protect against overwriting function pointers. As a mitigation against function pointer
overwrites, Wasm checks the signature of the called function when making an indirect function call, and
checks that the signature matches what is expected. The function signature is made up of the number
and types of arguments to the function. This means that an exploit cannot transfer execution to a
function having the wrong signature.
*/

void EMSCRIPTEN_KEEPALIVE default_func() {
 printf("Pointer overwrite failed\n");
}
void EMSCRIPTEN_KEEPALIVE hijacked_func() {
 printf("Pointer overwrite SUCCEEDED!\n");
}
void EMSCRIPTEN_KEEPALIVE func_ptr_overwrite(char *str) {
 void *func_ptr;
 char buf[50];
 func_ptr = default_func;
 printf("Function number for default_func : %x\n",(int)default_func);
 printf("Function number for hijacked_func : %x\n",(int)hijacked_func);
 printf("Address of buf: %x\n",(int)buf);
 printf("Address of func_ptr: %x\n",(int)&func_ptr);
 sprintf(buf,"The entered string is: %s",str);
 printf("%s\n",buf);
 ((void (*)(void))func_ptr)(); //execute function of pointer
}