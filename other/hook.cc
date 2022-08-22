#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <assert.h>

// extern "C" {
typedef void*(*malloc_fun)(size_t size);
malloc_fun malloc_f = NULL;
typedef void(*free_fun)(void *ptr);
free_fun free_f = NULL;
// }

void *malloc(size_t size) {
    fprintf(stderr, "[malloc]\t\t\tsize = %ld\n", size);
    void* ret = malloc_f(size);
    fprintf(stderr, "[malloc success]\t\tsize = %ld ptr = %p\n", size, ret);
    return ret;
}
void free(void *ptr) {
    fprintf(stdout, "[free]\t\t\t\tptr = %p\n", ptr);
    free_f(ptr);
}


int main(int argc, char const *argv[]) {
    malloc_f = (malloc_fun)dlsym(RTLD_NEXT, "malloc");
    free_f = (free_fun)dlsym(RTLD_NEXT, "free");
    int *ptr = (int*)malloc(8);
    // delete ptr;
    free(ptr);
    return 0;
}

