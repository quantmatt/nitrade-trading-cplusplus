#pragma once

namespace Utils {
	template<class T>
	class SeriesBuffer
	{
	private:
		int _pos{ 0 };
		int _size{ 0 };
		int _filledIndex{ 0 };
		T* _values{ nullptr };

	public:
		class iterator;

	public:

		SeriesBuffer(int size): _size(size)
		{
			_values = new T[size];
		}

		virtual ~SeriesBuffer() {
			delete[] _values;
		}

		int size() {
			return _size;
		}
		void add(T value)
		{
			_values[_pos++] = value;

			//loop back to the start of the buffer
			if (_pos == _size)
				_pos = 0;

			if (_filledIndex < _size)
				_filledIndex++;
		}

		T &get(int pos)
		{
			//pos of 0 gets the current index (ie. most recent added item)
			//pos of 1 gets the second most recent added item
			//pos of 2 gets the thrid most recent added item. ect.

			//don't allow the call if the position is larger than the size
			if (pos > _size || pos < 0)
				throw std::out_of_range("Position of series buffer can't be larger than size or less than zero.");
						
			int intPos = _pos - 1 - pos;

			//the array acts as a loop so adjust the position if it goes off the end of the index
			if (intPos < 0)
				intPos = _size + intPos;

			if(intPos >= _filledIndex)
				throw std::out_of_range("Item in series buffer has not been assigned yet.");

			return _values[intPos];
		}

		iterator begin()
		{
			return iterator(0, *this);
		}

		iterator end()
		{
			return iterator(_size, *this);
		}

	};
	

	template<class T>
	class SeriesBuffer<T>::iterator {
	private:
		int _pos;
		SeriesBuffer &_seriesBuffer;
	public:
		iterator(int pos, SeriesBuffer &seriesBuffer): _seriesBuffer(seriesBuffer), _pos(pos) {

		}

		iterator& operator++(int) { //prefix
			_pos++;
			return this;
		}

		iterator& operator++() { //postfix
			_pos++;
			return *this;
		}

		T& operator*() {
			return _seriesBuffer.get(_pos);
		}

		bool operator!=(const iterator &other) const {
			return _pos != other._pos;
		}
	};

}
