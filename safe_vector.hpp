#include <bits/stdc++.h>
using namespace std;

template<typename T>
class SafeVector {
private:
    struct ElementWraper;
    struct IteratorHelper;
public:
    class Iterator{
    public:
        using iterator_category = contiguous_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        Iterator(shared_ptr<IteratorHelper> ith): ith_(ith){}
        Iterator& operator++() {
            ElementWraper* ele = ith_->ele_;
            ++ele;
            ith_ = ele->ref_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator ret(this->ith_);
            ElementWraper* ele = ith_->ele_;
          //  cout<<ele<<"\n";
            ++ele;
          //  cout<<ele<<"\n";
            ith_ = ele->ref_;
            return ret;
        }
        Iterator& operator--() {
            ElementWraper* ele = ith_->ele_;
            --ele;
            ith_ = ele->ref_;
            return *this;
        }
        Iterator operator--(int) {
            Iterator ret(this->ith_);
            ElementWraper* ele = ith_->ele_;
            --ele;
            ith_ = ele->ref_;
            return ret;
        }
        T& operator*() {
            return ith_->ele_->element_;
        }
        T* operator->() {
            return &ith_->ele_->element_;
        }
        bool operator==(const Iterator& rig) {
            return this->ith_ == rig.ith_;
        }
        bool operator!=(const Iterator& rig) {
            return this->ith_ != rig.ith_;
        }
        friend SafeVector;
    private:
        ElementWraper& getElement() {
            return *ith_->ele_;
        }
        shared_ptr<IteratorHelper> ith_;
    };
    SafeVector() : size_(0), capacity_(1) {
        arr_ = new ElementWraper[2];
    }
    SafeVector(initializer_list<T> init) {
        size_ = 0;
        capacity_ = init.size();
        arr_ = new ElementWraper[capacity_ + 1];
        for (auto it : init) this->push_back(it);
    }
    ~SafeVector() {
        delete[] arr_;
    }
    T& operator[](size_t index) {
        return arr_[index].element_;
    }
    void push_back(const T& ele) {
        if (this->size_ == this->capacity_) this->increment();
        this->arr_[size_++].element_ = ele;
    }
    void insert(Iterator pos, const T& value) {
        if (this->size_ == this->capacity_) this->increment();
        ElementWraper* changed_arr = new ElementWraper[capacity_+1];
        size_t i = 0;
        for (auto it = this->begin();it!=pos;it++) {
            changed_arr[i] = it.getElement();
            changed_arr[i].ref_->ele_ = &changed_arr[i];
            i++;
        }
        changed_arr[i].element_ = value;
        i++;
        for (auto it=pos;it!=this->end();it++) {
            changed_arr[i] = it.getElement();
            i++;
        }
        changed_arr[i] = this->end().getElement();
        delete[] arr_;
        arr_ = changed_arr;
        size_++;
        for (i=0;i<=size_;i++) {
            arr_[i].ref_->ele_ = &arr_[i];
        }
    }
    Iterator begin() {
        return Iterator(arr_[0].ref_);
    }
    Iterator end() {
        return Iterator(arr_[size_].ref_);
    }
private:
    struct IteratorHelper{
        IteratorHelper(ElementWraper* item) : ele_(item) {}
        ElementWraper* ele_;
    };
    struct ElementWraper{
        ElementWraper() : element_() {
            ref_ = make_shared<IteratorHelper>(this);
        }
        ElementWraper(const T& item) : element_(item) {
            ref_ = make_shared<IteratorHelper>(this);
        }
        T element_;
        shared_ptr<IteratorHelper> ref_;
    };
    ElementWraper* arr_;
    size_t size_;
    size_t capacity_;
    void increment() {
        capacity_ *= 2;
        ElementWraper* new_arr = new ElementWraper[capacity_+1];
        for (size_t i = 0; i <= size_; i++) {
            new_arr[i] = arr_[i];
            new_arr[i].ref_->ele_ = &new_arr[i];
        }
        delete[] arr_;
        arr_ = new_arr;
    }
};