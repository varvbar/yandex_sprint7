#include <string>
#include <iostream>
#include <list>
#include <algorithm>
#include "tests_yandex.h"

class Editor {
public:
    Editor(){
        text_.push_back(cursor_);
        cursor_pos_ = text_.begin();
    };

    // сдвинуть курсор влево
    void Left(){
        
        auto temp_it = cursor_pos_;
       
        if (cursor_pos_ == text_.begin()){
            return;
        }
        else {
            text_.insert(--temp_it, '|');
            text_.erase(cursor_pos_);
            cursor_pos_ = --temp_it;
        }
        return;
    };

    // сдвинуть курсор вправо 
    void Right(){
        
        auto temp_it = cursor_pos_;
        auto end_it = cursor_pos_;
        ++end_it;

        if (end_it == text_.end()){
            return;
        }
        else {
            std::advance(temp_it, 2);
            text_.insert(temp_it, '|');
            text_.erase(cursor_pos_);
            cursor_pos_ = --temp_it;
        }
        return;
    }

    // вставить символ token
    void Insert(char token){

        cursor_pos_ = text_.insert(cursor_pos_, token);
        ++cursor_pos_;
        return;

    }

    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(std::size_t tokens = 1){

        Copy(tokens);

        size_t actual_buffer_size = buffer_.size();

        if (actual_buffer_size == 0) {
            return;
        }

        auto temp_it = cursor_pos_;
        std::advance(temp_it, actual_buffer_size + 1);
        auto end_it = temp_it;
        text_.erase(cursor_pos_,end_it);
        cursor_pos_ = text_.insert(end_it, cursor_);

    }

    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(std::size_t tokens = 1){
        
        buffer_.clear();
        
        if (tokens == 0) {
            return;
        }

        auto distance_it = std::distance(cursor_pos_, text_.end());
        size_t capacity_size = static_cast<size_t> (distance_it - 1);

        if (capacity_size == 0) {
            return;
        }

        size_t actual_buffer_size = tokens;

        if (capacity_size < tokens) {
            actual_buffer_size = capacity_size;
        } 

        auto temp_it = cursor_pos_;
        std::advance(temp_it, actual_buffer_size + 1);
        auto end_it = temp_it;
        temp_it = cursor_pos_;
        buffer_.insert(buffer_.begin(), ++temp_it, end_it);
        
        return;
    }

    // вставить содержимое буфера в текущую позицию курсора
    void Paste(){

        if (!buffer_.empty()) {
          
            text_.insert(cursor_pos_, buffer_.begin(), buffer_.end());      

        }
        else {
            return;
        }
    }
    
    // получить текущее содержимое текстового редактора
    std::string GetText() const{
        
        std::string temp;
        if (text_.size() < 1) {
            return temp;
        }

        temp.reserve(text_.size() + 1);
        
        for(char ch : text_){

            if(ch != '|'){
                temp.push_back(ch);
            }
        
        }

        return temp;
    }

    std::list<char>::iterator GetCursor (){
        return cursor_pos_;
    }

    std::string GetBuffer() const{
        
        std::string temp;
        if (buffer_.size() < 1) {
            return temp;
        }

        temp.reserve(buffer_.size() + 1);
        
        for(char ch : buffer_){

            temp.push_back(ch);

        }

        return temp;
    }

private:
    std::list<char> text_;
    std::list<char> buffer_;
    const char cursor_ = '|';
    std::list<char>::iterator cursor_pos_;

};

// autotests

// test constructor
void TestConstructor(){

    using namespace std::literals;

    Editor temp;
    ASSERT_EQUAL_HINT(temp.GetText(), ""s, "Wrong constructor!"s);
};

// test GetText

void TestGetText() {
    using namespace std::literals;

    Editor temp;
    const std::string text = "alloha"s;
    for (char c : text) {
        temp.Insert(c);
    }
    ASSERT_EQUAL_HINT(temp.GetText(), "alloha"s, "Wrong GetText function!"s);

};

// test Insert
void TestInsert(){
    using namespace std::literals;

    Editor temp;
    temp.Insert('a');
    ASSERT_EQUAL_HINT(temp.GetText(), "a"s, "Wrong Insert function!"s);
    temp.Insert('b');
    ASSERT_EQUAL_HINT(temp.GetText(), "ab"s, "Wrong Insert function!"s);

};

// test Copy
void TestCopy(){
    using namespace std::literals;

    Editor temp;
    const std::string text = "alloha"s;

    for (char c : text) {
        temp.Insert(c);
    }

    temp.Copy();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), ""s, "Wrong Copy function!"s);
    temp.Left();
    temp.Copy();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "a"s, "Wrong Copy function!"s);
    temp.Left();
    temp.Copy(2);
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "ha"s, "Wrong Copy function!"s);
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Copy(6);
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "alloha"s, "Wrong Copy function!"s);
    temp.Right();
    temp.Copy(5);
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "lloha"s, "Wrong Copy function!"s);
    temp.Right();
    temp.Copy(4);
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "loha"s, "Wrong Copy function!"s);
    temp.Right();
    temp.Right();
    temp.Right();
    temp.Right();
    temp.Right();
    temp.Copy();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), ""s, "Wrong Copy function!"s);
    
};

