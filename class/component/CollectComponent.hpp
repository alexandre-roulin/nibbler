#pragma once


struct CollectComponent {
	explicit CollectComponent(bool food = true);
	bool food;
	CollectComponent &operator=(CollectComponent const &);
};


