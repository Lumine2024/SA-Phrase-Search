# SA-Phrase-Search
2024届数据结构与算法挑战班课程设计：基于后缀数组的短语检索

A high-performance library for phrase search using suffix arrays. Supports both English and Chinese text through `wchar_t` support.

## Features

- **Suffix Array Construction**: Linear-time SA-IS (Suffix Array - Induced Sorting) algorithm
- **Pattern Matching**: Efficient O(m log n) pattern search using binary search on suffix array
- **Query Parsing**: Parse complex queries with logical operators (AND, OR, NOT)
- **Logical Operations**: Combine search results using AND/OR/NOT operations
- **Unicode Support**: Full `wchar_t` support for handling Chinese, Japanese, and other international text

## Library Components

### 1. `sa_is.hpp` - Suffix Array Construction
Implements the SA-IS algorithm for building suffix arrays in linear time.

```cpp
#include "sa_is.hpp"
using namespace sa_phrase_search;

const char* text = "Romeo and Juliet";
SuffixArrayChar sa(text, strlen(text));
```

### 2. `sa_match.hpp` - Pattern Matching
Efficient pattern matching using the suffix array with binary search.

```cpp
#include "sa_match.hpp"

SAMatcherChar matcher(sa);
auto results = matcher.findAll("Romeo");  // Returns vector of positions
bool exists = matcher.exists("Juliet");   // Check if pattern exists
int count = matcher.count("love");        // Count occurrences
```

### 3. `parser.hpp` - Query Parser
Parse user queries with logical operators like `Romeo _AND_ Juliet`.

```cpp
#include "parser.hpp"

QueryParserChar parser;
auto ast = parser.parse("Romeo _AND_ Juliet");
auto words = parser.extractWords("Romeo _OR_ love");
```

### 4. `andor.hpp` - Logical Operations
Combine search results using logical operations.

```cpp
#include "andor.hpp"

auto results1 = matcher.findAll("Romeo");
auto results2 = matcher.findAll("Juliet");

// Logical AND
auto and_results = andOperation(results1, results2);

// Logical OR
auto or_results = orOperation(results1, results2);

// Logical NOT (difference)
auto not_results = notOperation(results1, results2);
```

## Wide Character Support (Chinese/International Text)

The library fully supports `wchar_t` for handling non-ASCII text:

```cpp
// Chinese text example
const wchar_t* text = L"罗密欧与朱丽叶。罗密欧爱朱丽叶。";
SuffixArrayWChar sa(text, wcslen(text));
SAMatcherWChar matcher(sa);

const wchar_t* pattern = L"罗密欧";  // "Romeo" in Chinese
auto results = matcher.findAll(pattern, wcslen(pattern));
```

## Building the Library

### Prerequisites
- C++14 compatible compiler
- CMake 3.10 or higher

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Run Demo

```bash
./demo
```

The demo program demonstrates:
- English text search
- Wide character support
- Chinese text search
- Query parsing
- Logical operations

## Usage Example

```cpp
#include "sa_is.hpp"
#include "sa_match.hpp"
#include "parser.hpp"
#include "andor.hpp"

using namespace sa_phrase_search;

int main() {
    // Build suffix array
    const char* text = "Romeo and Juliet";
    SuffixArrayChar sa(text, strlen(text));
    
    // Create matcher
    SAMatcherChar matcher(sa);
    
    // Search for patterns
    auto romeo_pos = matcher.findAll("Romeo");
    auto juliet_pos = matcher.findAll("Juliet");
    
    // Combine results
    auto both = andOperation(romeo_pos, juliet_pos);
    auto either = orOperation(romeo_pos, juliet_pos);
    
    // Parse queries
    QueryParserChar parser;
    auto words = parser.extractWords("Romeo _AND_ Juliet");
    
    return 0;
}
```

## API Reference

### SuffixArray<CharT>
- `SuffixArray(const CharT* text, size_t len)` - Construct suffix array
- `const std::vector<int>& getSuffixArray()` - Get suffix array
- `const std::vector<int>& getLCP()` - Get LCP array
- `const CharT* getText()` - Get original text
- `size_t getLength()` - Get text length

### SAMatcher<CharT>
- `std::vector<int> findAll(pattern)` - Find all occurrences
- `bool exists(pattern)` - Check if pattern exists
- `int count(pattern)` - Count occurrences

### QueryParser<CharT>
- `std::shared_ptr<ASTNode<CharT>> parse(query)` - Parse query to AST
- `std::vector<std::basic_string<CharT>> extractWords(query)` - Extract words

### Logical Operations
- `andOperation(a, b)` - AND operation
- `orOperation(a, b)` - OR operation
- `notOperation(a, b)` - NOT operation (difference)
- `proximityAnd(a, b, distance)` - Proximity-based AND
- `orderedProximity(a, b, len, distance)` - Ordered phrase search

## Complexity

- **Suffix Array Construction**: O(n) time, O(n) space
- **Pattern Search**: O(m log n) time, where m is pattern length
- **LCP Construction**: O(n) time using Kasai's algorithm

## License

See LICENSE file for details.

## Contributors

2024届数据结构与算法挑战班
