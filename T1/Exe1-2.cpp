#include <string>
#include <string_view>
#include <cassert>
#include <map>

using namespace std;

class Translator {
public:

    Translator() = default;

    void Add(string_view source, string_view target) {

        string source_str = static_cast<string> (source);
        string target_str = static_cast<string> (target);
        words_[source_str] = target_str;
        auto it_string = words_.find(source_str);
        forward_words_[it_string->first] = it_string->second;
        backward_words_[it_string->second] = it_string->first;

    }

    string_view TranslateForward(string_view source) const{
        
        if (auto result = forward_words_.find(source); result != forward_words_.end()) {
            return result->second;
        }
        return {};

    }

    string_view TranslateBackward(string_view target) const{
        
        if (auto result = backward_words_.find(target); result != backward_words_.end()) {
            return result->second;
        }
        return {};
    }

private:
    map<string_view, string_view> forward_words_ = {};
    map<string_view, string_view> backward_words_ = {};
    map<string, string> words_ = {};
   
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}

void Test1(){
    Translator t;
    t.Add(string("list"s), string("list"s));
    t.Add(string("okno"s), string("window"s));
    t.Add(string("stol"s), string("table"s));
    t.Add(string("stol"s), string("tab"s));

}

int main() {
    TestSimple();
    Test1();
}