# MISRA C:2025 Guideline Re-categorization Plan

| | |
|---|---|
| Document ID | ESL-TXLA64-GRP |
| Related | [Guideline Enforcement Plan](gep.md) |
| Source of guideline list | Parasoft C/C++test 2026.1.3 rule map `MISRAC2025.xml` |

## 1. Summary

| MISRA category | Count |
|---|---|
| Mandatory | 22 |
| Required | 151 |
| Advisory | 45 |
| Disapplied | 1 |
| **Total** | **219** |

## 2. Re-categorizations

Changes from the MISRA category. Each entry gives the reason.

| Guideline | MISRA category | Project category | Rationale |
|---|---|---|---|
| Rule 15.5 | Disapplied | Disapplied | Disapplied by MISRA C:2025. Parasoft enables its checker by default, so the project configuration disables it. |

Advisory guidelines are added to this table as Disapplied only when applying
them would prevent suitable or efficient code for the port. Each addition
needs a rationale and technical lead approval.

## 3. Full guideline list

The "Checkers" column lists the Parasoft checker suffixes, for example `a`
means `MISRAC2025-RULE_x_y-a`.

| Guideline | MISRA category | Project category | Checkers | Title |
|---|---|---|---|---|| Dir 4.1 | Required | Required | a, b, c, d, e, g, h, i, j, k, l, m | Run-time failures shall be minimized |
| Dir 4.2 | Advisory | Advisory | a | All usage of assembly language should be documented |
| Dir 4.3 | Required | Required | a | Assembly language shall be encapsulated and isolated |
| Dir 4.4 | Advisory | Advisory | a, b | Sections of code should not be "commented out" |
| Dir 4.5 | Advisory | Advisory | a | Identifiers in the same name space with overlapping visibility should be typographically unambiguous |
| Dir 4.6 | Advisory | Advisory | a, b, c | typedefs that indicate size and signedness should be used in place of the basic integer types |
| Dir 4.7 | Required | Required | a, b | If a function returns error information, then that error information shall be tested |
| Dir 4.8 | Advisory | Advisory | a | If a pointer to a structure or union is never dereferenced within a translation unit, then the implementation of the object should be hidden |
| Dir 4.9 | Advisory | Advisory | a | A function should be used in preference to a function-like macro where they are interchangeable |
| Dir 4.10 | Required | Required | a | Precautions shall be taken in order to prevent the contents of a header file being included more than once |
| Dir 4.11 | Required | Required | a | The validity of values passed to library functions shall be checked |
| Dir 4.12 | Required | Required | a | Dynamic memory allocation shall not be used |
| Dir 4.13 | Advisory | Advisory | a, b, c, d, e, f | Functions which are designed to provide operations on a resource should be called in an appropriate sequence |
| Dir 4.14 | Required | Required | a, b, c, d, e, f, g, h, i, j, k, l | The validity of values received from external sources shall be checked |
| Dir 4.15 | Required | Required | a | Evaluation of floating-point expressions shall not lead to the undetected generation of infinities and NaNs |
| Dir 5.1 | Required | Required | a, b, c | There shall be no data races between threads |
| Dir 5.2 | Required | Required | a | There shall be no deadlocks between threads |
| Dir 5.3 | Required | Required | a | There shall be no dynamic thread creation |
| Rule 1.1 | Required | Required | a, b, c, d | The program shall contain no violations of the standard C syntax and constraints, and shall not exceed the implementation's translation limits |
| Rule 1.3 | Required | Required | a, b, c, d, e, f, g, h, i, j, k, l, m, n, o | There shall be no occurrence of undefined or critical unspecified behaviour |
| Rule 1.4 | Required | Required | j, k, l, m, n | Emergent language features shall not be used |
| Rule 1.5 | Required | Required | a, b, c, d, e, f, g | Obsolescent language features shall not be used |
| Rule 2.1 | Required | Required | a, b, c, d, e, f, g, h | A project shall not contain unreachable code |
| Rule 2.2 | Required | Required | a, b | A project shall not contain dead code |
| Rule 2.3 | Advisory | Advisory | a, b | A project should not contain unused type declarations |
| Rule 2.4 | Advisory | Advisory | a, b | A project should not contain unused tag declarations |
| Rule 2.5 | Advisory | Advisory | a | A project should not contain unused macro definitions |
| Rule 2.6 | Advisory | Advisory | a | A function should not contain unused label declarations |
| Rule 2.7 | Advisory | Advisory | a | A function should not contain unused parameters |
| Rule 2.8 | Advisory | Advisory | a, b, c | A project should not contain unused object definitions |
| Rule 3.1 | Required | Required | a, b, c | The character sequences /* and // shall not be used within a comment |
| Rule 3.2 | Required | Required | a | Line-splicing shall not be used in // comments |
| Rule 4.1 | Required | Required | a | Octal and hexadecimal escape sequences shall be terminated |
| Rule 4.2 | Advisory | Advisory | a | Trigraphs should not be used |
| Rule 5.1 | Required | Required | a | External identifiers shall be distinct |
| Rule 5.2 | Required | Required | a, b, c, d | Identifiers declared in the same scope and name space shall be distinct |
| Rule 5.3 | Required | Required | a, b | An identifier declared in an inner scope shall not hide an identifier declared in an outer scope |
| Rule 5.4 | Required | Required | a, b, c, d | Macro identifiers shall be distinct |
| Rule 5.5 | Required | Required | a, b | Identifiers shall be distinct from macro names |
| Rule 5.6 | Required | Required | a | A typedef name shall be a unique identifier |
| Rule 5.7 | Required | Required | a | A tag name shall be a unique identifier |
| Rule 5.8 | Required | Required | a, b | Identifiers that define objects or functions with external linkage shall be unique |
| Rule 5.9 | Advisory | Advisory | a, b | Identifiers that define objects or functions with internal linkage should be unique |
| Rule 5.10 | Required | Required | a, b, c | A reserved identifier or reserved macro name shall not be declared |
| Rule 6.1 | Required | Required | a | Bit-fields shall only be declared with an appropriate type |
| Rule 6.2 | Required | Required | a | Single-bit named bit-fields shall not be of a signed type |
| Rule 6.3 | Required | Required | a | A bit-field shall not be declared as a member of a union |
| Rule 7.1 | Required | Required | a | Octal constants shall not be used |
| Rule 7.2 | Required | Required | a | A "u" or "U" suffix shall be applied to all integer constants that are represented in an unsigned type |
| Rule 7.3 | Required | Required | a | The lowercase character "l" shall not be used in a literal suffix |
| Rule 7.4 | Required | Required | a | A string literal shall not be assigned to an object unless the object's type is "pointer to const-qualified char" |
| Rule 7.5 | Mandatory | Mandatory | a | The argument of an integer constant macro shall have an appropriate form |
| Rule 7.6 | Required | Required | a | The small integer variants of the minimum-width integer constant macros shall not be used |
| Rule 8.1 | Required | Required | a, b | Types shall be explicitly specified |
| Rule 8.2 | Required | Required | a, b, c | Function types shall be in prototype form with named parameters |
| Rule 8.3 | Required | Required | a, b, c | All declarations of an object or function shall use the same names and type qualifiers |
| Rule 8.4 | Required | Required | a, b | A compatible declaration shall be visible when an object or function with external linkage is defined |
| Rule 8.5 | Required | Required | a | An external object or function shall be declared once in one and only one file |
| Rule 8.6 | Required | Required | a, b | An identifier with external linkage shall have exactly one external definition |
| Rule 8.7 | Advisory | Advisory | a | Functions and objects should not be defined with external linkage if they are referenced in only one translation unit |
| Rule 8.8 | Required | Required | a | The static storage class specifier shall be used in all declarations of objects and functions that have internal linkage |
| Rule 8.9 | Advisory | Advisory | a | An object should be declared at block scope if its identifier only appears in a single function |
| Rule 8.10 | Required | Required | a | An inline function shall be declared with the static storage class |
| Rule 8.11 | Advisory | Advisory | a | When an array with external linkage is declared, its size should be explicitly specified |
| Rule 8.12 | Required | Required | a | Within an enumerator list, the value of an implicitly-specified enumeration constant shall be unique |
| Rule 8.13 | Advisory | Advisory | a, b | A pointer should point to a const-qualified type whenever possible |
| Rule 8.14 | Required | Required | a | The restrict type qualifier shall not be used |
| Rule 8.15 | Required | Required | a, b | All declarations of an object with an explicit alignment specification shall specify the same alignment |
| Rule 8.16 | Advisory | Advisory | a | The alignment specification of zero should not appear in an object declaration |
| Rule 8.17 | Advisory | Advisory | a | At most one explicit alignment specifier should appear in an object declaration |
| Rule 8.18 | Required | Required | a | There shall be no tentative definitions in a header file |
| Rule 8.19 | Advisory | Advisory | a, b, c, d | There should be no external declarations in a source file |
| Rule 9.1 | Mandatory | Mandatory | a | The value of an object with automatic storage duration shall not be read before it has been set |
| Rule 9.2 | Required | Required | a | The initializer for an aggregate or union shall be enclosed in braces |
| Rule 9.3 | Required | Required | a | Arrays shall not be partially initialized |
| Rule 9.4 | Required | Required | a | An element of an object shall not be initialized more than once |
| Rule 9.5 | Required | Required | a | Where designated initializers are used to initialize an array object the size of the array shall be specified explicitly |
| Rule 9.6 | Required | Required | a | An initializer using chained designators shall not contain initializers without designators |
| Rule 9.7 | Mandatory | Mandatory | a | Atomic objects shall be appropriately initialized before being accessed |
| Rule 10.1 | Required | Required | a, b, c, d, e, f, g, h | Operands shall not be of an inappropriate essential type |
| Rule 10.2 | Required | Required | a | Expressions of essentially character type shall not be used inappropriately in addition and subtraction operations |
| Rule 10.3 | Required | Required | a, b | The value of an expression shall not be assigned to an object with a narrower essential type or of a different essential type category |
| Rule 10.4 | Required | Required | a, b | Both operands of an operator in which the usual arithmetic conversions are performed shall have the same essential type category |
| Rule 10.5 | Advisory | Advisory | a, b, c | The value of an expression should not be cast to an inappropriate essential type |
| Rule 10.6 | Required | Required | a | The value of a composite expression shall not be assigned to an object with wider essential type |
| Rule 10.7 | Required | Required | a, b | If a composite expression is used as one operand of an operator in which the usual arithmetic conversions are performed then the other operand shall not have wider essential type |
| Rule 10.8 | Required | Required | a | The value of a composite expression shall not be cast to a different essential type category or a wider essential type |
| Rule 11.1 | Required | Required | a, b | Conversions shall not be performed between a pointer to a function and any other type |
| Rule 11.2 | Required | Required | a | Conversions shall not be performed between a pointer to an incomplete type and any other type |
| Rule 11.3 | Required | Required | a | A conversion shall not be performed between a pointer to object type and a pointer to a different object type |
| Rule 11.4 | Required | Required | b, c | A conversion shall not be performed between a pointer to object and an arithmetic type |
| Rule 11.5 | Advisory | Advisory | b | A conversion should not be performed from pointer to void into pointer to object |
| Rule 11.6 | Required | Required | b, c | A cast shall not be performed between pointer to void and an arithmetic type |
| Rule 11.8 | Required | Required | a | A conversion shall not remove any const, volatile or _Atomic qualification from the type pointed to by a pointer |
| Rule 11.9 | Required | Required | a, b | The macro NULL shall be the only permitted form of integer null pointer constant |
| Rule 11.10 | Required | Required | a | The _Atomic qualifier shall not be applied to the incomplete type void |
| Rule 11.11 | Required | Required | a | Pointers shall not be implicitly compared to NULL |
| Rule 12.1 | Advisory | Advisory | a, c | The precedence of operators within expressions should be made explicit |
| Rule 12.2 | Required | Required | a, b | The right hand operand of a shift operator shall lie in the range zero to one less than the width in bits of the essential type of the left hand operand |
| Rule 12.3 | Advisory | Advisory | a | The comma operator should not be used |
| Rule 12.4 | Advisory | Advisory | a, b | Evaluation of constant expressions should not lead to unsigned integer wrap-around |
| Rule 12.5 | Mandatory | Mandatory | a | The sizeof operator shall not have an operand which is a function parameter declared as "array of type" |
| Rule 12.6 | Required | Required | a | Structure and union members of atomic objects shall not be directly accessed |
| Rule 13.1 | Required | Required | a | Initializer lists shall not contain persistent side effects |
| Rule 13.2 | Required | Required | a, b, c, d, e, f, g, h | The value of an expression and its persistent side effects shall be the same under all permitted evaluation orders and shall be independent from thread interleaving |
| Rule 13.3 | Advisory | Advisory | a | A full expression containing an increment (++) or decrement (--) operator should have no other potential side effects other than that caused by the increment or decrement operator |
| Rule 13.4 | Advisory | Advisory | a | The result of an assignment operator should not be used |
| Rule 13.5 | Required | Required | a | The right hand operand of a logical && or \|\| operator shall not contain persistent side effects |
| Rule 13.6 | Required | Required | a, b, c | The operand of the sizeof operator shall not contain any expression which has potential side effects |
| Rule 14.1 | Required | Required | a, b | A loop counter shall not have essentially floating type |
| Rule 14.2 | Required | Required | a, b, c, d | A for loop shall be well-formed |
| Rule 14.3 | Required | Required | ac | Controlling expressions shall not be invariant |
| Rule 14.4 | Required | Required | a | The controlling expression of an if statement and the controlling expression of an iteration-statement shall have essentially Boolean type |
| Rule 15.1 | Advisory | Advisory | a | The goto statement should not be used |
| Rule 15.2 | Required | Required | a | The goto statement shall jump to a label declared later in the same function |
| Rule 15.3 | Required | Required | a | Any label referenced by a goto statement shall be declared in the same block, or in any block enclosing the goto statement |
| Rule 15.4 | Advisory | Advisory | a | There should be no more than one break or goto statement used to terminate any iteration statement |
| Rule 15.5 | Disapplied | Disapplied | a | A function should have a single point of exit at the end |
| Rule 15.6 | Required | Required | a, b | The body of an iteration-statement or a selection-statement shall be a compound-statement |
| Rule 15.7 | Required | Required | a | All if ... else if constructs shall be terminated with an else statement |
| Rule 16.1 | Required | Required | a, b, e, f, g, h, i | All switch statements shall be well-formed |
| Rule 16.2 | Required | Required | a | A switch label shall only be used when the most closely-enclosing compound statement is the body of a switch statement |
| Rule 16.3 | Required | Required | c | Every switch-clause shall be appropriately terminated |
| Rule 16.4 | Required | Required | a, b | Every switch statement shall have a default label |
| Rule 16.5 | Required | Required | a | A default label shall appear as either the first or the last switch label of a switch statement |
| Rule 16.6 | Required | Required | a | Every switch statement shall have at least two switch-clauses |
| Rule 16.7 | Required | Required | a, b | A switch-expression shall not have essentially Boolean type |
| Rule 17.1 | Required | Required | a, b, c | The standard header file <stdarg.h> shall not be used |
| Rule 17.2 | Required | Required | b | Functions shall not call themselves, either directly or indirectly |
| Rule 17.3 | Mandatory | Mandatory | a | A function shall not be declared implicitly |
| Rule 17.4 | Mandatory | Mandatory | a, b | All exit paths from a function with non-void return type shall have an explicit return statement with an expression |
| Rule 17.5 | Required | Required | a | The function argument corresponding to a parameter declared to have an array type shall have an appropriate number of elements |
| Rule 17.7 | Required | Required | a, b | The value returned by a function having non-void return type shall be used |
| Rule 17.8 | Advisory | Advisory | a | A function parameter should not be modified |
| Rule 17.9 | Mandatory | Mandatory | a | A function declared with a _Noreturn function specifier shall not return to its caller |
| Rule 17.10 | Required | Required | a | A function declared with a _Noreturn function specifier shall have void return type |
| Rule 17.11 | Advisory | Advisory | a | A function that never returns should be declared with a _Noreturn function specifier |
| Rule 17.12 | Advisory | Advisory | a | A function identifier should only be used with either a preceding &, or with a parenthesized parameter list |
| Rule 17.13 | Required | Required | a | A function type shall not be type qualified |
| Rule 18.1 | Required | Required | a, b, c | A pointer resulting from arithmetic on a pointer operand shall address an element of the same array as that pointer operand |
| Rule 18.2 | Required | Required | a | Subtraction between pointers shall only be applied to pointers that address elements of the same array |
| Rule 18.3 | Required | Required | a | The relational operators >, >=, < and <= shall not be applied to expressions of pointer type except where they point into the same object |
| Rule 18.4 | Advisory | Advisory | a | The +, -, += and -= operators should not be applied to an expression of pointer type |
| Rule 18.5 | Advisory | Advisory | a | Declarations should contain no more than two levels of pointer nesting |
| Rule 18.6 | Required | Required | a, b, c | The address of an object with automatic or thread-local storage shall not be copied to another object that persists after the first object has ceased to exist |
| Rule 18.7 | Required | Required | a | Flexible array members shall not be declared |
| Rule 18.8 | Required | Required | a | Variable-length arrays shall not be used |
| Rule 18.9 | Required | Required | a | An object with temporary lifetime shall not undergo array-to-pointer conversion |
| Rule 18.10 | Mandatory | Mandatory | a | Pointers to variably-modified array types shall not be used |
| Rule 19.1 | Mandatory | Mandatory | a, b, c | An object shall not be assigned or copied to an overlapping object |
| Rule 19.2 | Advisory | Advisory | a | The union keyword should not be used |
| Rule 19.3 | Required | Required | a | A union member shall not be read unless it has been previously set |
| Rule 20.1 | Advisory | Advisory | a | #include directives should only be preceded by preprocessor directives or comments |
| Rule 20.2 | Required | Required | a, b | The ', " or \ characters and the /* or // character sequences shall not occur in a header file name |
| Rule 20.3 | Required | Required | a | The #include directive shall be followed by either a <filename> or "filename" sequence |
| Rule 20.4 | Required | Required | a, b | A macro shall not be defined with the same name as a keyword |
| Rule 20.5 | Advisory | Advisory | a | #undef should not be used |
| Rule 20.6 | Required | Required | a | Tokens that look like a preprocessing directive shall not occur within a macro argument |
| Rule 20.7 | Required | Required | b | Expressions resulting from the expansion of macro parameters shall be appropriately delimited |
| Rule 20.8 | Required | Required | a | The controlling expression of a #if or #elif preprocessing directive shall evaluate to 0 or 1 |
| Rule 20.9 | Required | Required | b | All identifiers used in the controlling expression of #if or #elif preprocessing directives shall be #define'd before evaluation |
| Rule 20.10 | Advisory | Advisory | a | The # and ## preprocessor operators should not be used |
| Rule 20.11 | Required | Required | a | A macro parameter immediately following a # operator shall not immediately be followed by a ## operator |
| Rule 20.12 | Required | Required | a | A macro parameter used as an operand to the # or ## operators, which is itself subject to further macro replacement, shall only be used as an operand to these operators |
| Rule 20.13 | Required | Required | a | A line whose first token is # shall be a valid preprocessing directive |
| Rule 20.14 | Required | Required | a | All #else, #elif and #endif preprocessor directives shall reside in the same file as the #if, #ifdef or #ifndef directive to which they are related |
| Rule 20.15 | Required | Required | a, b, c, d | #define and #undef shall not be used on a reserved identifier or reserved macro name |
| Rule 21.3 | Required | Required | a | The memory allocation and deallocation functions of <stdlib.h> shall not be used |
| Rule 21.4 | Required | Required | a, b | The standard header file <setjmp.h> shall not be used |
| Rule 21.5 | Required | Required | a, b | The standard header file <signal.h> shall not be used |
| Rule 21.6 | Required | Required | a | The Standard Library input/output functions shall not be used |
| Rule 21.7 | Required | Required | a | The Standard Library functions atof, atoi, atol and atoll of <stdlib.h> shall not be used |
| Rule 21.8 | Required | Required | a, b, c, d | The Standard Library termination functions of <stdlib.h> shall not be used |
| Rule 21.9 | Required | Required | a | The Standard Library functions bsearch and qsort of <stdlib.h> shall not be used |
| Rule 21.10 | Required | Required | a, b, c | The Standard Library time and date functions shall not be used |
| Rule 21.11 | Advisory | Advisory | a | The standard header file <tgmath.h> should not be used |
| Rule 21.12 | Required | Required | a, b | The standard header file <fenv.h> shall not be used |
| Rule 21.13 | Mandatory | Mandatory | a | Any value passed to a function in <ctype.h> shall be representable as an unsigned char or be the value EOF |
| Rule 21.14 | Required | Required | a | The Standard Library function memcmp shall not be used to compare null terminated strings |
| Rule 21.15 | Required | Required | a | The pointer arguments to the Standard Library functions memcpy, memmove and memcmp shall be pointers to qualified or unqualified versions of compatible types |
| Rule 21.16 | Required | Required | a | The pointer arguments to the Standard Library function memcmp shall point to either a pointer type, an essentially signed type, an essentially unsigned type, an essentially Boolean type or an essentially enum type |
| Rule 21.17 | Mandatory | Mandatory | a, b | Use of the string handling functions from <string.h> shall not result in accesses beyond the bounds of the objects referenced by their pointer parameters |
| Rule 21.18 | Mandatory | Mandatory | a | The size_t argument passed to any function in <string.h> shall have an appropriate value |
| Rule 21.19 | Mandatory | Mandatory | a, b | The pointers returned by the Standard Library functions localeconv, getenv, setlocale or strerror shall only be used as if they have pointer to const-qualified type |
| Rule 21.20 | Mandatory | Mandatory | a | The pointer returned by the Standard Library functions asctime, ctime, gmtime, localtime, localeconv, getenv, setlocale or strerror shall not be used following a subsequent call to the same function |
| Rule 21.21 | Required | Required | a | The Standard Library function system of <stdlib.h> shall not be used |
| Rule 21.22 | Mandatory | Mandatory | a | All operand arguments to any type-generic macros declared in <tgmath.h> shall have an appropriate essential type |
| Rule 21.23 | Required | Required | a | All operand arguments to any multi-argument type-generic macros declared in <tgmath.h> shall have the same standard type |
| Rule 21.24 | Required | Required | a | The random number generator functions of <stdlib.h> shall not be used |
| Rule 21.25 | Required | Required | a | All memory synchronization operations shall be executed in sequentially consistent order |
| Rule 21.26 | Required | Required | a | The Standard Library function mtx_timedlock() shall only be invoked on mutex objects of appropriate mutex type |
| Rule 22.1 | Required | Required | a | All resources obtained dynamically by means of Standard Library functions shall be explicitly released |
| Rule 22.2 | Mandatory | Mandatory | a, b | A block of memory shall only be freed if it was allocated by means of a Standard Library function |
| Rule 22.3 | Required | Required | a | The same file shall not be open for read and write access at the same time on different streams |
| Rule 22.4 | Mandatory | Mandatory | a | There shall be no attempt to write to a stream which has been opened as read-only |
| Rule 22.5 | Mandatory | Mandatory | a, b | A pointer to a FILE object shall not be dereferenced |
| Rule 22.6 | Mandatory | Mandatory | a | The value of a pointer to a FILE shall not be used after the associated stream has been closed |
| Rule 22.7 | Required | Required | a | The macro EOF shall only be compared with the unmodified return value from any Standard Library function capable of returning EOF |
| Rule 22.8 | Required | Required | a | The value of errno shall be set to zero prior to a call to an errno-setting-function |
| Rule 22.9 | Required | Required | a | The value of errno shall be tested against zero after calling an errno-setting-function |
| Rule 22.10 | Required | Required | a | The value of errno shall only be tested when the last function to be called was an errno-setting-function |
| Rule 22.11 | Required | Required | a | A thread that was previously either joined or detached shall not be subsequently joined nor detached |
| Rule 22.12 | Mandatory | Mandatory | a | Thread objects, thread synchronization objects, and thread-specific storage pointers shall only be accessed by the appropriate Standard Library functions |
| Rule 22.13 | Required | Required | a | Thread objects, thread synchronization objects and thread-specific storage pointers shall have appropriate storage duration |
| Rule 22.14 | Mandatory | Mandatory | a, b | Thread synchronization objects shall be initialized before being accessed |
| Rule 22.15 | Required | Required | a | Thread synchronization objects and thread-specific storage pointers shall not be destroyed until after all threads accessing them have terminated |
| Rule 22.16 | Required | Required | a | All mutex objects locked by a thread shall be explicitly unlocked by the same thread |
| Rule 22.17 | Required | Required | a | No thread shall unlock a mutex or call cnd_wait() or cnd_timedwait() for a mutex it has not locked before |
| Rule 22.18 | Required | Required | a | Non-recursive mutexes shall not be recursively locked |
| Rule 22.19 | Required | Required | a | A condition variable shall be associated with at most one mutex object |
| Rule 22.20 | Mandatory | Mandatory | a | Thread-specific storage pointers shall be created before being accessed |
| Rule 23.1 | Advisory | Advisory | a, b | A generic selection should only be expanded from a macro |
| Rule 23.2 | Required | Required | a | A generic selection that is not expanded from a macro shall not contain potential side effects in the controlling expression |
| Rule 23.3 | Advisory | Advisory | a | A generic selection should contain at least one non-default association |
| Rule 23.4 | Required | Required | a | A generic association shall list an appropriate type |
| Rule 23.5 | Advisory | Advisory | a | A generic selection should not depend on implicit pointer type conversion |
| Rule 23.6 | Required | Required | a, b | The controlling expression of a generic selection shall have an essential type that matches its standard type |
| Rule 23.7 | Advisory | Advisory | a | A generic selection that is expanded from a macro should evaluate its argument only once |
| Rule 23.8 | Required | Required | a | A default association shall appear as either the first or the last association of a generic selection |
