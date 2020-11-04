#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

// don't modify this

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run(); 
} 