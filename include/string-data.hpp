#pragma once

#include "sa-is.hpp"
#include "sa-match.hpp"
#include "grouped-data.hpp"
#include <vector>
#include <string>

namespace sa_ps {

class string_data {
public:
    explicit string_data(const std::wstring &str) : m_str(str), sa(detail::suffix_array(str)) {}
    std::vector<int> search(const std::wstring &pattern) const {
        return detail::sa_match(m_str, sa, pattern);
    }
    template<detail::group_type Type>
    std::vector<int> search(const detail::grouped_data<Type> &data, int max_distance = 5) const {
        return detail::grouped_match(m_str, sa, data, max_distance);
    }
private:
    std::wstring m_str;
    std::vector<int> sa;
};

} // namespace sa_ps