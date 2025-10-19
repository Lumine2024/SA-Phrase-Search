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
    auto t0 = chrono::high_resolution_clock::now();
    string_data data(content);
    auto t1 = chrono::high_resolution_clock::now();
    wcout << L"Suffix array built in " << chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() << L" ms." << endl;
    auto inds = data.search(wstring(L"林黛玉"));
    auto t2 = chrono::high_resolution_clock::now();
    wcout << L"Search completed in " << chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << L" ms." << endl;
    // for(auto ind : inds) {
    //     wcout << ind << L": "
    //     << content.substr(ind - 8, 20) << L"\n" << endl;
    // }
    return 0;
}