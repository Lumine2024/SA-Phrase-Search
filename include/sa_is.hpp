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

    // Helper function to determine if a suffix is S-type or L-type
    void computeTypes(const CharT* s, int n, std::vector<bool>& types) {
        types.resize(n);
        types[n - 1] = false; // Last character is S-type
        
        for (int i = n - 2; i >= 0; --i) {
            if (s[i] < s[i + 1]) {
                types[i] = false; // S-type
            } else if (s[i] > s[i + 1]) {
                types[i] = true; // L-type
            } else {
                types[i] = types[i + 1];
            }
        }
    }

    // Induced sorting from LMS suffixes
    void induceSort(const CharT* s, int n, std::vector<int>& sa, 
                    const std::vector<bool>& types, const std::vector<int>& buckets,
                    const std::vector<int>& bucket_ends) {
        // Initialize SA with -1
        std::fill(sa.begin(), sa.end(), -1);
        
        // Place LMS suffixes at the end of their buckets
        std::vector<int> bucket_tails = bucket_ends;
        for (int i = n - 1; i >= 0; --i) {
            if (isLMS(i, types)) {
                sa[--bucket_tails[s[i]]] = i;
            }
        }
        
        // Induce L-type suffixes
        std::vector<int> bucket_heads = buckets;
        for (int i = 0; i < n; ++i) {
            if (sa[i] > 0 && types[sa[i] - 1]) {
                sa[bucket_heads[s[sa[i] - 1]]++] = sa[i] - 1;
            }
        }
        
        // Induce S-type suffixes
        bucket_tails = bucket_ends;
        for (int i = n - 1; i >= 0; --i) {
            if (sa[i] > 0 && !types[sa[i] - 1]) {
                sa[--bucket_tails[s[sa[i] - 1]]] = sa[i] - 1;
            }
        }
    }

    // Check if position is LMS (left-most S-type)
    bool isLMS(int i, const std::vector<bool>& types) const {
        if (i == 0) return false;
        return !types[i] && types[i - 1];
    }

    // Build suffix array using SA-IS algorithm
    void buildSAIS(const CharT* s, int n, std::vector<int>& sa, int alphabet_size) {
        if (n == 0) {
            return;
        }
        
        if (n == 1) {
            sa[0] = 0;
            return;
        }
        
        // Compute suffix types (S or L)
        std::vector<bool> types;
        computeTypes(s, n, types);
        
        // Compute bucket sizes
        std::vector<int> buckets(alphabet_size, 0);
        for (int i = 0; i < n; ++i) {
            buckets[s[i]]++;
        }
        
        // Compute bucket start positions
        std::vector<int> bucket_starts(alphabet_size, 0);
        std::vector<int> bucket_ends(alphabet_size, 0);
        int sum = 0;
        for (int i = 0; i < alphabet_size; ++i) {
            bucket_starts[i] = sum;
            sum += buckets[i];
            bucket_ends[i] = sum;
        }
        
        // Induce sort
        induceSort(s, n, sa, types, bucket_starts, bucket_ends);
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
        
        // Find alphabet size (max character value + 1)
        int max_char = 0;
        for (size_t i = 0; i < n_; ++i) {
            if (text_[i] > max_char) {
                max_char = text_[i];
            }
        }
        int alphabet_size = max_char + 1;
        
        buildSAIS(text, n_, sa_, alphabet_size);
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
