#ifndef NETWORK_DATATCP_HPP
#define NETWORK_DATATCP_HPP

#include "BaseDataType.hpp"
#include <functional>
#include <vector>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <cores/Snake.hpp>

namespace KNW {

	class DataTCP : public boost::enable_shared_from_this<KNW::DataTCP> {

	public:
		using Header = BaseDataType::Header;

		using b_sptr = boost::shared_ptr<DataTCP>;
		using b_wptr = boost::weak_ptr<DataTCP>;

		static b_sptr create();

		DataTCP(DataTCP const &) = delete;

		template<typename T>
		void addDataType(std::function<void(T)> callback);

		template<typename T, typename H>
		void addDataType(std::function<void(T)> callback, H header);

		template<typename T>
		bool hasType() const;

		void sendDataToCallback(Header header, void *data);

		int getSizeOfHeader(Header header);

		~DataTCP();

		template<typename T>
		std::string serializeData(BaseDataType::Header header, T data);

	private:
		DataTCP();


		class AbstractCallback {
		public:
			virtual ~AbstractCallback() = default;
			virtual void operator()(void *) = 0;
		};

		template<typename T>
		class CallbackType : public AbstractCallback {
		public:
			explicit CallbackType(std::function<void(T)> function)
					: function_(function) {}

			virtual void operator()(void *pVoid) override;

			std::function<void(T)> function_;
		};

		std::vector<int> sizeType;
		std::vector<std::shared_ptr<AbstractCallback>> callbackType;
	};

	template<typename T>
	void DataTCP::CallbackType<T>::operator()(void *pVoid) {
		T data = *reinterpret_cast<T*>(pVoid);
		return function_(data);
	}


	template<typename T>
	void DataTCP::addDataType(std::function<void(T)> callback) {
		auto header = DataType<T>::getHeader();
		assert(!hasType<T>());
		if (header >= sizeType.size() && header >= callbackType.size()) {
			sizeType.resize(header + 1, -1);
			callbackType.resize(header + 1);
		}

		sizeType[header] = sizeof(T);
		if (callback)
			callbackType[header] = std::make_shared<CallbackType<T>>(callback);

	}


	template<typename T, typename H>
	void
	DataTCP::addDataType(std::function<void(T)> callback, H header) {

		auto header_ = static_cast<BaseDataType::Header>(header);
		if (header_ >= sizeType.size() && header_ >= callbackType.size()) {
			sizeType.resize(header_ + 1, -1);
			callbackType.resize(header_ + 1);
		}
		sizeType[header_] = sizeof(T);
		if (callback)
			callbackType[header_] = std::make_shared<CallbackType<T>>(callback);
	}


	template<typename T>
	bool DataTCP::hasType() const {
		auto header = DataType<T>::getHeader();
		return header < sizeType.size() && header < callbackType.size() && sizeType[header] != -1;
	}

	template<typename T>
	std::string
	DataTCP::serializeData(BaseDataType::Header header, T data) {
		std::string buffer;
		buffer.append(reinterpret_cast<const char *>(&header),
				sizeof(BaseDataType::Header));
		buffer.append(reinterpret_cast<const char *>(&data),
				static_cast<unsigned long>(getSizeOfHeader(header)));
		return buffer;
	}


}
#endif //NETWORK_DATATCP_HPP
