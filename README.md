# Tree-sitter parser for Fossil Wiki markup

Work in progress.

## TODO

- Implement all Fossil Wiki supported HTML elements
- HTML elements without a closing tag, such as `<br>`
- HTML empty attribute syntax
- HTML unquoted attribute value syntax
- Case-insensitive `<verbatim>` tag names
- I should be able to do the verbatim start and end tags in `grammar.js`
  and do only the verbatim content in `scanner.c`: mark just before the "<"
  and return true if we see all of `</verbatim>`
- Ensure that nesting of content is consistent with the Fossil Wiki parser
