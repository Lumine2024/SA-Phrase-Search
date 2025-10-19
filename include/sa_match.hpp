#ifndef SA_MATCH_HPP
#define SA_MATCH_HPP

#include "sa_is.hpp"
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>

/**
 * @brief Pattern matching using suffix array
 * 
 * This module provides efficient pattern matching using the suffix array.
 * Uses binary search to find pattern occurrences in O(m log n) time,
 * where m is pattern length and n is text length.
 * Supports both char and wchar_t for English and Chinese text.
 */

namespace sa_phrase_search {

template<typename CharT>
class SAMatcher {
private:
    const SuffixArray<CharT>& sa_;
    
    // Compare pattern with suffix at given position
    int comparePattern(const CharT* pattern, size_t pattern_len, int suffix_idx) const {
        const CharT* text = sa_.getText();
        size_t text_len = sa_.getLength();
        size_t pos = suffix_idx;
        
        for (size_t i = 0; i < pattern_len; ++i) {
            if (pos + i >= text_len) {
                return 1; // Pattern is longer, so pattern > suffix
            }
            if (pattern[i] < text[pos + i]) {
                return -1;
            } else if (pattern[i] > text[pos + i]) {
                return 1;
            }
        }
        return 0; // Pattern matches
    }
    
    // Find leftmost occurrence using binary search
    int findLeft(const CharT* pattern, size_t pattern_len) const {
        const auto& sa = sa_.getSuffixArray();
        int left = 0;
        int right = sa.size();
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (comparePattern(pattern, pattern_len, sa[mid]) > 0) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return left;
    }
    
    // Find rightmost occurrence using binary search
    int findRight(const CharT* pattern, size_t pattern_len) const {
        const auto& sa = sa_.getSuffixArray();
        int left = 0;
        int right = sa.size();
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (comparePattern(pattern, pattern_len, sa[mid]) >= 0) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return left;
    }

public:
    /**
     * @brief Construct matcher from suffix array
     * @param sa Suffix array object
     */
    explicit SAMatcher(const SuffixArray<CharT>& sa) : sa_(sa) {}
    
    /**
     * @brief Find all occurrences of pattern in text
     * @param pattern Pattern to search for
     * @param pattern_len Length of pattern
     * @return Vector of positions where pattern occurs
     */
    std::vector<int> findAll(const CharT* pattern, size_t pattern_len) const {
        if (pattern_len == 0 || sa_.getLength() == 0) {
            return {};
        }
        
        int left = findLeft(pattern, pattern_len);
        int right = findRight(pattern, pattern_len);
        
        std::vector<int> result;
        const auto& sa = sa_.getSuffixArray();
        
        for (int i = left; i < right; ++i) {
            result.push_back(sa[i]);
        }
        
        // Sort positions in ascending order
        std::sort(result.begin(), result.end());
        
        return result;
    }
    
    /**
     * @brief Check if pattern exists in text
     * @param pattern Pattern to search for
     * @param pattern_len Length of pattern
     * @return True if pattern exists, false otherwise
     */
    bool exists(const CharT* pattern, size_t pattern_len) const {
        if (pattern_len == 0 || sa_.getLength() == 0) {
            return false;
        }
        
        int left = findLeft(pattern, pattern_len);
        int right = findRight(pattern, pattern_len);
        
        return left < right;
    }
    
    /**
     * @brief Count occurrences of pattern in text
     * @param pattern Pattern to search for
     * @param pattern_len Length of pattern
     * @return Number of occurrences
     */
    int count(const CharT* pattern, size_t pattern_len) const {
        if (pattern_len == 0 || sa_.getLength() == 0) {
            return 0;
        }
        
        int left = findLeft(pattern, pattern_len);
        int right = findRight(pattern, pattern_len);
        
        return right - left;
    }
    
    /**
     * @brief Find all occurrences with std::basic_string interface
     * @param pattern Pattern to search for
     * @return Vector of positions where pattern occurs
     */
    std::vector<int> findAll(const std::basic_string<CharT>& pattern) const {
        return findAll(pattern.c_str(), pattern.length());
    }
    
    /**
     * @brief Check if pattern exists with std::basic_string interface
     * @param pattern Pattern to search for
     * @return True if pattern exists, false otherwise
     */
    bool exists(const std::basic_string<CharT>& pattern) const {
        return exists(pattern.c_str(), pattern.length());
    }
    
    /**
     * @brief Count occurrences with std::basic_string interface
     * @param pattern Pattern to search for
     * @return Number of occurrences
     */
    int count(const std::basic_string<CharT>& pattern) const {
        return count(pattern.c_str(), pattern.length());
    }
};

// Type aliases for convenience
using SAMatcherChar = SAMatcher<char>;
using SAMatcherWChar = SAMatcher<wchar_t>;

} // namespace sa_phrase_search

#endif // SA_MATCH_HPP
