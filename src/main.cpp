#ifdef NDEBUG
    extern const bool in_debug_mode = false;
#else
    extern const bool in_debug_mode = true;
#endif

#include <Engine.hpp>

int main(int argc, char* argv[])
{
    // Unused argc, argv
    (void) argc;
    (void) argv;

    std::cout << "Hello World!" << std::endl;

    return 0;
}