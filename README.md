# Symboless: Lightweight Scripting Language
Symboless is a lightweight, interpreted scripting language designed for simplicity, flexibility, and ease of integration with C++ applications. Whether you're looking for a standalone scripting tool or need a seamless way to embed scripting capabilities into your C++ project, Symboless has you covered.

## Key Features
Customizable Keywords: Define your own keywords using a simple JSON configuration file.

Three Data Types: Supports Integer, Text, and Real, with arrays for each type.

Flow Control: Includes branching, looping, and reusable blocks of code (subroutines).

Human-Readable Syntax: Easy-to-understand syntax for both beginners and experts.

Error Checking: Built-in error checking for smoother debugging.

Lightweight: Just 50KB in size.

Easy C++ Integration: Integrate Symboless into C++ applications with a single header file, symls.hpp.

Comprehensive Documentation: Full documentation available on the official website.

## How It Works
Symboless can run as an independent scripting environment or be integrated into C++ applications:

#### Command-Line Use: Run the interpreter interactively, executing one line of code at a time.
#### Script Files: Run pre-written scripts saved with the .symls extension.
#### C++ Integration: Easily embed Symboless in C++ projects with the provided header file, symls.hpp, to run scripts within your C++ code.

## Installation
#### Standalone Use (Command-Line):
Download the symboless interpreter.

Place it in a directory on your system.

Run it directly from the command line with .symls scripts or one-line commands.

#### C++ Integration:
Download the symls.hpp header file.
Include it in your C++ project.
Link with the necessary Symboless runtime files to execute scripts.
For detailed integration instructions, refer to the documentation.

Example Usage
Command-Line:

```
> symls
print "Hello World!"
Hello World!
end
>
```

```
#include<iostream>
#include<string>
#include "symls.hpp"
int main() {
    std::string input = "1 2";
    std::string code = "let integer a and read a and let integer b and read b and let integer c is a plus b and print c";
    std::string output = "";
    symboless::Symboless s(code,input,output);
    std::cout<<output;
    return 0;
}
```

#### Documentation : https://ansh1406.github.io/Symboless-Documentation

License
This project is licensed under the MIT License - see the LICENSE file for details.

