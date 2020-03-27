# Specification Notes

## Input / Output

We need to take the output from the lexicographical analyzer (LA) and parse tokens from it.

### Example

For the following source file:

```pascal
var x, y;
begin
    x := y + 56;
end.
```

The LA will produce this lexeme list:

`29 2 x 17 2 y 18 21 2 x 20 2 y 4 3 56 18 22 19`

The general form of a lexeme is the symbol type (an integer) which in some cases is followed by the literal (if symbol == 3) or identifier name (if symbol == 2).

The parser should go through that list and determine if the lexemes form a valid PL/0 program. To do this, the list should be fed through a recursive descent function that conforms to the language specification detailed in Appendix B.

## Parsing Technique

- Initialize the parser's token cursor to `0`. This is the first symbol to be looked at.
- Only advance the token cursor when we want to consume a symbol.
- If no error is encountered during parsing, the program is considered valid.
- Look at current token for an optional symbol, otherwise get the next token.
- After parsing a piece of grammer, the current token should be the token immediately following where that piece of grammar finished.
- These are equivalent:
  
  ```c
  next_token(parser);
  token *t = current_token(parser);
  ```

  ```c
  token *t = next_token(parser);
  ```

## Errata

There are several errors in the specification, which are documented below

- Appendix A
  - First symbolic representation should have `semicolonsym`, not `semicolon`
  - Second symbolic representation should have `varsym`, not `intsym`

- Appendix B
  - `constdeclaration` should be `const-declaration`

- Appendix C
  - Errors 4, 6, 12, 23: Procedures do not exist in this assignment
  - Error 14, 15: Calls do not exist in this assignment

- Appendix D
  - References to procedure and call symbols are made in this section, but they should not exist for this assignment
  - An `intsym` is used, where `varsym` should be
  - These could be permissible considering the title of this section describes the parser as being for a "PL/0 like programming language", and not our specific variant

- Appendix E
  - `char name[10];` should be `char name[12];`
  - Add `mark` to symbol struct
  - `MAX_SYMBOL_TABLE_SIZE` should be 200
