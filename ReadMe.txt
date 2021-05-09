Frederic Desjardins

/************* The Platypus Compiler *************/

This repo contains the work material for the course CST8152 - Compilers.

During the few months of the course, We gained an overview of how compilers translate predefined language into something understandable for itself.
The objective of the course was to build a Lexical Analyzer for the Platypus programming language. Originally created by Prof. Svillen Ranev, the documation for the platypus grammar can be found in the Documents/ folder.

The components of the Platypus compiler are the following:

The Buffer:

Using dynamic data structures with the C language to create an expanding and retracting container for a series of characters to be analyzed.

Scanner:

By creating Regular Expressions based on the language specifications for Platypus, we can implement Finite Deterministic Automaton to create a Lexical Analyzer for the Platypus Language.
 
Reads from the buffer.

The Parser:

the objective is to write a Recursive Decent Predictive Parser fro the Platypus 2.0 language and implement it into the existing Lexical Analyzer. Finally, you end up with a rudimentary PLATYPUS interpreter
