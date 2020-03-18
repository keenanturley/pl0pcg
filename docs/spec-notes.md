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

## Errata

- Appendix E
  - `char name[10];` should be `char name[12];`
  - Add `mark` to symbol struct