// test Cut function
void TestCut() {
    using namespace std::literals;

    Editor temp;
    const std::string text = "alloha"s;

    for (char c : text) {
        temp.Insert(c);
    }

    temp.Cut();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), ""s, "Wrong Cut function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "alloha"s, "Wrong Cut function!"s);
    temp.Left();
    temp.Cut();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "a"s, "Wrong Cut function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "alloh"s, "Wrong Cut function!"s);
    temp.Left();
    temp.Cut();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "h"s, "Wrong Cut function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "allo"s, "Wrong Cut function!"s);
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Cut();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "a"s, "Wrong Cut function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "llo"s, "Wrong Cut function!"s);
    temp.Right();
    temp.Cut(2);
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "lo"s, "Wrong Cut function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "l"s, "Wrong Cut function!"s);
   
};

// test Paste function
void TestPaste() {
    using namespace std::literals;

    Editor temp;
    const std::string text = "alloha"s;

    for (char c : text) {
        temp.Insert(c);
    }

    temp.Paste();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), ""s, "Wrong Paste function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "alloha"s, "Wrong Paste function!"s);
    temp.Copy();
    temp.Paste();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), ""s, "Wrong Paste function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "alloha"s, "Wrong Paste function!"s);
    temp.Cut();
    temp.Paste();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), ""s, "Wrong Paste function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "alloha"s, "Wrong Paste function!"s);
    
    temp.Left();
    temp.Copy();
    temp.Paste();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "a"s, "Wrong Paste function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "allohaa"s, "Wrong Paste function!"s);
    temp.Copy();
    temp.Paste();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "a"s, "Wrong Paste function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "allohaaa"s, "Wrong Paste function!"s);
    temp.Left();
    temp.Left();
    temp.Cut(3);
    temp.Left();
    temp.Paste();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "aaa"s, "Wrong Paste function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "alloaaah"s, "Wrong Paste function!"s);
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Left();
    temp.Copy(4);
    temp.Right();
    temp.Right();
    temp.Right();
    temp.Right();
    temp.Paste();
    ASSERT_EQUAL_HINT(temp.GetBuffer(), "allo"s, "Wrong Paste function!"s);
    ASSERT_EQUAL_HINT(temp.GetText(), "alloalloaaah"s, "Wrong Paste function!"s);

};

// test cursor positioning
void TestCursor() {
    using namespace std::literals;
    //test constructor
    {
        Editor temp;
        std::string a{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(a, "|"s, "Wrong cursor position!"s);
    }
    // test Insert
    {
        Editor temp;
        temp.Insert('a');
        std::string a{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(a, "|"s, "Wrong cursor position!"s);
        temp.Insert('b');
        std::string b{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(b, "|"s, "Wrong cursor position!"s);
    }
    //test Left function
    {
        Editor temp;
        temp.Insert('a');
        temp.Insert('b');
        temp.Insert('c');
        temp.Left();
        std::string a{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(a, "|"s, "Wrong cursor position!"s);
        temp.Left();
        std::string b{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(b, "|"s, "Wrong cursor position!"s);
        temp.Left();
        std::string c{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(c, "|"s, "Wrong cursor position!"s);
        temp.Left();
        std::string d{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(d, "|"s, "Wrong cursor position!"s);
    }
    //test Right function
    {
        Editor temp;
        temp.Insert('a');
        temp.Insert('b');
        temp.Insert('c');
        temp.Left();
        temp.Left();
        temp.Left();
        temp.Right();
        std::string a{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(a, "|"s, "Wrong cursor position!"s);
        temp.Right();
        std::string b{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(b, "|"s, "Wrong cursor position!"s);
        temp.Right();
        std::string c{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(c, "|"s, "Wrong cursor position!"s);
        temp.Right();
        std::string d{*(temp.GetCursor())};
        ASSERT_EQUAL_HINT(d, "|"s, "Wrong cursor position!"s);
    }

};

// test final
void TestEditor(){
    RUN_TEST(TestConstructor);
    RUN_TEST(TestGetText);
    RUN_TEST(TestInsert);
    RUN_TEST(TestCursor);
    RUN_TEST(TestCopy);
    RUN_TEST(TestCut);
    RUN_TEST(TestPaste);
};

int main() {

    TestEditor();

   /*  using namespace std;

    Editor editor;
    const string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText(); */
    return 0;
}