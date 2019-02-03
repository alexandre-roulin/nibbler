#include "DataTCP.hpp"

/** DataTCP **/

namespace KNW {

	DataTCP::DataTCP() = default;

	size_t DataTCP::getSizeOfHeader(DataTCP::Header header) {
		return sizeType[header];
	}

	void DataTCP::sendDataToCallback(DataTCP::Header header, void *data) {
		log_success("%s Header: %d callback : %d",
				__PRETTY_FUNCTION__, header, callbackType[header].get() != nullptr);

		if ((callbackType[header]))
			(*callbackType[header])(data);
	}

	DataTCP::~DataTCP() = default;

}