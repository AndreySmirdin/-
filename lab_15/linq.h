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
    virtual const T& operator*() const = 0;
    virtual enumerator& operator++() = 0;
    virtual operator bool() const = 0;

    auto drop(int count) {
        return drop_enumerator<T>(*this, count); // I dont like this class. Function will be better.
    }

    auto take(int count) {
        return take_enumerator<T>(*this, count);
    }

    template<typename U = T, typename F>
    auto select(F func) {
        return select_enumerator<U, T, F>(*this, func); // Order of template arguments is important.
    }

    template<typename F>
    auto until(F func) {
        return until_enumerator<T, F>(*this, func);
    }

    auto until_eq(const T& v){
        return until([&v](const T& x){ return x == v; });
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

    auto where_neq(const T& v){
        return where([&v](const T& x) { return x != v; });
    }
};

template<typename T, typename Iter>
class range_enumerator : public enumerator<T> {
public:
    range_enumerator(Iter begin, Iter end) : begin_(begin), end_(end) {
    }

    const T& operator*() const override {
        return *begin_;
    }

    range_enumerator& operator++() override {
        begin_++;
        return *this;
    }

    operator bool() const override {
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

    const T& operator*() const override { return *parent_; }

    drop_enumerator& operator++() override {
        ++parent_;
        return *this;
    }

    operator bool() const override { return parent_; }

private:
    enumerator<T>& parent_;
};


template<typename T>
class take_enumerator : public enumerator<T> {
public:
    take_enumerator(enumerator<T> &parent, int count) : parent_(parent), count_(count) {}

    const T& operator*() const override { return *parent_; }

    take_enumerator& operator++() override {
        ++parent_;
        --count_;
        return *this;
    }

    operator bool() const override {
        return count_ > 0 && parent_;
    }

private:
    enumerator<T>& parent_;
    int count_;
};


template<typename T, typename U, typename F>
class select_enumerator : public enumerator<T> {
public:
    select_enumerator(enumerator<U> &parent, F func) : parent_(parent), func_(std::move(func)) {
    }

    const T& operator*() const override {
        current_ = func_(*parent_);
        return current_;
    }

    select_enumerator& operator++() override {
        ++parent_;
        return *this;
    }

    operator bool() const override { return parent_; }

private:
    enumerator<U>& parent_;
    mutable F func_; // Need mutable because we want operator* to be const, but func_ can change.
    mutable T current_;
};


template<typename T, typename F>
class until_enumerator : public enumerator<T> {
public:
    until_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), predicate_((std::move(predicate))) {}

    const T& operator*() const override { return *parent_; }

    until_enumerator& operator++() override  {
        ++parent_;
        return *this;
    }

    operator bool() const override {
        return parent_ && !predicate_(*parent_);
    }
private:
    enumerator<T>& parent_;
    mutable F predicate_;
};


template<typename T, typename F>
class where_enumerator : public enumerator<T> {
public:
    where_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), predicate_(predicate) {
        if (parent_ && !predicate_(*parent_))
            this->operator++();
    }

    const T& operator*() const override { return *parent_; }

    where_enumerator& operator++() override  {
        ++parent_;
        while (parent_ && !predicate_(*parent_))
            ++parent_;
        return *this;
    }

    operator bool() const override { return parent_; }
private:
    enumerator<T>& parent_;
    F predicate_;
};

#endif