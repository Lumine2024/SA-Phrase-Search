#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace sa_ps {

namespace detail {

std::vector<int> sa_match(const std::wstring &s, const std::vector<int> &sa, const std::wstring &t) {
    if(s.size() < t.size()) return {};
    if(s.size() == t.size()) {
        if(s == t) return {0};
        return {};
    }
    int n = s.size();
    const wchar_t *__restrict ps = s.data();
    const wchar_t *__restrict pt = t.data();
    auto bin = [&](bool first) -> int {
        int l = 0, r = n - 1, ans = -1;
        while(l <= r) {
            int mid = (l + r) / 2;
            int cmp = std::char_traits<wchar_t>::compare(ps + sa[mid], pt, t.size());
            if(cmp == 0) {
                ans = mid;
                if(first) r = mid - 1;
                else l = mid + 1;
            } else if(cmp > 0) {
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        return ans;
    };
    int ansl = bin(true), ansr = bin(false);
    if(ansl == -1) return {};
    std::vector<int> ans(ansr - ansl + 1);
    for(int i = ansl, j = 0; i <= ansr; ++i, ++j) {
        ans[j] = sa[i];
    }
    std::sort(ans.begin(), ans.end());
    return ans;
}

}

}