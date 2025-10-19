#include "string-data.hpp"
#include <bits/stdc++.h>

using namespace std;
using namespace sa_ps;

int main(int argc, char *argv[]) {
    locale::global(locale(""));
    wcout.imbue(locale());
    wifstream file("../examples/hlm.txt");
    if(!file) {
        wcerr << L"Cannot open file." << endl;
        return 1;
    }
    file.imbue(locale());
    wstring content((istreambuf_iterator<wchar_t>(file)), istreambuf_iterator<wchar_t>());
    string_data sd(content);
    vector<int> result = sd.search(wstring(L"宝玉") & wstring(L"黛玉"), 7);
    for(int pos : result) {
        wcout << pos << ' ' << content.substr(pos, 30) << endl;
    }
    return 0;
}