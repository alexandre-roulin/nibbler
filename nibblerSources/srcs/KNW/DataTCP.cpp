#include "DataTCP.hpp"

/** DataTCP **/

namespace KNW {

	DataTCP::b_sptr DataTCP::create() {
		return DataTCP::b_sptr(new DataTCP);
	}

	int DataTCP::getSizeOfHeader(DataTCP::Header header) {
		return (header < sizeType.size() ? sizeType[header] : 0);
	}

	void DataTCP::sendDataToCallback(DataTCP::Header header, void *data) {
		if (header < callbackType.size() && callbackType[header])
			(*callbackType[header])(data);
	}
}