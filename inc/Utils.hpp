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
    public:
        FixedArray() { }
        ~FixedArray() { }
        FixedArray(FixedArray& fa)
        {
            for (size_t i = 0; i < Nm; i++)
                this->_data[i] = fa._data[i];
        }

        T* begin() { return _data; }
        T* end() { return _data + Nm; }

        T& operator[](size_t pos) { return _data[pos]; }
        T& at(size_t pos) { return this->_data[pos]; }
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