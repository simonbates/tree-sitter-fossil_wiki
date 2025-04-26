/**
 * @file FossilWiki grammar for tree-sitter
 * @author Simon Bates <simon@bitstructures.com>
 * @license MIT
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

// Fossil source code for formatting wiki text:
// https://fossil-scm.org/home/file?name=src/wikiformat.c&ci=trunk

const supportedHTMLElements = [
  "a",
  "code",
  "h1",
  "h2",
  "h3",
  "h4",
  "h5",
  "h6",
  "p",
  "title"
];

class HTMLElementRulesGenerator {
  constructor() {
    this.rules = {};
    this.elementRuleNames = [];
  }

  addRulesForElement(elementName) {
    const elementRuleName = `_html_element_${elementName}`;
    const startTagRuleName = `html_start_tag_${elementName}`;
    const endTagRuleName = `html_end_tag_${elementName}`;

    this.elementRuleNames.push(elementRuleName);

    // Element
    this.rules[elementRuleName] = function($) {
      return seq(
        alias($[startTagRuleName], $.html_start_tag),
        repeat($._wiki_markup),
        alias($[endTagRuleName], $.html_end_tag)
      );
    };

    // Start tag
    this.rules[startTagRuleName] = function($) {
      return seq(
        "<",
        alias(new RegExp(elementName, "i"), $.html_tag_name),
        repeat($.html_attribute),
        ">"
      );
    };

    // End tag
    this.rules[endTagRuleName] = function($) {
      return seq(
        "</",
        alias(new RegExp(elementName, "i"), $.html_tag_name),
        ">"
      );
    };
  }
}

const htmlElementRulesGenerator = new HTMLElementRulesGenerator();

for (const elementName of supportedHTMLElements) {
  htmlElementRulesGenerator.addRulesForElement(elementName);
}

module.exports = grammar({
  name: "fossil_wiki",

  externals: $ => [$.verbatim_content],

  rules: {
    document: $ => repeat($._wiki_markup),

    _wiki_markup: $ => choice($.text, $.html_element, $.verbatim, $.link),

    text: $ => /[^<\[]+/,

    // Define html_element as a choice between the rules generated for
    // HTML elements
    html_element: $ => choice.apply(null,
      htmlElementRulesGenerator.elementRuleNames.map(ruleName => $[ruleName])),

    // Merge in the generated HTML element rules
    ...(htmlElementRulesGenerator.rules),

    html_attribute: $ => seq(
      $.html_attribute_name,
      "=",
      choice(
        seq(
          "'",
          alias($.html_attribute_value_single_quoted, $.html_attribute_value),
          "'"
        ),
        seq(
          '"',
          alias($.html_attribute_value_double_quoted, $.html_attribute_value),
          '"'
        )
      )
    ),

    html_attribute_name: $ => /[A-Za-z]+/,

    html_attribute_value_single_quoted: $ => /[^']*/,

    html_attribute_value_double_quoted: $ => /[^"]*/,

    verbatim: $ => seq(
      $.verbatim_start_tag,
      optional($.verbatim_content),
      $.verbatim_end_tag
    ),

    verbatim_start_tag: $ => seq(
      "<",
      alias(/verbatim/i, $.html_tag_name),
      ">"
    ),

    verbatim_end_tag: $ => seq(
      "</",
      alias(/verbatim/i, $.html_tag_name),
      ">"
    ),

    link: $ => seq(
      "[",
      $.link_target,
      optional(
        seq(
          "|",
          $.link_text
        )
      ),
      "]"
    ),

    link_target: $ => /[^\]\|]+/,

    link_text: $ => /[^\]]+/
  }
});
