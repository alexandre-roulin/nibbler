#pragma once

#include <vector>
#include <cassert>
#include "AbstractPool.hpp"
#include "Pool.hpp"
#include <list>
#include <queue>
#include <map>
#include <logger.h>

#define BASE_SIZE_EVENT 1
#define BASE_SIZE_COMPONENT 20

#define INDEX_ERR 0

namespace KINU {

	template<typename T>
	class Pool : public AbstractPool {
	private:
		std::queue<unsigned int> freeIndex_;
		std::vector<T> data_;
		std::map<unsigned int, unsigned int> hideIndexMap_;
		unsigned int size_;
		unsigned int capacity;
		unsigned int const BASE_SIZE;

		void manageVector() {
			if (size_ >= capacity - 1) {
				capacity += BASE_SIZE;
				resize(capacity);
			}
		}

		void resize(unsigned int size) {
			data_.resize(size);
		}

	public:

		Pool(unsigned int baseSize = BASE_SIZE_COMPONENT)
				: size_(0),
				  capacity(baseSize),
				  BASE_SIZE(baseSize){
			resize(capacity);
		}

		void invalidate(unsigned int index) {
			hideIndexMap_[index] = INDEX_ERR;
		}

		unsigned int size() {
			return data_.size();
		}

		bool isValid(unsigned int index) {
			return hideIndexMap_[index] != INDEX_ERR;
		}

		virtual ~Pool() {}

		bool isEmpty() const {
			return data_.empty();
		}

		virtual void clean() {
			data_.clear();
		}

		void clear() {
			data_.clear();
		}

		void add(T object) {
			data_.push_back(object);
		}

		bool set(unsigned int index, T object) {
			unsigned int indexHide;

			if (!freeIndex_.empty()) {
				indexHide = freeIndex_.front();
				freeIndex_.pop();
			} else {
				manageVector();
				indexHide = size_++;
			}
			hideIndexMap_[index] = indexHide;
			assert(indexHide < capacity);
			data_[indexHide] = object;
//			log_success("Pool[%s] in [%d] with key [%d] size[%d]", typeid(T).name(), index, indexHide, size());
			return true;
		}

		T &get(unsigned int index) {
			log_warn("Pool[%s] in [%d] with key [%d] size[%d]", typeid(T).name(), index, hideIndexMap_[index], size());
			return static_cast<T &>(data_[hideIndexMap_[index]]);
		}

		T &operator[](unsigned int index) {
			return data_[hideIndexMap_[index]];
		}

		const T &operator[](unsigned int index) const {
			return data_[hideIndexMap_[index]];
		}

		std::vector<T> getData() {
			return data_;
		}


	};

}
