#ifndef HEADER_93B32F70A69F4B74
#define HEADER_93B32F70A69F4B74
#include <iostream>

// http://www.cplusplus.com/reference/vector/vector/

/* Vector:
    - container
    - resize a.i. sa ocupe ~ numar de el. memorie
        - capacity = 2^k spatiu unde k minim, k >= size
    - tipul de elemente dat prin template

    Functionalitati:
        - contructori:
            - ()
            - (int size, default_val)
            - (vector)

        - size()
        - capacity()
        - resize(int size)
        - empty()
        - reserve(int capacity)
        - shrink_to_fit()

        - front()
        - back()
        - operator [] (int pos)
        - begin()
        - end()

        - push_back(x)
        - pop_back()
        // - insert -> 3 variante
        - erase -> sterge range de elemente
        - clear()
*/

#pragma once
#include <cassert>

template<class type>
class vector {
public:
    vector();
    vector(const unsigned int size);
    vector(const unsigned int size, const type& default_value);
    vector(const vector<type>& other);
    ~vector();

    unsigned int size() const;
    unsigned int capacity() const;
    bool empty() const;
    void resize(const unsigned int size, const type& default_value = type());
    // face capacitatea macar capacitatea data
    void reserve(const unsigned int capacity);
    void shrink_to_fit();

    type& front();
    const type& front() const;
    type& back();
    const type& back() const;
    type& operator [](const int pos);
    const type& operator [](const int pos) const;
    type * begin();
    type * end();

    const int binary_search(const type val) const;
    void push_back(const type& value);
    void pop_back();
    void clear();

    // nu inca
    // void erase(type* begin, type* end);

private:
    static constexpr unsigned int kMinCapacity = 1;

    // modif capacity_ => realloc
    // size_ <= capacity_
    unsigned int size_ = 0;
    unsigned int capacity_ = 0;
    type * contents_;
};

template<class type>
vector<type>::vector() {
    capacity_ = kMinCapacity;
    contents_ = new type[capacity_];
}

template<class type>
vector<type>::vector(const unsigned int size) {
    size_ = capacity_ = size;
    contents_ = new type[capacity_]();
}

template<class type>
vector<type>::vector(const unsigned int size, const type& default_value) {
    size_ = capacity_ = size;
    contents_ = new type[capacity_];
    for (int i = 0; i < size; i++) 
        contents_[i] = default_value;
}

template<class type>
vector<type>::vector(const vector<type>& other) {
    this->size_ = other.size_;
    this->capacity_ = other.capacity_;
    this->contents_ = new type[capacity_];
    for (int i = 0; i < size_; i++) 
        contents_[i] = other.contents_[i];
}

template<class type>
vector<type>::~vector() {
    size_ = capacity_ = 0;
    delete[] contents_;
}

template<class type>
unsigned int vector<type>::size() const {
    return size_;
}

template<class type>
unsigned int vector<type>::capacity() const {
    return capacity_;
}

template<class type>
bool vector<type>::empty() const {
    return (size_ == 0);
}

// v = 4 5 2 4 3
// v.resize()
// daca size_ > size:
//      - micsoram numarul de elemente
// daca size_ < size:
//      - crestem size_
//      - size > capacity_ -> capacity_ creste
template<class type>
void vector<type>::resize(const unsigned int size, const type& default_value) {
    contents_ = (type*)realloc(contents_, size * sizeof(type));
    if(size > size_){
        for(int i = size_; i < size; i++)
            contents_[i] = default_value;
    }
    size_ = capacity_ = size;
}

// Daca capacitatea actuala este mai mica decat cea data,
// aloca suficient cat cea data sa incapa
template<class type>
void vector<type>::reserve(const unsigned int capacity) {
    //assert(capacity_ <= capacity);
    if(capacity_ < capacity)
    {
        capacity_ = capacity;
        contents_ = (type*)realloc(contents_, capacity_ * sizeof(type));
    }
}

// vector 5 elem => 5 elem in memorie
// mai exact, capacitatea ar trebui sa ajunga exact cat size-ul
template<class type>
void vector<type>::shrink_to_fit() {
    capacity_ = size_;
    contents_ = (type*)realloc(contents_, capacity_ * sizeof(type));

}

// daca size-ul depaseste capacitatea => dublam capacitatea
// si realocam
template<class type>
void vector<type>::push_back(const type& x) {
    if(size_ == capacity_)
    {
        capacity_ *= 2;
        contents_ = (type*)realloc(contents_, capacity_ * sizeof(type));
    }
    contents_[size_] = x;
    size_++;
}

// daca size_ <= capacity_ / 3 => capacity_ /= 2
// si realocam
template<class type>
void vector<type>::pop_back() {
    size_--;
    if(size_ <= capacity_ / 3)
    {
        capacity_ /= 2;
        contents_ = (type*)realloc(contents_, capacity_ * sizeof(type));
    }
}

// elimina elementele vectorului fara a le dealoca
// => size_ ajunge 0, dar capacity nu neaparat
template<class type>
void vector<type>::clear() {
    size_ = 0;
}

// random access (v[i])
template<class type>
type& vector<type>::operator [](const int pos) {
    assert(0 <= pos && pos < size_);
    return contents_[pos];
}

// random access (v[i])
template<class type>
const type& vector<type>:: operator [](const int pos) const {
    assert(0 <= pos && pos < size_);
    return contents_[pos]; 
}

// returneaza referinta la primul element
template<class type>
type& vector<type>::front() {
    assert(!empty());
    return contents_[0];
}

// returneaza referinta la primul element
template<class type>
const type& vector<type>::front() const {
    assert(!empty());
    return contents_[0];
}

// returneaza referinta la ultimul element
template<class type>
type& vector<type>::back() {
    assert(!empty());
    return contents_[size_ - 1];
}

// returneaza referinta la ultimul element
template<class type>
const type& vector<type>::back() const {
    assert(!empty());
    return contents_[size_ - 1];
}

// pointer la primul element
// for (type* it = v.begin(); it != v.end(); it++)
template<class type>
type * vector<type>::begin() {
    assert(!empty());
    return contents_;
}

// pointer la ultimul element
// for (type* it = v.begin(); it != v.end(); it++)
template<class type>
type * vector<type>::end() {
    assert(!empty());
    return contents_ + size_;
}

template<class type>
std::ostream& operator<< (std::ostream& out, const vector<type>& to_print) {
    for (int i = 0; i < to_print.size(); i++) {
        out << to_print[i] << " ";
    }
    return out;
}

/*template<class type>
std::ifstream& operator>> (std::ifstream& in, vector<type>& to_read) {
    
}*/

template<class type>
const int vector<type>::binary_search(const type val) const {
    for(int i = 0; i < this->size_ - 1; i++)
        if(contents_[i] >= contents_[i + 1])
        {
            throw std::runtime_error("Position is not valid!");
            return -1;
        }
    unsigned int st = 0, dr = this->size_ - 1;
    while(st <= dr)
    {
        unsigned int mijloc = (st + dr) / 2;
        if(contents_[mijloc] == val)
            return mijloc;
        else if(contents_[mijloc] < val)
            st = mijloc + 1; 
        else 
            dr = mijloc - 1; 
    }
    return -1;
}

#endif // header guard