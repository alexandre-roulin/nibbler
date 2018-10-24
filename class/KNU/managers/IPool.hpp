#pragma once

namespace KNU {
	class AbstractPool {
	public:
		virtual ~AbstractPool() {}

		virtual void clear() = 0;
	};

}