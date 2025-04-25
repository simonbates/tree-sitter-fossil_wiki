package tree_sitter_fossil_wiki_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_fossil_wiki "github.com/tree-sitter/tree-sitter-fossil_wiki/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_fossil_wiki.Language())
	if language == nil {
		t.Errorf("Error loading Fossil Wiki grammar")
	}
}
