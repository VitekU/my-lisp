# MY-LISP INTERPRETER

This is my implementation of an interpreter for a simplified LISP dialect.

## Build and Usage

### Compilation
Compile the interpreter with `gcc` or `clang`:

```
gcc main.c eval.c parser.c helper.c lexer.c -o mlsp
```

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

### Recursive factorial 
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
