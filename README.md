# MY-LISP INTERPRETER

This is my implementation of an interpreter for a simplified LISP dialect.

## Build and Usage

### Compilation
Create a build directory, `mkdir build`, and `cd` into it. Then run `cmake ..` and `make`. 

### Running the REPL
Run `./mlsp` without arguments to enter the REPL  environment:

```
./mlsp
>>> (+ 1 2)
3
>>> (def my-var 5)
>>> (* my-var (+ 2 3))
25
```

### Running a File
Pass a source file as the first argument to run the code in the file.

```
./mlsp factorial.lisp
```

---

## Language Concepts

In MY-LISP everything is an expression that gets evaluated to a `Value` type. Numbers and strings evaluate directly to themselves.<br> Symbols have assigned values in the current environment. The most important expression is the `list`. It is an expression that gets evaluated based on the arguments that it contains.
Below are listed the supported data types, primitives and builtin functions with examples for better understanding.

## Supported Data Types

| Type | Syntax / Examples | Description |
| :--- | :--- | :--- |
| `number` | `1`, `-2`, `0` | Signed integer values. |
| `string` | `"hello"`, `"world"` | Quoted strings. |
| `symbol` | `x`, `add`, `+` | Symbols that are resolved to their assigned value in the environment. |
| `list` | `(+ 1 2)`, `(def x 1)` |  Expressions enclosed in parenthesis'. |
| `function` | `(fn (x) (* x x))` |  Function value that internally captures the parameters and the function body to execute upon calling it. |
| `nil` | `()` | Empty lists resolved to the `nil` value. |

---

## Primitives and Builtin Functions

| Function | Syntax | Number of arguments | Returns | Description |
| :--- | :--- | :---: | :--- | :--- |
| `def` | `(def <symbol> <expr>)` | 2 | Value of `<expr>` | Evaluates `<expr>` and registers the `key:value` pair `<symbol>:<expr>` to the current environment. |
| `fn` | `(fn (<params...>) <body>)` | 2 | `function` | Constructs a function that captures parameters, the function body, and the current environment. |
| `if` | `(if <cond> <then> [<else>])` | 2 or 3 | Value of chosen branch or `nil` | Evaluates `<cond>`. If truthy (`1` or `"true"`), evaluates and returns `<then>`. Otherwise, evaluates and returns `<else>`. |
| `==` | `(== <arg1> <arg2>)` | 2 | `1` or `0` | Evaluates both arguments. Returns `1` if equal, `0` if unequal. Arguments must be of the same type (`number` or `string`). |
| `>` | `(> <arg1> <arg2>)` | 2 | `1` or `0` | Evaluates both arguments. Returns `1` if `arg1 > arg2`, `0` otherwise. Compares numerically for numbers and via `strcmp` for strings. Arguments must be of the same type (`number` or `string`). |
| `<` | `(< <arg1> <arg2>)` | 2 | `1` or `0` | Evaluates both arguments. Returns `1` if `arg1 < arg2`, `0` otherwise. Compares numerically for numbers and via `strcmp` for strings. Arguments must be of the same type (`number` or `string`). |
| `+` | `(+ [expr ...])` | 0 or more | `number` | Evaluates all argument expressions and computes their sum. Evaluates to `0` when called with no arguments. |
| `-` | `(- <first> [expr ...])` | 0 or more | `number` | Takes the first expression and substracts every following expression. Evaluates to `0` with no arguments. |
| `*` | `(* [expr ...])` | 0 or more | `number` | Evaluates all argument expressions and computes their product. Evaluates to `1` when called with no arguments. |
| `/` | `(/ <first> [expr ...])` | 0 or more | `number` | Takes the first expression and divides it by every following expression. Evaluates to `0` with no arguments. |

---

## Code Examples

More code examples are located in the folder *examples*.

### Factorial 
```
(def fact
    (fn (x)
        (if (== x 0)
            1
            (* x (fact (- x 1))))))

(fact 5)
```

## Inner structure of the program

### Overview
The interpreter logic is split into 3 steps. First the `Lexer` has to split the input text into a stream of `Tokens`. <br>
These tokens are then piped into the `Parser` which builds an `AST` from them. The tree is then traversed recursively with the `eval` function that evaluates the expressions in the tree and finally returns a value that is printed to the output.

The `eval` function also takes an `Environment` parameter that serves as a large dictionnary of symbols that are bound to their respective values.

### Lexer

The `Lexer` gets its input as a stream of chars `char*`. It then goes one character at a time and groups them into `Tokens` of different types - listed below. 

```
typedef enum {
    T_LPAREN,
    T_RPAREN,
    T_STRING,
    T_NUMBER,
    T_SYMBOL,
    T_EOF
} TokenType;
```

### Parser

The `Parser` takes the `Tokens` produced by the `Lexer` (the `Parser` struct actually takes the `Lexer` and then gets the `Token`s on demand as it does the parsing step - this is called lazy lexing I believe) and forms them into an abstract syntax tree. <br>This step is simpler in LISP compared to the other languages (like Python or C) because the program flow is directly tied to the nesting of the `list` type expressions. <br>
This means, that the AST created is a structure that can be described as expressions of which the expressions of the type `list` are nested in other `list` expressions. We can imagine this as a list of list in a language like Python. <br>
The result is an AST that consist of the `Node` structures that can either represent a `number`, `string`, `symbol` or a `list`, the latter of those contains more nested `Node` structs as described above.

```
typedef struct Node {
    NodeType type;
    union {
        int number;
        char* string;
        char* symbol;
        struct {
            struct Node **elements;
            size_t count;
            size_t capacity;
        } list;
    };
} Node;
```

The `parse_expression` function used for the parsing step returns the root of the AST which is then passed in the `eval` function.

### Eval 

The `eval` function takes the root of the AST and recursively evaluates the nested `Node` structures. Each expression evaluates to a `Value` type of the following structure.
```
typedef struct {
    ValueType type;
    union {
        int number;
        char *string;
        Function fn;
    };
} Value;
```
The expressions of type `list` are evaluated based on their first argument. These are the primitive symbols and builtin functions as described in the table in this [section](#primitives-and-builtin-functions).
When using the `def` keyword, the following arguement serves a key to the next expression that is evaluated. This key value pair is then registered in the current environment. 

### Environment

The `Environment` is a simple array of `Pairs` and a pointer to a parent environment. The lookup of keys is therefore done in linear time.
```
typedef struct Pair {
    char *key;
    Value *value;
} Pair;

struct Environment {
    struct Environment *parent;
    Pair *pairs;
    size_t count;
    size_t capacity;
};
```

### Arena Allocator

In my language the memory is managed with an *Arena allocator*. This means that the interpreter preallocates a buffer of given size (default is 4 MB) and the memory is then allocated from this buffer. When the program finishes running, that buffer is freed.  
___
My two primary sources of knowledge for this project were the book [Crafting Interpreters](https://craftinginterpreters.com) by Robert Nystrom
and [this blogpost](https://zstix.io/posts/make-a-lisp-in-python/) by Zack.
