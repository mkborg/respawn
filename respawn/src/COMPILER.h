/*

  Compiler specific stuff like GCC's "__attribute__"

*/

#if !defined COMPILER_h
#define COMPILER_h


/*
    A few standard library functions, such as `abort' and `exit', cannot
    return. Some programs define their own functions that never return.
*/
#define NORETURN __attribute__ ((__noreturn__))

/*
    The `packed' attribute specifies that a variable or structure field
    should have the smallest possible alignment.
*/
#define PACKED __attribute__((packed))

/*
    Many functions have no effects except the return value and their
    return value depends only on the parameters and/or global variables.
*/
#define PURE __attribute__((pure))

/*
    Normally, the compiler places the code it generates in the `text'
    section.  Sometimes, however, you need additional sections, or you
    need certain particular functions to appear in special sections.
*/
#define SECTION(section_name) __attribute__ ((section (section_name)))

/*
    When attached to a type (including a `union' or a `struct'), this
    attribute means that variables of that type are meant to appear
    possibly unused.  GCC will not produce a warning for any variables
    of that type, even if the variable appears to do nothing.
*/
#define UNUSED __attribute__((unused))


#endif // !defined COMPILER_h
