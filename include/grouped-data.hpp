#pragma once

#include "sa-match.hpp"
#include <string>
#include <vector>
#include <numeric>

namespace sa_ps {

namespace detail {

enum group_type {
    AND = 0,
    OR
};

template<group_type Type>
struct grouped_data {
    static constexpr group_type type = Type;
    std::vector<std::wstring> strs;
    grouped_data() {}
    explicit grouped_data(const std::vector<std::wstring> &_strs) : strs(_strs) {}
};

grouped_data<group_type::AND> operator&(const std::wstring &s1, const std::wstring &s2) {
    return grouped_data<group_type::AND>({s1, s2});
}
grouped_data<group_type::AND> &operator&=(grouped_data<group_type::AND> &value, const std::wstring &s2) {
    value.strs.push_back(s2);
    return value;
}
grouped_data<group_type::AND> operator&(const grouped_data<group_type::AND> &value, const std::wstring &s2) {
    grouped_data<group_type::AND> result = value;
    result &= s2;
    return result;
}
grouped_data<group_type::AND> operator&(const std::wstring &s1, const grouped_data<group_type::AND> &value) {
    grouped_data<group_type::AND> result = value;
    result &= s1;
    return result;
}

grouped_data<group_type::OR> operator|(const std::wstring &s1, const std::wstring &s2) {
    return grouped_data<group_type::OR>({s1, s2});
}
grouped_data<group_type::OR> &operator|=(grouped_data<group_type::OR> &value, const std::wstring &s2) {
    value.strs.push_back(s2);
    return value;
}
grouped_data<group_type::OR> operator|(const grouped_data<group_type::OR> &value, const std::wstring &s2) {
    grouped_data<group_type::OR> result = value;
    result |= s2;
    return result;
}
grouped_data<group_type::OR> operator|(const std::wstring &s1, const grouped_data<group_type::OR> &value) {
    grouped_data<group_type::OR> result = value;
    result |= s1;
    return result;
}

std::vector<int> grouped_match(const std::wstring &str, const std::vector<int> &sa, const grouped_data<group_type::AND> &data, int md) {
    int n = data.strs.size();
    if(n == 0) {
        std::vector<int> ans(str.size());
        std::iota(ans.begin(), ans.end(), 0);
        return ans;
    }
    std::vector<std::vector<int>> fevery(n);
    for(int i = 0; i < n; ++i) {
        fevery[i] = sa_match(str, sa, data.strs[i]);
    }
    std::vector<int> ans = fevery[0];
    std::vector<int> buffer;
    for(int i = 1; i < n; ++i) {
        int j = 0, k = 0;
        while(j < ans.size() && k < fevery[i].size()) {
            if(abs(ans[j] - fevery[i][k]) <= md) {
                buffer.push_back(std::min(ans[j], fevery[i][k]));
                ++j;
                ++k;
            } else if(ans[j] < fevery[i][k]) {
                ++j;
            } else {
                ++k;
            }
        }
        std::swap(ans, buffer);
        buffer.clear();
    }
    return ans;
}
std::vector<int> grouped_match(const std::wstring &str, const std::vector<int> &sa, const grouped_data<group_type::OR> &data, int md) {
    int n = data.strs.size();
    if(n == 0) {
        std::vector<int> ans(str.size());
        std::iota(ans.begin(), ans.end(), 0);
        return ans;
    }
    std::vector<std::vector<int>> fevery(n);
    for(int i = 0; i < n; ++i) {
        fevery[i] = sa_match(str, sa, data.strs[i]);
    }
    std::vector<int> ans = fevery[0];
    std::vector<int> buffer;
    for(int i = 1; i < n; ++i) {
        int j = 0, k = 0;
        while(j < ans.size() && k < fevery[i].size()) {
            if(abs(ans[j] - fevery[i][k]) <= md) {
                buffer.push_back(std::min(ans[j], fevery[i][k]));
                ++j;
                ++k;
            } else if(ans[j] < fevery[i][k]) {
                buffer.push_back(ans[j]);
                ++j;
            } else {
                buffer.push_back(fevery[i][k]);
                ++k;
            }
        }
        while(j < ans.size()) {
            buffer.push_back(ans[j]);
            ++j;
        }
        while(k < fevery[i].size()) {
            buffer.push_back(fevery[i][k]);
            ++k;
        }
        std::swap(ans, buffer);
        buffer.clear();
    }
    return ans;
}

} // namespace detail

} // namespace sa_ps

using sa_ps::detail::operator&;
using sa_ps::detail::operator|;