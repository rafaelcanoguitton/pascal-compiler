# Pascal Compiler

The present compiler is being made as the Compilers (CS342) course project, the objective is to build a simplified compiler for Pascal in a proof of concepts given in class manner.

## Stage

The current stage of the compiler is having the Lexical Analyzer done.

## Build

To build the project you only need **CMake** and a working **gcc** compiler as it's being written in **C++**.

Build it issuing the following:

```bash
cmake <Your preferred destination. e.g: build>
```

## Running

To run the current lexical analyzer simply do:

```bash
./pascal-compiler --compile <filename>
```

The compile flag will take a filename as it's argument but you can also do:

```bash
./pascal-compiler <filename>
```

## Features

As the current list of features we have the following:

- **¡Colors!** - The compiler will print the tokens in a colorful manner for visual aid.

- **--verbose** - Activates the verbose mode in order to provide additional details.


