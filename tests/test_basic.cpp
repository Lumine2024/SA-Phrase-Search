#include "../include/sa_is.hpp"
#include "../include/sa_match.hpp"
#include "../include/parser.hpp"
#include "../include/andor.hpp"
#include <iostream>
#include <cassert>
#include <cstring>

using namespace sa_phrase_search;

void testBasicSearch() {
    std::cout << "Testing basic search..." << std::endl;
    
    const char* text = "banana";
    SuffixArrayChar sa(text, strlen(text));
    SAMatcherChar matcher(sa);
    
    // Test finding "ana"
    auto results = matcher.findAll("ana");
    assert(results.size() == 2);  // Should find at positions 1 and 3
    assert(results[0] == 1);
    assert(results[1] == 3);
    
    // Test count
    assert(matcher.count("na") == 2);
    assert(matcher.count("ba") == 1);
    assert(matcher.count("xyz") == 0);
    
    // Test exists
    assert(matcher.exists("ana") == true);
    assert(matcher.exists("xyz") == false);
    
    std::cout << "✓ Basic search tests passed" << std::endl;
}

void testWideCharSearch() {
    std::cout << "Testing wide character search..." << std::endl;
    
    const wchar_t* text = L"banana";
    SuffixArrayWChar sa(text, wcslen(text));
    SAMatcherWChar matcher(sa);
    
    auto results = matcher.findAll(L"ana", 3);
    assert(results.size() == 2);
    
    std::cout << "✓ Wide character search tests passed" << std::endl;
}

void testLogicalOperations() {
    std::cout << "Testing logical operations..." << std::endl;
    
    std::vector<int> a = {1, 3, 5, 7, 9};
    std::vector<int> b = {2, 3, 5, 8, 10};
    
    // Test AND
    auto and_result = andOperation(a, b);
    assert(and_result.size() == 2);  // 3 and 5
    assert(and_result[0] == 3);
    assert(and_result[1] == 5);
    
    // Test OR
    auto or_result = orOperation(a, b);
    assert(or_result.size() == 8);  // Union of both: 1,2,3,5,7,8,9,10
    
    // Test NOT
    auto not_result = notOperation(a, b);
    assert(not_result.size() == 3);  // Elements in a but not in b: 1, 7, 9
    assert(not_result[0] == 1);
    assert(not_result[1] == 7);
    assert(not_result[2] == 9);
    
    std::cout << "✓ Logical operations tests passed" << std::endl;
}

void testQueryParser() {
    std::cout << "Testing query parser..." << std::endl;
    
    QueryParserChar parser;
    
    // Test simple query
    auto words1 = parser.extractWords("Romeo");
    assert(words1.size() == 1);
    assert(words1[0] == "Romeo");
    
    // Test AND query
    auto words2 = parser.extractWords("Romeo _AND_ Juliet");
    assert(words2.size() == 2);
    assert(words2[0] == "Romeo");
    assert(words2[1] == "Juliet");
    
    // Test OR query
    auto words3 = parser.extractWords("love _OR_ hate");
    assert(words3.size() == 2);
    assert(words3[0] == "love");
    assert(words3[1] == "hate");
    
    // Test complex query
    auto words4 = parser.extractWords("Romeo _AND_ Juliet _OR_ love");
    assert(words4.size() == 3);
    
    std::cout << "✓ Query parser tests passed" << std::endl;
}

void testChineseText() {
    std::cout << "Testing Chinese text..." << std::endl;
    
    // Chinese text: "罗密欧与朱丽叶。罗密欧爱朱丽叶。"
    const wchar_t* text = L"\u7F57\u5BC6\u6B27\u4E0E\u6731\u4E3D\u53F6\u3002"
                         L"\u7F57\u5BC6\u6B27\u7231\u6731\u4E3D\u53F6\u3002";
    
    SuffixArrayWChar sa(text, wcslen(text));
    SAMatcherWChar matcher(sa);
    
    // Search for "罗密欧" (Romeo)
    const wchar_t* romeo = L"\u7F57\u5BC6\u6B27";
    auto results = matcher.findAll(romeo, wcslen(romeo));
    
    assert(results.size() == 2);  // Should find Romeo twice
    assert(results[0] == 0);
    assert(results[1] == 8);
    
    std::cout << "✓ Chinese text tests passed" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   SA Phrase Search Library Tests      " << std::endl;
    std::cout << "========================================" << std::endl << std::endl;
    
    try {
        testBasicSearch();
        testWideCharSearch();
        testLogicalOperations();
        testQueryParser();
        testChineseText();
        
        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "   All tests passed! ✓                 " << std::endl;
        std::cout << "========================================" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
