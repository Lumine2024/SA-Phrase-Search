#include "string-data.hpp"
#include <bits/stdc++.h>

using namespace std;
using namespace sa_ps;

int main(int argc, char *argv[]) {
    locale::global(locale(""));
    wcout.imbue(locale());
    wstring str = L"罗密欧与朱丽叶。罗密欧爱朱丽叶。朱丽叶爱罗密欧。";
    string_data data(str);
    auto result = data.search(std::wstring(L"罗密欧") & std::wstring(L"爱"));
    for(int i : result) {
        wcout << i << ' ' << str.substr(i) << endl;
    }
    return 0;
}