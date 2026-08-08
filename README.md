# Badger++

Badger++ is a small experimental programming language implemented in modern C++. Source code is tokenized, parsed into an abstract syntax tree, compiled to bytecode, and executed by a stack-based virtual machine.

The project is intended as a hands-on exploration of language implementation, including parsing, AST design, bytecode generation, lexical scopes, and virtual-machine execution.

## Features

- Integer, floating-point, boolean, and string values
- Variable declarations, assignments, and initialization checks
- Arithmetic and unary operators
- Numeric, boolean, and string comparisons
- Logical expressions
- `if`, `else if`, and `else` control flow
- `while` loops
- Nested block scopes and variable shadowing
- String concatenation
- `print` and `println` output
- Bytecode compilation and stack-based execution

## Requirements

- A C++17-compatible compiler
- CMake 3.10 or newer

The project has primarily been developed on Windows, but it uses standard C++ and CMake.

## Build

From the repository root:

```powershell
cmake -S . -B build
cmake --build build
```

With a single-configuration generator, the executable is usually created directly in `build`. With a multi-configuration generator such as Visual Studio, it may be placed in `build/Debug`.

## Run

Pass a Badger++ source file as the first command-line argument.

Windows with a single-configuration generator:

```powershell
.\build\Badger++.exe examples\main.bg
```

Windows with a Visual Studio generator:

```powershell
.\build\Debug\Badger++.exe examples\main.bg
```

Linux or macOS:

```bash
./build/Badger++ examples/main.bg
```

## Language Overview

Badger++ statements end with a semicolon. Control-flow blocks do not require a semicolon after the closing brace.

### Values and variables

Variables are declared with `let`. Their value type is determined at runtime.

```text
let count = 10;
let price = 12.5;
let message = "Hello from Badger++";
let enabled = true;

count = count + 1;
```

A variable can be declared without a value, but it cannot be read before it is initialized:

```text
let result;
result = 42;
println(result);
```

### Output

`print` writes a value without a trailing newline. `println` adds a newline.

```text
print("Result: ");
println(42);
```

### Arithmetic

```text
let result = (2 + 3) * 4 - 10 / 2;
let negative = -result;
let text = "Badger" + "++";
```

Arithmetic can mix integer and floating-point values. Addition also supports concatenating two strings.

### Comparisons and logic

```text
let inRange = result > 5 && result < 20;
let matches = result == 15;
let different = result != 10;
let disabled = !inRange;
```

Conditions must evaluate to a boolean value.

### Conditionals

```text
if(result > 20) {
    println("Large result");
}
else if(result == 20) {
    println("Exactly twenty");
}
else {
    println("Small result");
}
```

### Loops

```text
let counter = 1;

while(counter < 6) {
    println(counter);
    counter = counter + 1;
}
```

### Scopes and shadowing

Every block creates a lexical scope. A variable declared in an inner scope may shadow a variable with the same name from an outer scope. Leaving the block restores access to the outer variable.

```text
let message = "outer";

if(true) {
    let message = "inner";
    println(message);
}

println(message);
```

This prints `inner` followed by `outer`.

## Operator Precedence

Operators are parsed in the following order, from lowest to highest precedence:

| Precedence | Operators | Description |
| --- | --- | --- |
| Lowest | `||` | Logical OR |
|  | `&&` | Logical AND |
|  | `==`, `!=` | Equality |
|  | `>`, `<` | Numeric comparison |
|  | `+`, `-` | Addition, subtraction, string concatenation |
|  | `*`, `/` | Multiplication and division |
| Highest | `!`, unary `-` | Logical NOT and numeric negation |

Parentheses can be used to override precedence:

```text
let value = (2 + 3) * 4;
let condition = !(value < 10 || value == 15);
```

## Examples

The [`examples`](examples) directory contains several runnable programs:

| Program | Demonstrates |
| --- | --- |
| [`main.bg`](examples/main.bg) | General language demonstration |
| [`factorial.bg`](examples/factorial.bg) | Iterative factorial calculation |
| [`finobacci.bg`](examples/finobacci.bg) | Fibonacci sequence generation |
| [`fizzbuzz.bg`](examples/fizzbuzz.bg) | Branching and loop state |
| [`primes.bg`](examples/primes.bg) | Nested loops and prime detection |
| [`collatz.bg`](examples/collatz.bg) | Collatz sequence |
| [`compound-interest.bg`](examples/compound-interest.bg) | Floating-point calculations |

Run any example by passing its path to the executable:

```powershell
.\build\Badger++.exe examples\fizzbuzz.bg
```

## Implementation Pipeline

Badger++ processes a program in four stages:

1. **Lexer** - converts source text into tokens.
2. **Parser** - applies precedence rules and constructs an inheritance-based AST.
3. **Compiler** - resolves variables and scopes, performs semantic checks, and emits bytecode.
4. **Virtual machine** - executes the bytecode using a value stack and variable storage.

The main implementation files are organized as follows:

```text
include/             Public declarations
src/lexer.cpp        Tokenization
src/parser.cpp       Parsing and AST construction
src/astnodes.cpp     AST bytecode compilation
src/compiler.cpp     Compiler state and bytecode management
src/interpreter.cpp  Stack-based bytecode execution
main.cpp             Command-line entry point
examples/            Example Badger++ programs
```

## Current Limitations

Badger++ is an educational work in progress rather than a production language. Current limitations include:

- No user input
- No arrays, collections, or user-defined types
- No comments in source files
- No modulo, `<=`, or `>=` operators
- No static type system
- Runtime errors currently terminate program execution
- Boolean output uses the underlying C++ stream representation and appear as `1` or `0`

## Functions

Function support has been partially created and may be completed in the future. The lexer, parser, AST nodes, function table, and initial `CALL`/`RETURN` bytecode generation are present, but runtime call frames, parameter binding, function-local storage, and returning control to the caller are not yet implemented in the virtual machine. Function declarations and calls should therefore be considered experimental and are not part of the currently supported runnable feature set.
