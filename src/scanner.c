#include "tree_sitter/parser.h"

enum TokenType { VERBATIM };

char verbatim_start_tag[] = "<verbatim>";
char verbatim_end_tag[] = "</verbatim>";

enum VerbatimState {
    VERBATIM_STATE_START_TAG,
    VERBATIM_STATE_CONTENT,
    VERBATIM_STATE_END_TAG
};

void *tree_sitter_fossil_wiki_external_scanner_create() { return NULL; }

void tree_sitter_fossil_wiki_external_scanner_destroy(void *payload) {}

unsigned tree_sitter_fossil_wiki_external_scanner_serialize(void *payload, char *buffer) { return 0; }

void tree_sitter_fossil_wiki_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {}

static bool scan_for_verbatim(TSLexer *lexer) {
    enum VerbatimState state = VERBATIM_STATE_START_TAG;
    int position = 0;

    while (true) {
        if (lexer->eof(lexer)) {
            return false;
        }
        switch (state) {
            case VERBATIM_STATE_START_TAG:
                if (lexer->lookahead == verbatim_start_tag[position]) {
                    position += 1;
                    if (verbatim_start_tag[position] == '\0') {
                        // Start tag completed, change to content state
                        state = VERBATIM_STATE_CONTENT;
                    }
                } else {
                    // Mismatch inside the start tag, not <verbatim>
                    return false;
                }
                break;
            case VERBATIM_STATE_CONTENT:
                if (lexer->lookahead == verbatim_end_tag[0]) {
                    // Found the beginning of the end tag, change state
                    state = VERBATIM_STATE_END_TAG;
                    position = 1;
                }
                break;
            case VERBATIM_STATE_END_TAG:
                if (lexer->lookahead == verbatim_end_tag[position]) {
                    position += 1;
                    if (verbatim_end_tag[position] == '\0') {
                        // End tag completed, consume the final character
                        // and return true to say that we have found the token
                        lexer->advance(lexer, false);
                        return true;
                    }
                } else {
                    // Mismatch inside the end tag, go back to content state
                    state = VERBATIM_STATE_CONTENT;
                }
                break;
        }
        lexer->advance(lexer, false);
    }
}

bool tree_sitter_fossil_wiki_external_scanner_scan(void *payload,
    TSLexer *lexer, const bool *valid_symbols)
{
    if (valid_symbols[VERBATIM] && scan_for_verbatim(lexer)) {
        lexer->result_symbol = VERBATIM;
        lexer->mark_end(lexer);
        return true;
    }

    return false;
}
