import XCTest
import SwiftTreeSitter
import TreeSitterFossilWiki

final class TreeSitterFossilWikiTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_fossil_wiki())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Fossil Wiki grammar")
    }
}
