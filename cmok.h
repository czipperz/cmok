/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2017 Chris Gregory czipperz@gmail.com
 */

/*! \file cmok.h
 *
 * \brief CMok is a simple mocking framework defined only through macros.
 *
 * To turn on mocking, define \c CMOK_DO_MOCK.
 * To declare a function that is mockable, use \c CMOK_DECLARE().
 * To define a function that is mockable, use \c CMOK_DEFINE().
 *
 * Helpful macros \c CMOK_DEFINE0 through \c CMOK_DEFINE9 are defined
 * to help defining functions without repetition.
 */

#ifndef HEADER_GUARD_CMOK_H
#define HEADER_GUARD_CMOK_H

/* If we are mocking, define mocking specific macros */
#ifdef CMOK_DO_MOCK
/*! \brief Get the function pointer that will be executed when
 *         \c name() is called.
 *
 * This is mutable, so you can assign a function that replaces the
 * behavior of the original function.
 */
#define CMOK_FUN(name) name##_cmok_fun
/*! \brief Reset \c CMOK_FUN(name) to \c CMOK_FUN_DEF(name). */
#define CMOK_RESET(name)                                             \
    do {                                                             \
        CMOK_FUN(name) = CMOK_FUN_DEF(name);                         \
    } while (0)
#endif

/*! \def CMOK_FUN_DEF(name)
 *
 * \brief Get a function pointer to the original function's
 * definition.
 *
 * This is typically used for recursion in a mocked function.
 */
/*! \def CMOK_DECLARE(ret, name, args)
 *
 * \brief Forward declare a function \c name that has arguments
 *        \c args and returns \c ret.
 *
 * If \c CMOK_DO_MOCK is defined, it forward declares
 * \c CMOK_FUN(name) and \c CMOK_FUN_DEF(name).
 */
/*! \def CMOK_DEFINE(ret, name, args, names)
 *
 * \brief Define a function \c name that has arguments \c args and
 *        returns \c ret.
 *
 * If \c CMOK_DO_MOCK is defined, it forward declares
 * \c CMOK_FUN(name) and \c CMOK_FUN_DEF(name).
 *
 * \arg \c ret is the return value of the function.
 * \arg \c name is the name of the function.
 * \arg \c args is a parenthesized list of parameters (typed
 *      arguments).
 * \arg \c names is a parenthesized list of arguments.
 */

/* Define macros for both modes */
#ifdef CMOK_DO_MOCK
#define CMOK_FUN_DEF(name) name##_cmok_fun_def
#define CMOK_DECLARE(ret, name, args)                                \
    extern ret (*CMOK_FUN(name)) args;                               \
    ret CMOK_FUN_DEF(name) args;                                     \
    ret name args
#define CMOK_DEFINE(ret, name, args, names)                          \
    /* prototype in case not previously included */                  \
    ret CMOK_FUN_DEF(name) args;                                     \
    ret (*CMOK_FUN(name)) args = CMOK_FUN_DEF(name);                 \
    ret name args { return CMOK_FUN(name) names; }                   \
    ret CMOK_FUN_DEF(name) args
#define CMOK_DEFINEv(name, args, names)                              \
    /* prototype in case not previously included */                  \
    void CMOK_FUN_DEF(name) args;                                    \
    void (*CMOK_FUN(name)) args = CMOK_FUN_DEF(name);                \
    void name args { CMOK_FUN(name) names; }                         \
    void CMOK_FUN_DEF(name) args
#else
#define CMOK_FUN_DEF(name) name
#define CMOK_DECLARE(ret, name, args) ret name args;
#define CMOK_DEFINE(ret, name, args, names) ret name args
#define CMOK_DEFINEv(name, args, names) void name args
#endif

/*! \brief Useful alias for \c CMOK_FUN_DEF that expresses the intent
 *         of using recursion.
 */
#define CMOK_REC(name) CMOK_FUN_DEF(name)

/* These macros work based on the specific macro for CMOK_DEFINE */
#define CMOK_DEFINE0(ret, name) CMOK_DEFINE(ret, name, (void), ())
#define CMOK_DEFINE1(ret, name, t1, n1)                              \
    CMOK_DEFINE(ret, name, (t1 n1), (n1))
#define CMOK_DEFINE2(ret, name, t1, n1, t2, n2)                      \
    CMOK_DEFINE(ret, name, (t1 n1, t2 n2), (n1, n2))
