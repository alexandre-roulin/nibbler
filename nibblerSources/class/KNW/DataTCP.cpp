#include "DataTCP.hpp"

/** DataTCP **/

namespace KNW {

	DataTCP::DataTCP() = default;

	DataTCP::boost_shared_ptr DataTCP::create() {
		return DataTCP::boost_shared_ptr(new DataTCP);
	}

	size_t DataTCP::getSizeOfHeader(DataTCP::Header header) {
		return (header < sizeType.size() ? sizeType[header] : 0);
	}

	void DataTCP::sendDataToCallback(DataTCP::Header header, void *data) {
		if (header < callbackType.size() && callbackType[header])
			(*callbackType[header])(data);
	}

	DataTCP::~DataTCP() = default;

}