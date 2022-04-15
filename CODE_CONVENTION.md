# Coding Conventions

For better readability of codes, there are coding conventions to follow.
Other dependent codes (library such as libhangul) are not applied by this convention.

## Indent

* Use 4 spaces for indentation.
* Tabs aren't allowed whether it generates compile errors.

## Identifiers

* Use `snake_case` for functions, variables names.
* Use `CONSTANT_CASE` for constant variables, constant macros, enums.
* Use `PascalCase` for defining struct(class) type names.
* Other casing rules can be used in their circumstances, but prefer rules above.
  * GObject, GLib may use PascalCase for struct(class) type names.
  * GoogleTest prefers not to use underscore(`_`) in function/test namings.

## Spacing

### Horizontal spacing

* Split control-flow statements, conditions, bracket with spaces.
* Do not split function name and its parameter definition with spaces.

### Vertical spacing (line breaks)

* Use single line break to split paragraph in function code blocks.
* Use double line break between function definitions for better distinguish.
* Declaring multiple functions and variables can skip line break by its context.
* Try to write all keywords on defining/declaring function and its signature.
  * Try to write `extern`/`static`, `inline`, return type and function name in same line.
  * When the signature is too long to represent in single line, try to break in parameters.
* For unary operators such as `&`, `++`, `*`(pointer dereference), do not separate.
* For binary operators such as `+`, `-`, `==`, separate with spaces.

### Bracket `{`, `}`

* For function, structure, union, class and enums:  
  opening and closing brackets must be placed on separate line.
* For initializing struct, array:  
  opening and closing brackets must be placed on separate line.  
  When the initialization is short enough to place in single line, it's allowed.
* For control-flow statements (ex. `if`, `for`, `while`...):  
  opening bracket must be placed on same line.  
  on following `else`, `else if` statements, closing bracket must be placed on same line.
  (ex. `} else {`)
* When the control-flow handles single line expression, bracket can be skipped.  
  But, one of `if`, `else if` and `else` has multiple line expression,
  use bracket even the others handle single line expression.

## Comments

* Both C style (`/* ... */`) and C++ style (`// ...`) comments are allowed.
* For consistency, try to use each styles for:
  * Use C style for multiple line comments. Such as documentation, long descriptions.
  * Use C++ style for single line comments. Such as supplimentary, short descriptions.
* Use space after/before comment specifiers.

