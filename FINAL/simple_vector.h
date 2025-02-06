#pragma once

#include "array_ptr.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>
#include <initializer_list>

struct ReserveProxyObj
{
    size_t capacity_to_reserve;

    ReserveProxyObj(size_t capa) 
    {
        capacity_to_reserve = capa;
    }
    
};


ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
};


template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) 
    : items_(size)
    , size_(size)
    , capacity_(size_)
    {

        std::fill(begin(), end(), 0);

    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) 
    : items_(size)
    , size_(size)
    , capacity_(size_)
    {

        std::fill(begin(), end(), value);
        
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) 
    : items_(init.size())
    , size_(init.size())
    , capacity_(size_)
    {
        std::move(init.begin(), init.end(), items_.Get());

    }

    // Конструктор копирования
    SimpleVector(const SimpleVector& other) 
    : items_(other.GetCapacity())
    , size_(other.GetSize())
    , capacity_(other.GetCapacity())
    {
        
        std::copy(other.begin(), other.end(), begin());

    }

    // Конструктор присваивания
    SimpleVector& operator=(const SimpleVector& rhs) {
        
        if (rhs.cbegin() == cbegin()) {
            return *this;
        }

        SimpleVector tmp (rhs);
        swap(tmp);

        return *this;
    }

    // Конструктор резервирования
    SimpleVector(ReserveProxyObj temp) {
        
        this -> Reserve(temp.capacity_to_reserve);

    }
    // конструктор копирования
    SimpleVector(SimpleVector&& source) noexcept {
        items_.swap(source.items_);
        std::swap(capacity_, source.capacity_);
        std::swap(size_, source.size_);
        source.Clear();
    }

    // Конструктор присваивания перемещением
    SimpleVector& operator=(SimpleVector&& rhs) noexcept {
        if (this != &rhs) {
            items_.swap(rhs.items_);
            std::swap(size_, rhs.size_);
            std::swap(capacity_, rhs.capacity_);
            rhs.Clear();
        }

        return *this;
    }

    void Reserve(size_t new_capacity) {
        
        if (new_capacity <= capacity_) {
            return;
        }

        ArrayPtr<Type> temp(new_capacity);
        std::move(begin(), end(), temp.Get());
        items_.swap(temp);
        capacity_ = new_capacity;

    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        
        if (size_ == 0 && capacity_ == 0) {
            Resize(1);
            items_[0] = item;
        }
        else if (size_ < capacity_) {
            items_[size_] = item;
            ++size_;
        }
        else {
            int old_size = size_;
            Resize(capacity_ * 2);
            size_ = old_size;
            items_[size_] = item;
            ++size_;
            
        }
    }

    // Pushback перемещением
    void PushBack(Type&& item) {
        
        if (size_ == 0 && capacity_ == 0) {
            Resize(1);
            std::exchange(items_[0], std::move(item));
        }
        else if (size_ < capacity_) {
            std::exchange(items_[size_], std::move(item));
            ++size_;
        }
        else {
            int old_size = size_;
            Resize(capacity_ * 2);
            size_ = old_size;
            std::exchange(items_[size_], std::move(item));
            ++size_;
            
        }
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {

        assert(!(pos < begin()));
        assert(!(pos > end()));

        auto pos_number = pos - begin();
        Iterator pos_temp {begin() + pos_number};

        if(size_ == 0 && capacity_ == 0) {
            
            ArrayPtr<Type> temp(1);
            temp[0] = value;
            items_.swap(temp);
            //items_[0] = value;

            ++size_;
            ++capacity_;
            return Iterator{begin()};
        
        }
        else if (size_ < capacity_) {
            
            std::copy_backward(pos_temp, end(), end() + 1);
            items_[static_cast<size_t> (pos_number)] = value;
            ++size_;
            return Iterator {begin() + pos_number};

        }
        else {
            const size_t new_capacity = capacity_ * 2;
            ArrayPtr<Type> temp(new_capacity);
            std::fill(&temp[0], &temp[new_capacity - 1], 0);
    
            Iterator it_pos = std::copy(begin(), pos_temp, &temp[0]);

            temp[pos_number] = value;
            std::copy(pos_temp, end(), it_pos + 1);

            items_.swap(temp);
            capacity_ = new_capacity;
            ++size_;
            return Iterator {begin() + pos_number};
            
        }

    }
    // Insert перемещением
    Iterator Insert(ConstIterator pos, Type&& value) {

        assert(!(pos < begin()));
        assert(!(pos > end()));

        auto pos_number = pos - begin();
        Iterator pos_temp {begin() + pos_number};

        if(size_ == 0 && capacity_ == 0) {
            
            Resize(1);
            std::exchange(items_[0], std::move(value));
            //++size_;
            //++capacity_;
            return Iterator{begin()};
        
        }
        else if (size_ < capacity_) {
            
            std::move_backward(pos_temp, end(), end() + 1);
            std::exchange(items_[pos_number], std::move(value));
            ++size_;
            return Iterator {begin() + pos_number};

        }
        else {
            const size_t new_capacity = capacity_ * 2;
            ArrayPtr<Type> temp(new_capacity);
            MoveFill(temp.Get(), temp.Get() +  new_capacity - 1);
            std::move(begin(), pos_temp, temp.Get());
            std::exchange(temp[pos_number], std::move(value));
            std::move(pos_temp, end(), temp.Get() + pos_number + 1);

            items_.swap(temp);
            capacity_ = new_capacity;
            ++size_;
            return Iterator {begin() + pos_number};
            
        }

    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
       
        assert(!(IsEmpty()));
        --size_;

    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        
        assert(!(pos == nullptr));
        assert(!(pos < begin()));
        assert(!(pos >= end()));

        std::move(Iterator(pos + 1), end(), Iterator(pos));
        --size_;
        return Iterator(pos);

    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        assert(!(this == &other));

        this->items_.swap(other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);

    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        
        return size_;

    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        
        return capacity_;

    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {

        return !(size_);

    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        
        return items_[index];

    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        
        return items_[index];

    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
       
        using namespace std::literals;

        if (index >= size_) {

            throw std::out_of_range("Too big index!"s);
        
        }
        
        return items_[index];
        
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        
        using namespace std::literals;

        if (index >= size_) {

            throw std::out_of_range("Too big index!"s);
        
        }
        
        return items_[index];

    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {

        if (new_size <= size_) {
            size_ = new_size;
        }
        else if (new_size > size_ && new_size <= capacity_){
            MoveFill(begin() + size_, begin() + new_size);
            size_ = new_size;
        }
        else {
            const size_t new_capacity = std::max(new_size, 2 * capacity_);
            ArrayPtr<Type> temp(new_capacity);
            MoveFill(temp.Get() + size_, temp.Get() + new_capacity - 1);
            std::move(begin(), end(), temp.Get());

            items_.swap(temp);
            size_ = new_size;
            capacity_ = new_capacity;
        }

    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return items_.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return items_.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return items_.Get() + size_;
    }
    
private:
    
    ArrayPtr<Type> items_;

    size_t size_ = 0;
    size_t capacity_ = 0;

    void MoveFill(Iterator first, Iterator last)
    {
        //assert(first < last);
        while (first < last)
        {
            *first = std::move(Type());
            ++first;
        }
    }
    
};



template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return std::equal(lhs.cbegin(),lhs.cend(), rhs.cbegin());;

}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return !(lhs == rhs);

}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return std::lexicographical_compare(lhs.cbegin(),lhs.cend(),rhs.cbegin(), rhs.cend());

}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return !(rhs < lhs);

}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return rhs < lhs;

}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return !(rhs > lhs);

} 