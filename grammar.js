/**
 * @file FossilWiki grammar for tree-sitter
 * @author Simon Bates <simon@bitstructures.com>
 * @license MIT
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
  name: "fossil_wiki",

  rules: {
    // TODO: add the actual grammar rules
    source_file: $ => "hello"
  }
});
