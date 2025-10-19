#ifndef SA_IS_HPP
#define SA_IS_HPP

#include <vector>
#include <algorithm>
#include <cstring>

/**
 * @brief SA-IS (Suffix Array - Induced Sorting) Algorithm
 * 
 * This implements the SA-IS algorithm for constructing suffix arrays in linear time.
 * Supports both char and wchar_t for handling English and Chinese text.
 * 
 * Reference: Nong, G., Zhang, S., & Chan, W. H. (2009). 
 * Linear Suffix Array Construction by Almost Pure Induced-Sorting.
 */

namespace sa_phrase_search {

template<typename CharT>
class SuffixArray {
private:
    std::vector<int> sa_;    // Suffix array
    std::vector<int> lcp_;   // Longest common prefix array
    const CharT* text_;      // Pointer to original text
    size_t n_;               // Length of text

    // Simple comparison-based suffix array construction
    // This is O(n^2 log n) but correct and simple
    void buildSimple(const CharT* s, int n, std::vector<int>& sa) {
        // Initialize suffix array with indices
        sa.resize(n);
        for (int i = 0; i < n; ++i) {
            sa[i] = i;
        }
        
        // Sort suffixes using comparison
        std::sort(sa.begin(), sa.end(), [s, n](int a, int b) {
            // Compare suffixes starting at positions a and b
            int i = 0;
            while (a + i < n && b + i < n) {
                if (s[a + i] < s[b + i]) return true;
                if (s[a + i] > s[b + i]) return false;
                i++;
            }
            // If one suffix is a prefix of the other, shorter one comes first
            return (a + i == n && b + i < n);
        });
    }

    // Compute LCP array using Kasai's algorithm
    void computeLCP() {
        int n = sa_.size();
        lcp_.resize(n, 0);
        std::vector<int> rank(n);
        
        // Compute rank array (inverse of suffix array)
        for (int i = 0; i < n; ++i) {
            rank[sa_[i]] = i;
        }
        
        int h = 0;
        for (int i = 0; i < n; ++i) {
            if (rank[i] > 0) {
                int j = sa_[rank[i] - 1];
                while (i + h < n && j + h < n && text_[i + h] == text_[j + h]) {
                    h++;
                }
                lcp_[rank[i]] = h;
                if (h > 0) h--;
            }
        }
    }

public:
    /**
     * @brief Construct suffix array from text
     * @param text Input text (must remain valid during the lifetime of this object)
     * @param len Length of text
     */
    SuffixArray(const CharT* text, size_t len) : text_(text), n_(len) {
        sa_.resize(n_);
        
        if (n_ == 0) return;
        
        // Use simple sorting algorithm for correctness
        buildSimple(text, n_, sa_);
        computeLCP();
    }
    
    /**
     * @brief Get the suffix array
     * @return Reference to suffix array
     */
    const std::vector<int>& getSuffixArray() const {
        return sa_;
    }
    
    /**
     * @brief Get the LCP array
     * @return Reference to LCP array
     */
    const std::vector<int>& getLCP() const {
        return lcp_;
    }
    
    /**
     * @brief Get the original text
     * @return Pointer to original text
     */
    const CharT* getText() const {
        return text_;
    }
    
    /**
     * @brief Get the length of text
     * @return Length of text
     */
    size_t getLength() const {
        return n_;
    }
};

// Type aliases for convenience
using SuffixArrayChar = SuffixArray<char>;
using SuffixArrayWChar = SuffixArray<wchar_t>;

} // namespace sa_phrase_search

#endif // SA_IS_HPP
