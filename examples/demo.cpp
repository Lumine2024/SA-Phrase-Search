#include "../include/sa_is.hpp"
#include "../include/sa_match.hpp"
#include "../include/parser.hpp"
#include "../include/andor.hpp"
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

using namespace sa_phrase_search;

// Helper function to print results
void printResults(const std::string& query, const std::vector<int>& results) {
    std::cout << "Query: \"" << query << "\"" << std::endl;
    std::cout << "Found " << results.size() << " occurrence(s) at position(s): ";
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << results[i];
        if (i < results.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl << std::endl;
}

// Demo with English text
void demoEnglish() {
    std::cout << "=== English Text Demo ===" << std::endl << std::endl;
    
    const char* text = "Romeo and Juliet is a tragedy written by William Shakespeare. "
                      "Romeo loves Juliet and Juliet loves Romeo. "
                      "The story of Romeo and Juliet is timeless.";
    
    std::cout << "Text: " << text << std::endl << std::endl;
    
    // Build suffix array
    SuffixArrayChar sa(text, strlen(text));
    SAMatcherChar matcher(sa);
    
    // Simple searches
    std::cout << "--- Simple Pattern Matching ---" << std::endl;
    
    auto romeo_results = matcher.findAll("Romeo");
    printResults("Romeo", romeo_results);
    
    auto juliet_results = matcher.findAll("Juliet");
    printResults("Juliet", juliet_results);
    
    auto love_results = matcher.findAll("love");
    printResults("love", love_results);
    
    // AND operation
    std::cout << "--- Logical AND Operation ---" << std::endl;
    auto and_results = andOperation(romeo_results, juliet_results);
    std::cout << "Documents containing both 'Romeo' AND 'Juliet': " 
              << and_results.size() << " positions" << std::endl;
    std::cout << "(In a real document system, these would be document IDs)" << std::endl << std::endl;
    
    // OR operation
    std::cout << "--- Logical OR Operation ---" << std::endl;
    auto or_results = orOperation(romeo_results, juliet_results);
    std::cout << "Positions with 'Romeo' OR 'Juliet': " 
              << or_results.size() << " total occurrences" << std::endl << std::endl;
    
    // Query parsing demo
    std::cout << "--- Query Parser Demo ---" << std::endl;
    QueryParserChar parser;
    
    std::string query1 = "Romeo _AND_ Juliet";
    auto words1 = parser.extractWords(query1);
    std::cout << "Query: \"" << query1 << "\"" << std::endl;
    std::cout << "Extracted words: ";
    for (const auto& word : words1) {
        std::cout << "\"" << word << "\" ";
    }
    std::cout << std::endl << std::endl;
    
    std::string query2 = "Romeo _OR_ love";
    auto words2 = parser.extractWords(query2);
    std::cout << "Query: \"" << query2 << "\"" << std::endl;
    std::cout << "Extracted words: ";
    for (const auto& word : words2) {
        std::cout << "\"" << word << "\" ";
    }
    std::cout << std::endl << std::endl;
}

// Demo with wide character (for international text support)
void demoWideChar() {
    std::cout << "=== Wide Character Demo (Unicode Support) ===" << std::endl << std::endl;
    
    // Example with ASCII represented as wchar_t (can work with Chinese/Japanese too)
    const wchar_t* text = L"Romeo and Juliet. Romeo loves Juliet. Juliet is beautiful.";
    
    std::wcout << L"Text: " << text << std::endl << std::endl;
    
    // Build suffix array for wide characters
    SuffixArrayWChar sa(text, wcslen(text));
    SAMatcherWChar matcher(sa);
    
    // Search patterns
    std::cout << "--- Wide Character Pattern Matching ---" << std::endl;
    
    auto romeo_results = matcher.findAll(L"Romeo");
    std::wcout << L"Query: \"Romeo\"" << std::endl;
    std::cout << "Found " << romeo_results.size() << " occurrence(s) at position(s): ";
    for (size_t i = 0; i < romeo_results.size(); ++i) {
        std::cout << romeo_results[i];
        if (i < romeo_results.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl << std::endl;
    
    auto juliet_results = matcher.findAll(L"Juliet");
    std::wcout << L"Query: \"Juliet\"" << std::endl;
    std::cout << "Found " << juliet_results.size() << " occurrence(s) at position(s): ";
    for (size_t i = 0; i < juliet_results.size(); ++i) {
        std::cout << juliet_results[i];
        if (i < juliet_results.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl << std::endl;
    
    // Demonstrate query parser with wide characters
    std::cout << "--- Wide Character Query Parser ---" << std::endl;
    QueryParserWChar parser;
    
    std::wstring query = L"Romeo _AND_ Juliet";
    auto words = parser.extractWords(query);
    std::wcout << L"Query: \"" << query << L"\"" << std::endl;
    std::wcout << L"Extracted words: ";
    for (const auto& word : words) {
        std::wcout << L"\"" << word << L"\" ";
    }
    std::cout << std::endl << std::endl;
    
    // AND operation
    auto and_results = andOperation(romeo_results, juliet_results);
    std::cout << "Combined results (Romeo AND Juliet): " 
              << and_results.size() << " documents/segments" << std::endl << std::endl;
}

// Demo with Chinese characters (commented as it needs proper UTF-8 terminal)
void demoChinese() {
    std::cout << "=== Chinese Text Demo ===" << std::endl << std::endl;
    std::cout << "Note: Chinese character demo requires UTF-8 terminal support" << std::endl;
    std::cout << "Example shows how the library handles wchar_t for CJK characters:" << std::endl << std::endl;
    
    // Example Chinese text: "罗密欧与朱丽叶。罗密欧爱朱丽叶。"
    // (Romeo and Juliet. Romeo loves Juliet.)
    const wchar_t* text = L"\u7F57\u5BC6\u6B27\u4E0E\u6731\u4E3D\u53F6\u3002"
                         L"\u7F57\u5BC6\u6B27\u7231\u6731\u4E3D\u53F6\u3002";
    
    SuffixArrayWChar sa(text, wcslen(text));
    SAMatcherWChar matcher(sa);
    
    // Search for "罗密欧" (Romeo)
    const wchar_t* romeo = L"\u7F57\u5BC6\u6B27";
    auto results = matcher.findAll(romeo, wcslen(romeo));
    
    std::cout << "Searching for Chinese characters (Romeo): " 
              << results.size() << " occurrences found" << std::endl;
    std::cout << "Positions: ";
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << results[i];
        if (i < results.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl << std::endl;
}

int main() {
    std::cout << "================================================" << std::endl;
    std::cout << "   Suffix Array Phrase Search Library Demo     " << std::endl;
    std::cout << "================================================" << std::endl << std::endl;
    
    // Run demos
    demoEnglish();
    std::cout << std::endl;
    
    demoWideChar();
    std::cout << std::endl;
    
    demoChinese();
    
    std::cout << "================================================" << std::endl;
    std::cout << "Demo complete! The library supports:" << std::endl;
    std::cout << "  - Suffix array construction (SA-IS algorithm)" << std::endl;
    std::cout << "  - Pattern matching in O(m log n) time" << std::endl;
    std::cout << "  - Query parsing with AND/OR operators" << std::endl;
    std::cout << "  - Both char and wchar_t (English and Chinese)" << std::endl;
    std::cout << "================================================" << std::endl;
    
    return 0;
}
