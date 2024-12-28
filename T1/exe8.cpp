#include <string>
#include <iostream>
#include <list>
#include <algorithm>


class Editor {
public:
    Editor(){
        text_.push_back(cursor_);
        cursor_pos_ = text_.begin();
    };

    // сдвинуть курсор влево
    void Left(){
        
        auto temp_it = cursor_pos_;
        auto temp_pos = cursor_pos_;

        if (cursor_pos_ == text_.begin()){
            return;
        }
        else {
            text_.insert(--temp_it, '|');
            text_.erase(temp_pos);
            cursor_pos_ = temp_it;
        }
        return;
    };

    // сдвинуть курсор вправо 
    void Right(){
        
        auto temp_it = cursor_pos_;
        auto temp_pos = cursor_pos_;

        if (cursor_pos_ == text_.end()){
            return;
        }
        else {
            std::advance(temp_it, 2);
            text_.insert(temp_it, '|');
            text_.erase(temp_pos);
            cursor_pos_ = temp_it;
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
       
        buffer_.clear();
        Copy(tokens);
        
        auto temp_it = cursor_pos_;
        auto temp_pos = cursor_pos_;

        std::advance(temp_it, tokens +1);
        auto end_it = temp_it;
        text_.erase(temp_pos,end_it);
        cursor_pos_ = text_.insert(end_it, cursor_);
        ++cursor_pos_;

    }

    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(std::size_t tokens = 1){
        
        buffer_.clear();
        
        auto temp_it = cursor_pos_;
        std::advance(temp_it, tokens + 1);
        auto end_it = temp_it;
        temp_it = cursor_pos_;
        
        for(++temp_it; temp_it != end_it; ++temp_it){
            if (temp_it == text_.end()){
                buffer_.push_back(*temp_it);
                break;
            }
            else {
                buffer_.push_back(*temp_it);
            }
        }
        return;
    }

    // вставить содержимое буфера в текущую позицию курсора
    void Paste(){
        if (!buffer_.empty()) {
            std::list temp_buffer = buffer_;
            text_.splice(cursor_pos_, temp_buffer);
            std::advance(cursor_pos_, buffer_.size());
        }
        else {
            return;
        }
    }
    
    // получить текущее содержимое текстового редактора
    std::string GetText() const{
        
        std::string temp;
        temp.reserve(text_.size() + 1);
        
        for(char ch : text_){
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

int main() {

    using namespace std;
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
    editor.Cut(2);  // Будут вырезаны 2 символа 

    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();
    return 0;
}