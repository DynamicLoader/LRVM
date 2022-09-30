#ifndef __LRVM_UTILS_HPP__
#define __LRVM_UTILS_HPP__

#include "lrvm_impl.hpp"

namespace LRVM {
namespace Internal {

    // Custom implement of std::array
    template <typename T, size_t Nm>
    class FixedArray {
    private:
        T _data[Nm];
        size_t _len = 0;

    public:
        FixedArray() { }
        ~FixedArray() { }
        FixedArray(FixedArray& fa)
            : _len(fa.len)
        {
            for (size_t i = 0; i < Nm; i++)
                this->_data[i] = fa._data[i];
        }

        T* begin() { return _data; }
        T* end() { return _data + _len; }

        T& operator[](size_t pos) { return _data[pos]; }
        T& at(size_t pos) { return this->_data[pos]; }

        bool push_back(T data) { return (this->_len <= Nm ? this->_data[this->_len] = data, ++this->_len, true : false); }
        bool set(size_t pos, T data) { return (pos <= Nm ? this->_data[pos] = data, true : false); }
        T get(size_t pos) { return (pos <= Nm ? this->_data[pos] : T {}); }
        void clear(size_t pos = Nm)
        {
            T tmp;
            if (pos != Nm) {
                this->_data[pos] = tmp;
            } else {
                for (size_t i=0;i<Nm;++i)
                    this->_data[i] = tmp;
            }
        }
    };

} // namespace Internal
} // namespace LRVM

#endif