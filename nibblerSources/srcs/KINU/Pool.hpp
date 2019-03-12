#pragma once

#include <vector>
#include <cassert>
#include <KINU/Config.hpp>

namespace KINU {

// Required to have a vector of pools containing different object types.
	class AbstractPool {
	public:
		AbstractPool() = default;
		virtual ~AbstractPool() = default;
		AbstractPool &operator=(const AbstractPool &) = delete;
		AbstractPool(const AbstractPool &) = delete;

		virtual void clear() = 0;
	};

// A pool is just a vector (contiguous data) of objects of type T.
	template<typename T>
	class Pool : public AbstractPool {
	public:

		Pool(int size) {
			resize(size);
		}

		Pool() = delete;
		virtual ~Pool() {}
		Pool &operator=(const Pool &) = delete;
		Pool(const Pool &) = delete;

		bool isEmpty() const {
			return data.empty();
		}

		unsigned int getSize() const {
			return data.size();
		}

		void resize(int n) {
			data.resize(n);
		}

		void clear() {
			data.clear();
		}

		bool set(unsigned int index, T object) {
			assert(index < getSize());
			data.at(index) = object;
			return true;
		}

		T &get(unsigned int index) {
			assert(index < getSize());
			return static_cast<T &>(data[index]);
		}

		void add(T object) {
			data.push_back(object);
		}

		T &operator[](unsigned int index) {
			assert(index < getSize());
			return data[index];
		}

		const T &operator[](unsigned int index) const {
			assert(index < getSize());
			return data[index];
		}

		std::vector<T> getData() {
			return data;
		}

	private:
		std::vector<T> data;
	};

}
