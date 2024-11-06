#include <cassert>
#include <iostream>
#include <string_view>
#include <vector>

using namespace std;

vector<string_view> SplitIntoWordsView(string_view str) {
    vector<string_view> result;

    auto begin_str = str.find_first_not_of(" ");

    if(begin_str == str.npos) {
        return {};
    }
    str.remove_prefix(begin_str);
    
    while (!str.empty()) {
        size_t temp = str.find(' ');
        result.push_back(temp == str.npos ? str.substr(0,str.npos) : str.substr(0, temp));

        if (0 == temp + 1) {
            return result;
        }
        str.remove_prefix(temp);
        auto next_str = str.find_first_not_of(" ");

        if (next_str == str.npos || next_str == 0){
            break;
        }
        str.remove_prefix(next_str);
    }

    return result;
}

int main() {
    assert((SplitIntoWordsView("") == vector<string_view>{}));
    assert((SplitIntoWordsView("     ") == vector<string_view>{}));
    assert((SplitIntoWordsView("aaaaaaa") == vector{"aaaaaaa"sv}));
    assert((SplitIntoWordsView("a") == vector{"a"sv}));
    assert((SplitIntoWordsView("a b c") == vector{"a"sv, "b"sv, "c"sv}));
    assert((SplitIntoWordsView("a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    cout << "All OK" << endl;
}