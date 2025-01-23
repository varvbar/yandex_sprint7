#pragma once

#include <array>
#include <stdexcept>

using namespace std::literals;

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0){
        if (a_size > v_.size()) {
            throw std::invalid_argument("Bad size for constructor"s);
        }
        else {
            vector_size_ = a_size;
        }
    };

    T& operator[](size_t index){
        return v_[index];
    };
    const T& operator[](size_t index) const{
        if (index <= vector_size_){
            return v_.at(index);
        } 
        else {
            throw std::invalid_argument("Wrong index!"s);
        }
    };

    auto begin(){
        return v_.begin();
    };
    auto end(){
        return &v_[vector_size_];
    };
    auto begin() const{
        return v_.cbegin();
    };
    auto end() const{
        return &v_[vector_size_];
    };

    size_t Size() const{
        return vector_size_;
    };
    size_t Capacity() const{
        return v_.size();
    };

    void PushBack(const T& value){
        if (v_.size() == vector_size_){
            throw std::overflow_error("Vector has maximum size"s);
        }
        else {
            ++vector_size_;
            v_[vector_size_ - 1] = value;
        }
    };

    T PopBack(){
        if(vector_size_ == 0){
            throw std::underflow_error("Vector is empty"s);
        }
        else {
            
            auto popback = v_[vector_size_ - 1];
            v_[vector_size_ - 1] = {};
            --vector_size_;
            return popback;
        }
    };

private:
std::array<T, N> v_;
size_t vector_size_ = 0;

};
