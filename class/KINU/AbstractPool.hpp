#pragma once

namespace KINU {

	class AbstractPool {
	public:
		virtual void clean() = 0;
		virtual ~AbstractPool() = default;
	};
}