#pragma once
#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <utility>
#include <type_traits>


using namespace std;

template <typename Type> class SimpleVector;

class ReserveProxyObj{
    public:
        ReserveProxyObj(size_t capacity_to_reserve){
            capacity = capacity_to_reserve;
        }
    size_t  capacity;
    
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}



template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
    
    SimpleVector(ReserveProxyObj obj){
        size_ = 0;
        cap_ = obj.capacity;
        v_ = new Type[cap_];
    }

    SimpleVector() noexcept{
        size_ = 0;
        cap_ = 0;
        v_ = new Type[1];
    };

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size, const Type& val = value_type()) {
        size_=size;
        cap_ = size;
        v_ = new Type[cap_];
        std::fill_n(v_.begin(), size_, val);
        
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) {
        size_=size;
        cap_ = size;
        v_ = new Type[cap_];
        std::fill_n(v_.begin(), size_, value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        size_= init.size();
        cap_= init.size();
        v_ = new Type[cap_];
        size_t i = 0;
        std::copy(init.begin(),init.end(), v_.begin());
    }
    
    ~SimpleVector(){
        if(size_ != 0){
            delete v_;
        }
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return cap_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        if(size_ == 0){
           return true;
        }
        return false;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        assert(index < size_);
        return v_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(index < size_);
        return v_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if(index >= size_){
            throw std::out_of_range("out_of_range");
        }
        return v_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if(index >= size_){
            throw std::out_of_range("out_of_range");
        }
        return v_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if(new_size <= size_){
            size_ = new_size;
        } else if(new_size <= cap_) {
           for(size_t i = size_; i< new_size; ++i){
                v_[i] = Type();
            }
            size_ = new_size;  
        }else {
            Type* v2_= new Type[new_size];
            for(size_t i = 0; i< size_; ++i){
                v2_[i] = move(v_[i]);
            }
            for(size_t i = size_; i< new_size; ++i){
                v2_[i] = Type();
            }
            delete []v_;
            size_ = new_size;
            cap_ = new_size;
            v_ = v2_;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        if(size_== 0){
            return nullptr;
        }
        return v_;
        
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        if(size_== 0){
            return nullptr;
        }
        return &v_[size_];
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        if(size_== 0){
            return nullptr;
        }
        return v_;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        if(size_== 0){
            return nullptr;
        }
        return &v_[size_];
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        if(size_== 0){
            return nullptr;
        }
        return v_;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        if(size_== 0){
            return nullptr;
        }
        return &v_[size_];
    }
    
    SimpleVector(const SimpleVector& other) {
            size_= other.size_;
            cap_ = other.cap_;
            v_ = new Type[cap_];
            std::copy(other.begin(),other.end(),v_.begin());
    }
    
    SimpleVector(SimpleVector&& other) {
        
            size_= other.size_;
            cap_ = other.cap_;
            v_ = new Type[cap_];

            for(size_t i = 0; i < size_; ++i){
                v_[i] = move(other.v_[i]);
            }
            other.size_ = 0;
        
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if(rhs != *this){
            size_= rhs.size_;
            cap_ = rhs.cap_;
            delete []v_;
            v_ = new Type[cap_];

            for(size_t i = 0; i < size_; ++i){
                v_[i] = rhs.v_[i];
            }
        }
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if(size_ < cap_){
            v_[size_] = item;
            ++size_;
        }else{
            if(cap_ == 0){
                cap_ = 1;
            }else{
                cap_ = 2*cap_;
            }
            Type* v2_= new Type[cap_];
            for(size_t i = 0; i< size_; ++i){
                v2_[i] = v_[i];
            }
            v2_[size_] = item;
            delete []v_;
            ++size_;
            v_ = v2_;
        }
    }
    
    void PushBack(Type&& item) {
        if(size_ < cap_){
            v_[size_] = move(item);
            ++size_;
        }else{
            if(cap_ == 0){
                cap_ = 1;
            }else{
                cap_ = 2*cap_;
            }
            Type* v2_= new Type[cap_];
            for(size_t i = 0; i< size_; ++i){
                v2_[i] = move(v_[i]);
            }
            v2_[size_] = move(item);
            delete []v_;
            ++size_;
            v_ = v2_;
        }
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(pos >= begin() && pos <= end());
        if(size_==0 && cap_!=0) {
            v_[0] = value;
            size_ = 1;
            pos = v_;
            return const_cast<Iterator>(pos);
        }
        if(size_ < cap_){
            
            for(long int i = size_ - 1; i >= const_cast<Iterator>(pos) - begin(); --i){
                v_[i + 1] = v_[i];
            }
            ++size_;
            //copy_backward(const_cast<Iterator>(pos), end() - 1, end());
            *(const_cast<Iterator>(pos)) = value;
        }else {
            if(cap_ == 0){
                cap_ = 1;
                v_ = new Type[cap_];
                v_[0] = value;
                size_ = 1;
                pos = v_;
            }else{
                cap_ = 2*cap_;
                Type* v2_= new Type[cap_];
                //copy(begin(), const_cast<Iterator>(pos), v2_);
                auto it = begin(), it2 = v2_;
                int step=0;
                for( ; it!=pos; it++, it2++, step++)
                    *it2 = *it;
                v2_[step] = value;     
                for(it2++; it!=end(); it++, it2++)
                    *it2 = *it;             
                //copy_backward (const_cast<Iterator>(pos), end(), &v2_[size_ + 1]);
                delete []v_;
                ++size_;
                v_ = v2_;
                pos = v2_ + step;
            }
        }
        return const_cast<Iterator>(pos);
        
    }
    
    Iterator Insert(ConstIterator pos, Type&& value) {
        assert(pos >= begin() && pos <= end());
        if(size_==0 && cap_!=0) {
            v_[0] = move(value);
            size_ = 1;
            pos = v_;
            return const_cast<Iterator>(pos);
        }
        if(size_ < cap_){
            
            for(long int i = size_ - 1; i >= const_cast<Iterator>(pos) - begin(); --i){
                v_[i + 1] = move(v_[i]);
            }
            ++size_;
            
            *(const_cast<Iterator>(pos)) = move(value);
        }else {
            if(cap_ == 0){
                cap_ = 1;
                v_ = new Type[cap_];
                v_[0] = move(value);
                size_ = 1;
                pos = v_;
            }else{
                cap_ = 2*cap_;
                Type* v2_= new Type[cap_];
                auto it = begin(), it2 = v2_;
                int step=0;
                for( ; it!=pos; it++, it2++, step++)
                    *it2 = move(*it);
                v2_[step] = move(value);     
                for(it2++; it!=end(); it++, it2++)
                    *it2 = move(*it);             
                delete []v_;
                ++size_;
                v_ = v2_;
                pos = v2_ + step;
            }
        }
        return const_cast<Iterator>(pos);
        
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if(size_ > 0){
            --size_;
        }
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(pos >= begin() && pos <= end());
        for(Type* it = const_cast<Iterator>(pos), *it2 = const_cast<Iterator>(pos) +1; it!= end(); ++it, ++it2){
            *it= move(*it2);
        }
        --size_;
        if(size_==0)
            pos = nullptr;
        return const_cast<Iterator>(pos);
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        if(other != *this){
            std::swap(other.size_, size_);
            std::swap(other.cap_, cap_);
            std::swap(other.v_, v_);
        }
    }
    
    void Reserve(size_t new_capacity){
        if(cap_ < new_capacity){
            cap_ = new_capacity;
            Type* v2 = new Type[cap_];
            for(size_t i = 0; i < size_; ++i){
                v2[i] = v_[i];
            }
            delete []v_;
            v_ = v2;
        }
    }; 
 
    private:
    ArrayPtr<Type> v_;
    size_t size_;
    size_t cap_;
    
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !lexicographical_compare(lhs.begin(), lhs.end(),rhs.begin(), rhs.end()) && !lexicographical_compare(rhs.begin(), rhs.end(),lhs.begin(), lhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return std::lexicographical_compare(lhs.begin(), lhs.end(),rhs.begin(), rhs.end()) || std::lexicographical_compare(rhs.begin(), rhs.end(),lhs.begin(), lhs.end());;
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return std::lexicographical_compare(lhs.begin(), lhs.end(),rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return lhs < rhs || lhs == rhs;
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return !std::lexicographical_compare(lhs.begin(), lhs.end(),rhs.begin(), rhs.end()) && lhs != rhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return !std::lexicographical_compare(lhs.begin(), lhs.end(),rhs.begin(), rhs.end());
} 
