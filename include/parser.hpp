#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include <cctype>
#include <algorithm>

/**
 * @brief Query parser for phrase search
 * 
 * This module parses user queries with logical operators like:
 * - "Romeo _AND_ Juliet"
 * - "Romeo _OR_ Juliet"
 * - "Romeo _AND_ (Juliet _OR_ love)"
 * 
 * Supports both char and wchar_t for English and Chinese text.
 */

namespace sa_phrase_search {

// Token types for parsing
enum class TokenType {
    WORD,
    AND,
    OR,
    NOT,
    LPAREN,
    RPAREN,
    END
};

// Token structure
template<typename CharT>
struct Token {
    TokenType type;
    std::basic_string<CharT> value;
    
    Token(TokenType t) : type(t) {}
    Token(TokenType t, const std::basic_string<CharT>& v) : type(t), value(v) {}
};

// Abstract syntax tree node
enum class NodeType {
    WORD,
    AND,
    OR,
    NOT
};

template<typename CharT>
struct ASTNode {
    NodeType type;
    std::basic_string<CharT> value;  // For WORD nodes
    std::shared_ptr<ASTNode<CharT>> left;
    std::shared_ptr<ASTNode<CharT>> right;
    
    explicit ASTNode(NodeType t) : type(t) {}
    ASTNode(NodeType t, const std::basic_string<CharT>& v) : type(t), value(v) {}
};

template<typename CharT>
class QueryParser {
private:
    std::vector<Token<CharT>> tokens_;
    size_t current_;
    
    // Helper to check if string starts with prefix (case insensitive)
    bool startsWithIgnoreCase(const std::basic_string<CharT>& str, 
                             const std::basic_string<CharT>& prefix) const {
        if (str.length() < prefix.length()) return false;
        
        for (size_t i = 0; i < prefix.length(); ++i) {
            CharT c1 = str[i];
            CharT c2 = prefix[i];
            // Simple ASCII-based case conversion for English
            if (c1 >= 'a' && c1 <= 'z') c1 = c1 - 'a' + 'A';
            if (c2 >= 'a' && c2 <= 'z') c2 = c2 - 'a' + 'A';
            if (c1 != c2) return false;
        }
        return true;
    }
    
    // Tokenize input string
    void tokenize(const std::basic_string<CharT>& query) {
        tokens_.clear();
        size_t i = 0;
        
        while (i < query.length()) {
            // Skip whitespace
            while (i < query.length() && std::isspace(static_cast<unsigned char>(query[i]))) {
                i++;
            }
            
            if (i >= query.length()) break;
            
            // Check for operators with underscores
            if (query[i] == '_') {
                size_t start = i;
                i++; // Skip first underscore
                
                // Read until next underscore or end
                std::basic_string<CharT> op;
                while (i < query.length() && query[i] != '_') {
                    op += query[i];
                    i++;
                }
                
                if (i < query.length() && query[i] == '_') {
                    i++; // Skip closing underscore
                }
                
                // Convert to uppercase for comparison
                std::basic_string<CharT> op_upper = op;
                for (auto& c : op_upper) {
                    if (c >= 'a' && c <= 'z') {
                        c = c - 'a' + 'A';
                    }
                }
                
                if (op_upper == std::basic_string<CharT>({'A', 'N', 'D'})) {
                    tokens_.emplace_back(TokenType::AND);
                } else if (op_upper == std::basic_string<CharT>({'O', 'R'})) {
                    tokens_.emplace_back(TokenType::OR);
                } else if (op_upper == std::basic_string<CharT>({'N', 'O', 'T'})) {
                    tokens_.emplace_back(TokenType::NOT);
                } else {
                    // Treat as word if not a recognized operator
                    tokens_.emplace_back(TokenType::WORD, std::basic_string<CharT>(
                        query.begin() + start, query.begin() + i));
                }
            }
            // Check for parentheses
            else if (query[i] == '(') {
                tokens_.emplace_back(TokenType::LPAREN);
                i++;
            }
            else if (query[i] == ')') {
                tokens_.emplace_back(TokenType::RPAREN);
                i++;
            }
            // Read word
            else {
                size_t start = i;
                while (i < query.length() && 
                       !std::isspace(static_cast<unsigned char>(query[i])) &&
                       query[i] != '(' && query[i] != ')' && query[i] != '_') {
                    i++;
                }
                
                std::basic_string<CharT> word(query.begin() + start, query.begin() + i);
                tokens_.emplace_back(TokenType::WORD, word);
            }
        }
        
        tokens_.emplace_back(TokenType::END);
    }
    
