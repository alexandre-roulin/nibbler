#ifndef NETWORK_DATATCP_HPP
#define NETWORK_DATATCP_HPP

#include "BaseDataType.hpp"
#include <functional>
#include <vector>
#include <network/Data.hpp>
#include <boost/enable_shared_from_this.hpp>
namespace KNW {

	class DataTCP : public boost::enable_shared_from_this<KNW::DataTCP> {

	public:
		using Header = BaseDataType::Header;
		using boost_shared_ptr = boost::shared_ptr<DataTCP>;
		using boost_weak_ptr = boost::weak_ptr<DataTCP>;
		static boost_shared_ptr create();
		DataTCP(DataTCP const &) = delete;

		template<typename T>
		void addDataType(std::function<void(T)> callback);

		template<typename T, typename H>
		void addDataType(std::function<void(T)> callback, H header);

		template<typename T>
		bool hasType() const;

		void sendDataToCallback(Header header, void *data);

		size_t getSizeOfHeader(Header header);

		~DataTCP();

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

			void operator()(void *pVoid) override {
				T data;
				std::memcpy(static_cast<void *>(&data), pVoid, sizeof(T));
				return function_(data);
			}

			std::function<void(T)> function_;
		};

		friend class ServerTCP;

		friend class ConnectionTCP;

		friend class ClientTCP;

		bool setHeader_;
		std::vector<size_t> sizeType;
		std::vector<std::shared_ptr<AbstractCallback>> callbackType;
	};


	template<typename T>
	void DataTCP::addDataType(std::function<void(T)> callback) {
		auto header = DataType<T>::getHeader();
		assert(!hasType<T>());
		if (header >= sizeType.size() && header >= callbackType.size()) {
			sizeType.resize(header + 1);
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
			sizeType.resize(header_ + 1);
			callbackType.resize(header_ + 1);
		}
		sizeType[header_] = sizeof(T);
		if (callback)
			callbackType[header_] = std::make_shared<CallbackType<T>>(callback);
	}


	template<typename T>
	bool DataTCP::hasType() const {
		auto header = DataType<T>::getHeader();
		return header < sizeType.size() && header < callbackType.size();
	}

}
#endif //NETWORK_DATATCP_HPP
