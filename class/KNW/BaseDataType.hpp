#pragma once

#include <cstdint>
#include <bitset>
#include "Config.hpp"

namespace KNW {
	struct BaseDataType {
	public:
		using Header = uint16_t;
	protected:
		static Header nextHeader;
	};

	template <typename T>
	struct DataType : BaseDataType {
		static Header getHeader();
	};

	template<typename T>
	BaseDataType::Header DataType<T>::getHeader() {
		static Header id = nextHeader++;
		return id;
	}

}