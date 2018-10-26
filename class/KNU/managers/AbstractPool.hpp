#pragma once

namespace KNU {

	class AbstractPool {
	public:
		virtual void clean() = 0;
		virtual ~AbstractPool() = default;
	};
}