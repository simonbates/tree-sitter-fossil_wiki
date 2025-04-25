# Tree-sitter parser for Fossil Wiki markup

Work in progress.

## TODO

- Implement all Fossil Wiki supported HTML elements
- HTML elements without a closing tag, such as `<br>`
- HTML empty attribute syntax
- HTML unquoted attribute value syntax
- Case-insensitive `<verbatim>` tag names
- Handle whitespace at the end of the `</verbatim >` end tag
- Ensure that nesting of content is consistent with the Fossil Wiki parser
