#include "tree_sitter/parser.h"

enum TokenType { VERBATIM_CONTENT };

char verbatim_end_tag[] = "</verbatim>";

void *tree_sitter_fossil_wiki_external_scanner_create() { return NULL; }

void tree_sitter_fossil_wiki_external_scanner_destroy(void *payload) {}

unsigned tree_sitter_fossil_wiki_external_scanner_serialize(void *payload, char *buffer) { return 0; }

void tree_sitter_fossil_wiki_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {}

static bool scan_for_verbatim_content(TSLexer *lexer) {
    int content_len = 0;
    int advance_count = 0;
    int position_in_end_tag = 0;

    while (true) {
        if (lexer->eof(lexer)) {
            return false;
        }
        if (lexer->lookahead == verbatim_end_tag[position_in_end_tag]) {
            if (position_in_end_tag == 0) {
                // Match at the start of the end tag,
                // mark the end of the content
                lexer->mark_end(lexer);
                content_len = advance_count;
            }
            position_in_end_tag += 1;
            if (verbatim_end_tag[position_in_end_tag] == '\0') {
                // Reached the end of the end tag
                if (content_len > 0) {
                    // Content was non-zero length
                    return true;
                } else {
                    // Content was zero length
                    return false;
                }
            }
        } else {
            // Did not match within the end tag, reset
            position_in_end_tag = 0;
        }
        lexer->advance(lexer, false);
        advance_count += 1;
    }
}

bool tree_sitter_fossil_wiki_external_scanner_scan(void *payload,
    TSLexer *lexer, const bool *valid_symbols)
{
    if (valid_symbols[VERBATIM_CONTENT] && scan_for_verbatim_content(lexer)) {
        lexer->result_symbol = VERBATIM_CONTENT;
        return true;
    }

    return false;
}
