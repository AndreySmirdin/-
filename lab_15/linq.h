#ifndef LINQ_H_
#define LINQ_H_

#include <utility>
#include <vector>
#include <type_traits>

template<typename T, typename Iter>
class range_enumerator;
template<typename T>
class drop_enumerator;
template<typename T>
class take_enumerator;
template<typename T, typename U, typename F>
class select_enumerator;
template<typename T, typename F>
class until_enumerator;
template<typename T, typename F>
class where_enumerator;


template<typename T>
class enumerator {
public:
    virtual T operator*() = 0; // Получает текущий элемент.
    virtual enumerator& operator++() = 0;  // Переход к следующему элементу
    virtual operator bool() const = 0;  // Возвращает true, если есть текущий элемент

    auto drop(int count) {
        return drop_enumerator<T>(*this, count);
    }

    auto take(int count) {
        return take_enumerator<T>(*this, count);
    }

    template<typename U = T, typename F>
    auto select(F func) {
        return select_enumerator<U, T, F>(*this, func);
    }

    template<typename F>
    auto until(F func) {
        return until_enumerator<T, F>(*this, func);
    }

    auto until_eq(T v){
        return until([v](T x){ return x == v; });
    }

    std::vector<T> to_vector() {
        std::vector<T> ans;
        while (*this) {
            ans.push_back(**this);
            ++(*this);
        }
        return ans;
    }

    template<typename Iter>
    void copy_to(Iter it) {
        while (*this) {
            *it = **this;
            it++;
            ++(*this);
        }
    }

    template<typename F>
    auto where(F func){
        return where_enumerator<T, F>(*this, func);
    }

    auto where_neq(T v){
        return where([v](T x) { return x != v; });
    }
};

template<typename T, typename Iter>
class range_enumerator : public enumerator<T> {
public:
    range_enumerator(Iter begin, Iter end) : begin_(begin), end_(end) {
    }

    T operator*() {
        return *begin_;
    }

    range_enumerator& operator++() {
        begin_++;
        return *this;
    }

    operator bool() const {
        return begin_ != end_;
    }
private:
    Iter begin_, end_;
};


template<typename T>
auto from(T begin, T end) {
    return range_enumerator<typename std::iterator_traits<T>::value_type , T>(begin, end);
}


template<typename T>
class drop_enumerator : public enumerator<T> {
public:
    drop_enumerator(enumerator<T> &parent, int count) : parent_(parent) {
        for (int i = 0; i < count; i++)
            ++parent_;
    }

    T operator*(){ return *parent_; }

    drop_enumerator& operator++() {
        ++parent_;
        return *this;
    }

    operator bool() const { return parent_; }

private:
    enumerator<T>& parent_;
};


template<typename T>
class take_enumerator : public enumerator<T> {
public:
    take_enumerator(enumerator<T> &parent, int count) : parent_(parent), count_(count) { taken_ = 0;}

    T operator*()  { return *parent_; }

    take_enumerator& operator++() {
        ++parent_;
        ++taken_;
        return *this;
    }

    operator bool() const {
        return taken_ < count_ && parent_;
    }

private:
    enumerator<T>& parent_;
    int taken_, count_;
};


template<typename T, typename U, typename F>
class select_enumerator : public enumerator<T> {
public:
    select_enumerator(enumerator<U> &parent, F func) : parent_(parent), func_(func) {
    }

    T operator*()  {
        return func_(*parent_);
    }

    select_enumerator& operator++() {
        ++parent_;
        return *this;
    }

    operator bool() const { return parent_; }

private:
    enumerator<U>& parent_;
    F func_;
};


template<typename T, typename F>
class until_enumerator : public enumerator<T> {
public:
    until_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), predicate_(predicate) {}

    T operator*()  { return *parent_; }

    until_enumerator& operator++() {
        ++parent_;
        return *this;
    }

    operator bool() const {
        return parent_ && !predicate_(*parent_);
    }
private:
    enumerator<T>& parent_;
    F predicate_;
};


template<typename T, typename F>
class where_enumerator : public enumerator<T> {
public:
    where_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), predicate_(predicate) {
        if (parent_ && !predicate_(*parent_))
            this->operator++();
    }

    T operator*()  { return *parent_; }

    where_enumerator& operator++() {
        ++parent_;
        while (parent_ && !predicate_(*parent_))
            ++parent_;
        return *this;
    }

    operator bool() const { return parent_; }
private:
    enumerator<T>& parent_;
    F predicate_;
};

#endif