#ifndef ARRAY_HPP_
#define ARRAY_HPP_

#include <cstddef>
#include <functional>
#include <vector>

template <typename T>
class Array
{
public:
	Array(void)
	{
	}

	T &operator[](size_t i)
	{
		if(i >= _array.size())
			_array.resize(i + 1);

		return _array[i];
	}

	bool operator==(Array<T> &array)
	{
		if(_array.size() != array.size())
			return false;

		for(size_t i = 0, size = _array.size(); i < size; i++)
			if(_array[i] != array[i])
				return false;

		return true;
	}

	bool operator!=(Array<T> &array)
	{
		if(_array.size() != array.size())
			return true;

		for(size_t i = 0, size = _array.size(); i < size; i++)
			if(_array[i] != array[i])
				return true;

		return false;
	}

	Array<T> operator=(Array<T> array)
	{
		_array.resize(array.size());

		for(size_t i = 0, size = _array.size(); i < size; i++)
			_array[i] = array[i];

		return array;
	}

	size_t size(void)
	{
		return _array.size();
	}

	void resize(size_t s)
	{
		_array.resize(s);
	}

	void clear(void)
	{
		_array.clear();
	}

	bool find(T &t)
	{
		for(size_t i = 0, size = _array.size(); i < size; i++)
		{
			if(_array[i] == t)
				return true;
		}

		return false;
	}

	void for_each(std::function<void(T &, size_t)> map)
	{
		for(size_t i = 0, size = _array.size(); i < size; i++)
		{
			map(_array[i], i);
		}
	}

	void for_each(std::function<void(T &)> map)
	{
		for(size_t i = 0, size = _array.size(); i < size; i++)
		{
			map(_array[i]);
		}
	}
private:
	std::vector<T> _array;
};

#endif	// ARRAY_HPP_
