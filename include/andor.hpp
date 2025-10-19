#ifndef ANDOR_HPP
#define ANDOR_HPP

#include <vector>
#include <algorithm>
#include <set>

/**
 * @brief Logical AND/OR operations on search results
 * 
 * This module provides functions to combine search results using
 * logical AND and OR operations. Results are represented as vectors
 * of positions where patterns occur.
 */

namespace sa_phrase_search {

/**
 * @brief Perform AND operation on two result sets
 * 
 * For phrase search, AND operation finds positions where both patterns
 * occur within the same document/segment. This can be extended to check
 * proximity or just co-occurrence.
 * 
 * @param a First set of positions
 * @param b Second set of positions
 * @return Intersection of both sets
 */
inline std::vector<int> andOperation(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    
    // Use set intersection for sorted vectors
    std::set_intersection(a.begin(), a.end(), 
                         b.begin(), b.end(),
                         std::back_inserter(result));
    
    return result;
}

/**
 * @brief Perform OR operation on two result sets
 * 
 * OR operation combines positions from both patterns.
 * 
 * @param a First set of positions
 * @param b Second set of positions
 * @return Union of both sets
 */
inline std::vector<int> orOperation(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    
    // Use set union for sorted vectors
    std::set_union(a.begin(), a.end(),
                   b.begin(), b.end(),
                   std::back_inserter(result));
    
    return result;
}

/**
 * @brief Perform NOT operation (difference)
 * 
 * NOT operation finds positions in first set that are not in second set.
 * 
 * @param a First set of positions
 * @param b Second set of positions (to exclude)
 * @return Difference (a - b)
 */
inline std::vector<int> notOperation(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    
    // Use set difference for sorted vectors
    std::set_difference(a.begin(), a.end(),
                       b.begin(), b.end(),
                       std::back_inserter(result));
    
    return result;
}

/**
 * @brief Check if two patterns occur within a certain distance
 * 
 * This is useful for phrase search where we want to find patterns
 * that occur close to each other.
 * 
 * @param a First set of positions
 * @param b Second set of positions
 * @param max_distance Maximum allowed distance between positions
 * @return Positions from first set that have a match in second set within distance
 */
inline std::vector<int> proximityAnd(const std::vector<int>& a, const std::vector<int>& b, 
                                     int max_distance) {
    std::vector<int> result;
    
    size_t j = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        // Move j forward to valid range
        while (j < b.size() && b[j] < a[i] - max_distance) {
            j++;
        }
        
        // Check if any position in b is within distance
        size_t k = j;
        while (k < b.size() && b[k] <= a[i] + max_distance) {
            if (std::abs(b[k] - a[i]) <= max_distance) {
                result.push_back(a[i]);
                break;
            }
            k++;
        }
    }
    
    return result;
}

/**
 * @brief Find positions where pattern a is followed by pattern b within max_distance
 * 
 * This implements ordered phrase search where we need a specific order.
 * 
 * @param a First pattern positions
 * @param b Second pattern positions
 * @param pattern_a_len Length of first pattern
 * @param max_distance Maximum distance between end of a and start of b
 * @return Positions of pattern a that are followed by pattern b
 */
inline std::vector<int> orderedProximity(const std::vector<int>& a, const std::vector<int>& b,
                                        int pattern_a_len, int max_distance = 0) {
    std::vector<int> result;
    
    size_t j = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        int end_of_a = a[i] + pattern_a_len;
        
        // Move j forward
        while (j < b.size() && b[j] < end_of_a) {
            j++;
        }
        
        // Check if any position in b is within distance after a
        size_t k = j;
        while (k < b.size() && b[k] <= end_of_a + max_distance) {
            result.push_back(a[i]);
            break;
        }
    }
    
    return result;
}

/**
 * @brief Combine multiple result sets with AND operation
 * @param results Vector of result sets
 * @return Intersection of all sets
 */
inline std::vector<int> multiAnd(const std::vector<std::vector<int>>& results) {
    if (results.empty()) {
        return {};
    }
    
    std::vector<int> result = results[0];
    for (size_t i = 1; i < results.size(); ++i) {
        result = andOperation(result, results[i]);
    }
    
    return result;
}

/**
 * @brief Combine multiple result sets with OR operation
 * @param results Vector of result sets
 * @return Union of all sets
 */
inline std::vector<int> multiOr(const std::vector<std::vector<int>>& results) {
    if (results.empty()) {
        return {};
    }
    
    std::vector<int> result = results[0];
    for (size_t i = 1; i < results.size(); ++i) {
        result = orOperation(result, results[i]);
    }
    
    return result;
}

} // namespace sa_phrase_search

#endif // ANDOR_HPP
