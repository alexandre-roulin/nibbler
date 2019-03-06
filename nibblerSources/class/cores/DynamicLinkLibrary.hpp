#pragma once

#include <exception>
#include <IDisplay.hpp>
#include <array>
#include "nibbler.hpp"

template < typename T, typename E >
class DynamicLinkLibrary {
public:

	class Error : public std::runtime_error {
	public:
		Error(void) noexcept;
		Error(std::string const &s) noexcept;
		const char *what() const noexcept override;
		~Error(void) noexcept = default;
		Error(Error const &src) noexcept;

		Error &operator=(Error const &rhs) noexcept = delete;
	private:
		std::string error_;
	};

	DynamicLinkLibrary();
	virtual ~DynamicLinkLibrary();
	DynamicLinkLibrary &operator=(const DynamicLinkLibrary &) = delete;
	DynamicLinkLibrary(const DynamicLinkLibrary &) = delete;

	void switchNextLibrary();
	void loadDynamicLibrary(E typeInstance);
	void constructDynamicLibrary(int width, int height);
	void unloadDynamicLibrary();
	void dlError();
	bool hasLibraryLoaded() const;
	bool hasConstructorLoaded() const;
	T *getDisplay() const;
	E getKDisplay() const;
	LibraryInfo< E > const &getCurrentLibraryInfo() const;

	static std::vector< LibraryInfo< E > > libraryInfo;

protected:
private:
	typedef typename std::vector< LibraryInfo< E > >::iterator iterator_;

	void *dlHandle_;
	T *(*newInstance_)(int, int, char const *);
	T *instance_;
	void (*deleteInstance_)(IDisplay *);
	E kInstance_;
	iterator_ currentLibraryInfo_;
};

template < typename T, typename E >
DynamicLinkLibrary< T, E >::DynamicLinkLibrary():
		dlHandle_(nullptr),
		newInstance_(nullptr),
		instance_(nullptr),
		deleteInstance_(nullptr),
		kInstance_(kDisplaySfmlLibrary),
		currentLibraryInfo_(DynamicLinkLibrary< T, E >::libraryInfo.end()) {
}

template < typename T, typename E >
DynamicLinkLibrary< T, E >::~DynamicLinkLibrary() {
	unloadDynamicLibrary();
}

template < typename T, typename E >
void DynamicLinkLibrary< T, E >::dlError() {
	unloadDynamicLibrary();
	throw (DynamicLinkLibrary< T, E>::Error(dlerror()));
}

template < typename T, typename E >
void DynamicLinkLibrary< T, E >::loadDynamicLibrary(E typeInstance) {
	auto lib = std::find_if(libraryInfo.begin(), libraryInfo.end(),
			[typeInstance](LibraryInfo< E > const &info){
				return info.kLibrary == typeInstance;
			});

	if (lib == libraryInfo.end())
		throw(std::range_error("DynamicLinkLibrary:: Library info is not found"));

	if (!(dlHandle_ = dlopen(lib.path, RTLD_LAZY | RTLD_LOCAL))) {
		dlError();
		return;
	}
	if (!(newInstance_ = reinterpret_cast<
			IDisplay *(*)(int, int, const char *)
			>(dlsym(dlHandle_, "newInstance_")))) {
		dlError();
		return;
	}
	if (!(deleteInstance_ = reinterpret_cast<
			void (*)(IDisplay *)
			>(dlsym(dlHandle_, "deleteInstance")))) {
		dlError();
		return;
	}
	kInstance_ = typeInstance;
	currentLibraryInfo_ = lib;
}

template < typename T, typename E >
void DynamicLinkLibrary< T, E >::unloadDynamicLibrary() {
	if (deleteInstance_ && instance_)
		deleteInstance_(instance_);
	deleteInstance_ = nullptr;
	newInstance_ = nullptr;
	instance_ = nullptr;
	if (dlHandle_)
		dlclose(dlHandle_);
	dlHandle_ = nullptr;
}

template < typename T, typename E >
void DynamicLinkLibrary< T, E >::switchNextLibrary() {
	unloadDynamicLibrary();
	++currentLibraryInfo_;
	if (currentLibraryInfo_ == DynamicLinkLibrary< T, E>::libraryInfo.end())
		DynamicLinkLibrary< T, E>::libraryInfo.front();
	loadDynamicLibrary(kInstance_);
}

template < typename T, typename E >
void DynamicLinkLibrary< T, E >::constructDynamicLibrary(int width, int height) {
	assert(newInstance_ != nullptr);
	instance_ = newInstance_(width, height, currentLibraryInfo_->title);
}

template < typename T, typename E >
bool DynamicLinkLibrary< T, E >::hasConstructorLoaded() const {
	return	newInstance_ != nullptr && dlHandle_ != nullptr && deleteInstance_ != nullptr;
}
template < typename T, typename E >
bool DynamicLinkLibrary< T, E >::hasLibraryLoaded() const {
	return instance_ != nullptr;
}

template < typename T, typename E >
T *DynamicLinkLibrary< T, E >::getDisplay() const {
	return instance_;
}
template < typename T, typename E >
E DynamicLinkLibrary< T, E >::getKDisplay() const {
	return kInstance_;
}
template < typename T, typename E >
LibraryInfo< E > const &DynamicLinkLibrary< T, E >::getCurrentLibraryInfo() const {
	return *currentLibraryInfo_;
}

template < typename T, typename E >
std::vector< LibraryInfo< E > > DynamicLinkLibrary< T, E >::libraryInfo(0);
template < typename T, typename E >
DynamicLinkLibrary< T, E >::Error::Error() noexcept {}
template < typename T, typename E >
DynamicLinkLibrary< T, E >::Error::Error(std::string const &s) noexcept : std::runtime_error(s), error_(s) {}
template < typename T, typename E >
DynamicLinkLibrary< T, E >::Error::Error(DynamicLinkLibrary< T, E >::Error const &src) noexcept : std::runtime_error(error_), error_(src.error_) { error_ = src.error_; }
template < typename T, typename E >
const char * DynamicLinkLibrary< T, E >::Error::what() const noexcept { return (error_.c_str()); }