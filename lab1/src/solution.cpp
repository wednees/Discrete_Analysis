#include <cstddef>
#include <string>
#include <iostream>
#include <fstream>

namespace vector {
    template <class T>
    class Vector{
    public:
        Vector();
        explicit Vector(size_t n);
        Vector(const Vector &other);
        ~Vector();

        Vector& operator= (const Vector& other);
        T& operator[](const int& index);
        void push_back(T& value);
        bool empty();
        size_t size();
    private:
        T* buf_;
        size_t size_;
        size_t capacity_;
    };

    template <class T>
    Vector<T>::Vector(){
        size_ = 0;
        capacity_ = 2;
        buf_ = new T[capacity_];
    }

    template <class T>
    Vector<T>::Vector(size_t n){
        size_ = n;
        capacity_ = n;
        buf_ = new T[capacity_];
    }

    template <class T>
    Vector<T>::Vector(const Vector &other){

        this->capacity_ = other.capacity_;
        this->size_ = other.size_;

        
        buf_ = new T[capacity_];

        for (size_t i = 0; i < this->size_; ++i){
            this->buf_[i] = other.buf_[i];
        }
    }

    template <class T>
    Vector<T>::~Vector(){
        delete[] buf_;
        buf_ = nullptr;
    }

    template <class T>
    T& Vector<T>::operator[](const int& index){
        return this->buf_[index];
    }

    template <class T>
    void Vector<T>::push_back(T& value){
        if (size_ == capacity_){
            capacity_ *= 2;
            T* tmp = new T[capacity_];
            for (size_t i = 0; i < size_; ++i){
                tmp[i] = buf_[i];
            }
            delete[] buf_;
            buf_ = tmp;
        }
        buf_[size_] = value;
        size_++;
    }

    template <class T>
    size_t Vector<T>::size(){
        return size_;
    }

    template <class T>
    Vector<T>& Vector<T>::operator= (const Vector& other){
        if (this != &other){
            this->size_ = other.size_;
            this->capacity_ = other.capacity_;
            delete [] this->buf_;
            this->buf_ = new T [this->capacity_];
            for (size_t i = 0; i < this->size_; ++i){
                this->buf_[i] = other.buf_[i];
            }
        }
        return *this;
    }

    template <class T>
    bool Vector<T>::empty(){
        return size_ == 0;
    }
}

struct Pair{
    unsigned short first;
    std::string second;
    // char second[2049];
};

namespace sort{
    void counting_sort(vector::Vector<Pair> &elems){
        
        if(elems.empty()){
            return;
        }

        unsigned short max_elem = 0;
        for(size_t i = 0; i < elems.size(); ++i){
            max_elem = (elems[i].first > max_elem) ? elems[i].first : max_elem;
        }

        // vector::Vector<int> tmp(max_elem + 1);
        int tmp[max_elem + 1] = {0};
        // for(size_t i = 0; i < tmp.size(); ++i){
        //     tmp[i] = 0;
        // }

        for(size_t i = 0; i < elems.size(); ++i){
            ++tmp[elems[i].first];
        }

        for (size_t i = 1; i < max_elem + 1; ++i){
            tmp[i] += tmp[i - 1];
        }

        vector::Vector<Pair> res(elems.size());
        // size_t pos;
        for(int i = elems.size() - 1; i >= 0; --i){
            //unsigned short key = elems[i].first;
            size_t pos = tmp[elems[i].first]--;
            res[pos-1] = elems[i];
        }

        // std::swap(elems, res);
        elems = res;
    }

}

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    vector::Vector<Pair> arr;
    Pair elem;
    // std::ifstream is("test.txt");
    // std::ofstream os("output.txt");
    // unsigned short key;
    // std::string val;
    // val.reserve(2048);

    while(std::cin >> elem.first >> elem.second){
        arr.push_back(elem);
    }

    // while(is >> elem.first >> elem.second){
    //     arr.push_back(elem);
    // }

    sort::counting_sort(arr);


    for(size_t i = 0; i < arr.size(); ++i){
        std::cout << arr[i].first << "\t" << arr[i].second << '\n';
        // os << arr[i].first << "\t" << arr[i].second << '\n';
    }

    return 0;
}