#define CMOK_DEFINE3(ret, name, t1, n1, t2, n2, t3, n3)              \
    CMOK_DEFINE(ret, name, (t1 n1, t2 n2, t3 n3), (n1, n2, n3))
#define CMOK_DEFINE4(ret, name, t1, n1, t2, n2, t3, n3, t4, n4)      \
    CMOK_DEFINE(ret, name, (t1 n1, t2 n2, t3 n3, t4 n4),             \
                (n1, n2, n3, n4))
#define CMOK_DEFINE5(ret, name, t1, n1, t2, n2, t3, n3, t4, n4, t5,  \
                     n5)                                             \
    CMOK_DEFINE(ret, name, (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5),      \
                (n1, n2, n3, n4, n5))
#define CMOK_DEFINE6(ret, name, t1, n1, t2, n2, t3, n3, t4, n4, t5,  \
                     n5, t6, n6)                                     \
    CMOK_DEFINE(ret, name,                                           \
                (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6),          \
                (n1, n2, n3, n4, n5, n6))
#define CMOK_DEFINE7(ret, name, t1, n1, t2, n2, t3, n3, t4, n4, t5,  \
                     n5, t6, n6, t7, n7)                             \
    CMOK_DEFINE(ret, name,                                           \
                (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6, t7 n7),   \
                (n1, n2, n3, n4, n5, n6, n7))
#define CMOK_DEFINE8(ret, name, t1, n1, t2, n2, t3, n3, t4, n4, t5,  \
                     n5, t6, n6, t7, n7, t8, n8)                     \
    CMOK_DEFINE(ret, name, (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5,       \
                            t6 n6, t7 n7, t8 n8),                    \
                (n1, n2, n3, n4, n5, n6, n7, n8))
#define CMOK_DEFINE9(ret, name, t1, n1, t2, n2, t3, n3, t4, n4, t5,  \
                     n5, t6, n6, t7, n7, t8, n8, t9, n9)             \
    CMOK_DEFINE(ret, name, (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5,       \
                            t6 n6, t7 n7, t8 n8, t9 n9),             \
                (n1, n2, n3, n4, n5, n6, n7, n8, n9))

#define CMOK_DEFINEv0(name) CMOK_DEFINEv(name, (void), ())
#define CMOK_DEFINEv1(name, t1, n1) CMOK_DEFINEv(name, (t1 n1), (n1))
#define CMOK_DEFINEv2(name, t1, n1, t2, n2)                          \
    CMOK_DEFINEv(name, (t1 n1, t2 n2), (n1, n2))
#define CMOK_DEFINEv3(name, t1, n1, t2, n2, t3, n3)                  \
    CMOK_DEFINEv(name, (t1 n1, t2 n2, t3 n3), (n1, n2, n3))
#define CMOK_DEFINEv4(name, t1, n1, t2, n2, t3, n3, t4, n4)          \
    CMOK_DEFINEv(name, (t1 n1, t2 n2, t3 n3, t4 n4), (n1, n2, n3, n4))
#define CMOK_DEFINEv5(name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5)  \
    CMOK_DEFINEv(name, (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5),          \
                 (n1, n2, n3, n4, n5))
#define CMOK_DEFINEv6(name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5,  \
                      t6, n6)                                        \
    CMOK_DEFINEv(name, (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6),   \
                 (n1, n2, n3, n4, n5, n6))
#define CMOK_DEFINEv7(name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5,  \
                      t6, n6, t7, n7)                                \
    CMOK_DEFINEv(name,                                               \
                 (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6, t7 n7),  \
                 (n1, n2, n3, n4, n5, n6, n7))
#define CMOK_DEFINEv8(name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5,  \
                      t6, n6, t7, n7, t8, n8)                        \
    CMOK_DEFINEv(name, (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6,    \
                        t7 n7, t8 n8),                               \
                 (n1, n2, n3, n4, n5, n6, n7, n8))
#define CMOK_DEFINEv9(name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5,  \
                      t6, n6, t7, n7, t8, n8, t9, n9)                \
    CMOK_DEFINEv(name, (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6,    \
                        t7 n7, t8 n8, t9 n9),                        \
                 (n1, n2, n3, n4, n5, n6, n7, n8, n9))

#endif
