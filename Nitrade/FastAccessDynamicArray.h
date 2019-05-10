#pragma once

namespace Utils {

	//interface for mocking
	template<class T>
	class IFastAccessDynamicArray
	{
	public:
		IFastAccessDynamicArray() {}
		virtual ~IFastAccessDynamicArray() {}

		virtual void add(T value) = 0;
		virtual T& get(int pos) = 0;
	};


	template<class T>
	class FastAccessDynamicArray :
		public IFastAccessDynamicArray<T>
	{
	private:
		int _pos{ 0 };
		int _size{ 0 };
		T* _values{ nullptr };

	public:

		FastAccessDynamicArray() 
		{
		}

		virtual ~FastAccessDynamicArray() {
			delete[] _values;
		}

		int size() {
			return _size;
		}
		void add(T value)
		{
			//create a temp array one size larger
			T* temp = new T[_size + 1];

			//copy the pointers across
			for (int i = 0; i < _size; i++)
				temp[i] = _values[i];

			_values = temp;

			//add in the new item and increment the counter
			_values[_size] = value;
			_size++;


		}

		T& get(int pos)
		{
			//don't check pos is in range - we want fast access, caller is responsible for sending correct pos						
			return _values[pos];
		}

		T operator [](int pos) { return _values[pos]; }


	};


}

