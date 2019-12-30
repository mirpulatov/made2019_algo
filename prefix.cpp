/*
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n.
Время O(n + p), доп. память – O(p).
p <= 30000, n <= 300000.
- С помощью префикс-функции;
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void findPrefixes(string pattern, vector<int> &prefixes) {
    int j = 0;

    for (int i = 1; i < pattern.length(); i++) {
        while (j > 0 && pattern[j] != pattern[i])
            j = prefixes[j - 1];

        if (pattern[j] == pattern[i])
            j++;

        prefixes[i] = j;
    }
}

void prefixFunction(string text, string pattern) {

    vector <int> prefixes(pattern.length(), 0);

    findPrefixes(pattern, prefixes);

    int j = 0;

    for (int i = 0; i < text.length(); i++) {
        while (j > 0 && pattern[j] != text[i])

            j = prefixes[j - 1];

        if (pattern[j] == text[i])
            j++;

        if (j == pattern.length())
            cout << i - j + 1 << ' ';
    }
}

int main() {
    string txt, pttrn;
    cin >> pttrn;
    vector<int> result;
    cin >> txt;
    prefixFunction(txt, pttrn);
    return 0;
}

