
#ifndef __COMPONENT_MASK_HPP__
#define __COMPONENT_MASK_HPP__

#include <components/Component.hpp>
#include <type_traits>

struct ComponentMask {
private:
	unsigned int mask;
public:

	ComponentMask();

	template<typename ComponentType>
	void addComponent();

	template<typename ComponentType>
	void removeComponent();

	unsigned int getMask() const;

	bool operator==(ComponentMask const &);

	void clear();

	template <typename C>
	static int getComponentFamily();
};

#endif