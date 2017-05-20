# cmok

CMok is a mocking framework for C.  It is a header-only macro-only
library.  This makes it incredibly easy to embed in other projects.

*Note: the code calling the mocked function does not change or need
to be recompiled.*  Only the definition file needs to be recompiled.

## Creating the mocked function

To enable CMOK to do anything, define the macro `CMOK_DO_MOCK`.

The next subsection is not required to compile normally, but is
required as a forward declaration before tests.

### Declaring the mocked functions

Instead of declaring your functions normally, use the macro
`CMOK_DECLARE()`.  This is easy to do.  Just wrap a call to
`CMOK_DECLARE()` around it and insert commas before and after the
name:

    /* instead of `int mocked_fun(void);`, do: */
    CMOK_DECLARE(int, mocked_fun, (void));

CMOK_DECLARE is not really required in the header, but it is required
before you mock the function in a test.

### Defining the mocked function

Then when you are defining your function, use the macro `CMOK_DEFINE()`:

    /* instead of `int mocked_fun(void) { return 0; }`, do: */
    CMOK_DEFINE0(int, mocked_fun) { return 0; }

If your function accepts arguments, increment the 0 to the number you want.
Here is how to mock `realloc`:

    CMOK_DECLARE(void*, custom_realloc, (void*, size_t size));
    CMOK_DEFINE2(void*, custom_realloc, void*, ptr, size_t, size) {
        return realloc(ptr, size);
    }

If your function accepts 10 or more arguments then the define call has
to change to something like the following.  We first pass arguments
like in declare then a list of names:

    CMOK_DECLARE(int, sum10, (int, int, int, int, int, int,
                              int, int, int, int))
    CMOK_DEFINE2(int, sum10,
                 (int a, int b, int c, int d, int e, int f, int g,
                  int h, int i, int j),
                 (a, b, c, d, e, f, g, h, i, j)) {
        return a + b + c + d + e + f + g + h + i + j;
    }

#### Recursion

If you use recursion in your function definition, you **MUST** recurse
by calling the function `CMOK_REC(name)` instead of `name`.  If you
don't and `CMOK_FUN(name)` is not reset, this could potentially cause
weird bugs in testing.

    CMOK_DEFINE1(int, fib, (int n)) {
        if (n == 0) return 1;
        if (n == 1) return 2;
        return CMOK_REC(fib)(n - 1) + CMOK_REC(fib)(n - 2);
    }

## Usage in tests

Make sure `CMOK_DO_MOCK` is defined.  If it is not, `CMOK_FUN()` and
`CMOK_RESET()` will not be defined.

Let's use the previous `custom_realloc` function:

    CMOK_DECLARE(void*, custom_realloc, (void*, size_t size));
    CMOK_DEFINE2(void*, custom_realloc, void*, ptr, size_t, size) {
        return realloc(ptr, size);
    }

Then we'll create a fake version that always fails to allocate:

    void* null_realloc(void* ptr, size_t size) { return NULL; }

Then we can set all calls to `custom_realloc` to go to `null_realloc`
instead (note that this must be inside a function):

    CMOK_FUN(custom_realloc) = null_realloc;
    assert(custom_realloc(NULL, 1) == NULL);

Once we are done with these tests, we reset our function:

    CMOK_RESET(custom_realloc);

Putting it all together:

    /* Forward declaring the old fashioned way is still valid, you
     * just are forced to repeat your self... */
    void* custom_realloc(void*, size_t size);
    /* When you forward declare the mocking components.  This also
     * forward declares the function. */
    CMOK_DECLARE(void*, custom_realloc, (void*, size_t size));
    CMOK_DEFINE2(void*, custom_realloc, void*, ptr, size_t, size) {
        return realloc(ptr, size);
    }
    void* null_realloc(void* ptr, size_t size) { return NULL; }

    void test_null_realloc_mock(void) {
        CMOK_FUN(custom_realloc) = null_realloc;
        assert(custom_realloc(NULL, 1) == NULL);
        CMOK_RESET(custom_realloc);
    }

If your function returns `void` instead of a value, insert a `v` after
`DEFINE` in the call to `CMOK_DEFINE` and don't specify a return
value.  You do not need to do this if you use C++.
