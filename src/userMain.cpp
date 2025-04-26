#include "../test/threads_C_API.hpp"


//calling test functions and started thread beside main thread
void userMain(void*) {
    threads_C_API_test();
}