    // Recursive descent parser
    std::shared_ptr<ASTNode<CharT>> parseExpression() {
        return parseOr();
    }
    
    std::shared_ptr<ASTNode<CharT>> parseOr() {
        auto left = parseAnd();
        
        while (current_ < tokens_.size() && tokens_[current_].type == TokenType::OR) {
            current_++;
            auto right = parseAnd();
            auto node = std::make_shared<ASTNode<CharT>>(NodeType::OR);
            node->left = left;
            node->right = right;
            left = node;
        }
        
        return left;
    }
    
    std::shared_ptr<ASTNode<CharT>> parseAnd() {
        auto left = parseNot();
        
        while (current_ < tokens_.size() && tokens_[current_].type == TokenType::AND) {
            current_++;
            auto right = parseNot();
            auto node = std::make_shared<ASTNode<CharT>>(NodeType::AND);
            node->left = left;
            node->right = right;
            left = node;
        }
        
        return left;
    }
    
    std::shared_ptr<ASTNode<CharT>> parseNot() {
        if (current_ < tokens_.size() && tokens_[current_].type == TokenType::NOT) {
            current_++;
            auto operand = parsePrimary();
            auto node = std::make_shared<ASTNode<CharT>>(NodeType::NOT);
            node->left = operand;
            return node;
        }
        
        return parsePrimary();
    }
    
    std::shared_ptr<ASTNode<CharT>> parsePrimary() {
        if (current_ >= tokens_.size()) {
            return nullptr;
        }
        
        if (tokens_[current_].type == TokenType::LPAREN) {
            current_++;
            auto expr = parseExpression();
            if (current_ < tokens_.size() && tokens_[current_].type == TokenType::RPAREN) {
                current_++;
            }
            return expr;
        }
        
        if (tokens_[current_].type == TokenType::WORD) {
            auto node = std::make_shared<ASTNode<CharT>>(NodeType::WORD, tokens_[current_].value);
            current_++;
            return node;
        }
        
        return nullptr;
    }

public:
    QueryParser() : current_(0) {}
    
    /**
     * @brief Parse a query string
     * @param query Query string with operators like "Romeo _AND_ Juliet"
     * @return Abstract syntax tree representing the query
     */
    std::shared_ptr<ASTNode<CharT>> parse(const std::basic_string<CharT>& query) {
        tokenize(query);
        current_ = 0;
        return parseExpression();
    }
    
    /**
     * @brief Get extracted words from query (for simple cases)
     * @param query Query string
     * @return Vector of words extracted from query
     */
    std::vector<std::basic_string<CharT>> extractWords(const std::basic_string<CharT>& query) {
        tokenize(query);
        std::vector<std::basic_string<CharT>> words;
        
        for (const auto& token : tokens_) {
            if (token.type == TokenType::WORD) {
                words.push_back(token.value);
            }
        }
        
        return words;
    }
};

// Type aliases for convenience
using QueryParserChar = QueryParser<char>;
using QueryParserWChar = QueryParser<wchar_t>;
using ASTNodeChar = ASTNode<char>;
using ASTNodeWChar = ASTNode<wchar_t>;

} // namespace sa_phrase_search

#endif // PARSER_HPP
