#ifndef ARRAY_HPP_
#define ARRAY_HPP_

#include <cstddef>
#include <functional>
#include <vector>

template <typename T>
class Array : public std::vector<T>
{
public:
        T &operator[](size_t i)
        {
                if(i >= this->size())
                        this->resize(i + 1);

                return *(this->begin() + i);
        }

        bool operator==(Array<T> &array)
        {
                if(this->size() != array.size())
                        return false;

                for(size_t i = 0, size = this->size(); i < size; i++)
                        if((*this)[i] != array[i])
                                return false;

                return true;
        }
        bool operator!=(Array<T> &array)
        {
                if(this->size() != array.size())
                        return true;

                for(size_t i = 0, size = this->size(); i < size; i++)
                        if((*this)[i] != array[i])
                                return true;

                return false;
        }

        bool find(T &t)
        {
                for(size_t i = 0, size = this->size(); i < size; i++)
                {
                        if((*this)[i] == t)
                                return true;
                }

                return false;
        }

        void for_each(std::function<void(T &, size_t)> map)
        {
                for(size_t i = 0, size = this->size(); i < size; i++)
                {
                        map((*this)[i], i);
                }
        }

        void for_each(std::function<void(T &)> map)
        {
                for(size_t i = 0, size = this->size(); i < size; i++)
                {
                        map((*this)[i]);
                }
        }
};

#endif        // ARRAY_HPP_

