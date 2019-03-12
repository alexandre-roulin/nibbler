#pragma once

#include <exception>
#include <string>
#include <dlfcn.h>
#include <functional>
#include <vector>

template < typename T >
struct LibraryInfo {
	std::string title;
	std::string path;
	T kLibrary;

	LibraryInfo() = default;
	LibraryInfo(std::string const &&ctitle, std::string const &&cpath, T ckLibrary) :
	title(ctitle),
	path(cpath),
	kLibrary(ckLibrary) {}
};

template < typename T, typename E, typename SIGNATURE = T *(*)()>
class ADynamicLinkLibrary {
public:

	class Error : public std::runtime_error {
	public:
		Error() noexcept;
		Error(std::string const &s) noexcept;
		const char *what() const noexcept override;
		~Error() noexcept = default;
		Error(Error const &src) noexcept;

		Error &operator=(Error const &rhs) noexcept = delete;
	private:
		std::string error_;
	};

	ADynamicLinkLibrary(E typeInstance);
	virtual ~ADynamicLinkLibrary();
	ADynamicLinkLibrary &operator=(const ADynamicLinkLibrary &) = delete;
	ADynamicLinkLibrary(const ADynamicLinkLibrary &) = delete;

	template< typename ...Args >
	void loadDynamicLibrary(Args... args) {
		auto lib = std::find_if(libraryInfo.begin(), libraryInfo.end(),
								[this](LibraryInfo< E > const &info){
									return info.kLibrary == kNextInstance_;
								});

		if (lib == libraryInfo.end())
			throw(std::range_error("DynamicLinkLibrary:: Library info is not found"));

		unloadDynamicLibrary();

		if (!(dlHandle_ = dlopen(lib->path.c_str(), RTLD_LAZY | RTLD_LOCAL))) {
			dlError();
			return;
		}
		if (!(newInstance_ = reinterpret_cast< SIGNATURE >(dlsym(dlHandle_, "newInstance")))) {
			dlError();
			return;
		}
		if (!(deleteInstance_ = reinterpret_cast<
				void (*)(T *)
				>(dlsym(dlHandle_, "deleteInstance")))) {
			dlError();
			return;
		}
		currentLibraryInfo_ = lib;
		instance_ = newInstance_(args...);
		kInstance_ = kNextInstance_;
	}
	void setNextKInstance();
	void unloadDynamicLibrary();
	void dlError();
	bool hasLibraryLoaded() const;

	T *getInstance() const;
	void setNextKInstance(E kinstance);
	LibraryInfo< E > const &getCurrentLibraryInfo() const;
	LibraryInfo< E > const &getNextLibraryInfo() const;

	static std::vector< LibraryInfo< E > > libraryInfo;

protected:
	T *instance_;
	E kInstance_;
	E kNextInstance_;
	typename std::vector< LibraryInfo< E > >::iterator nextLibraryInfo_;
	typename std::vector< LibraryInfo< E > >::iterator currentLibraryInfo_;
	SIGNATURE newInstance_;

private:

	void *dlHandle_;
	void (*deleteInstance_)(T *);
};

template < typename T, typename E, typename SIGNATURE >
ADynamicLinkLibrary< T, E, SIGNATURE >::ADynamicLinkLibrary(E typeInstance):
		instance_(nullptr),
		kInstance_(typeInstance),
		currentLibraryInfo_(ADynamicLinkLibrary< T, E >::libraryInfo.end()),
		newInstance_(nullptr),
		dlHandle_(nullptr),
		deleteInstance_(nullptr) {
}

template < typename T, typename E, typename SIGNATURE >
ADynamicLinkLibrary< T, E, SIGNATURE >::~ADynamicLinkLibrary() {
	unloadDynamicLibrary();
}

template < typename T, typename E, typename SIGNATURE >
void ADynamicLinkLibrary< T, E, SIGNATURE >::dlError() {
	std::string error(dlerror());
	unloadDynamicLibrary();
	throw(ADynamicLinkLibrary< T, E, SIGNATURE >::Error(error));
}

template < typename T, typename E, typename SIGNATURE >
void ADynamicLinkLibrary< T, E, SIGNATURE >::unloadDynamicLibrary() {
	if (deleteInstance_ && instance_)
		deleteInstance_(instance_);
	deleteInstance_ = nullptr;
	newInstance_ = nullptr;
	instance_ = nullptr;
	if (dlHandle_)
		dlclose(dlHandle_);
	dlHandle_ = nullptr;
}

template < typename T, typename E, typename SIGNATURE >
bool ADynamicLinkLibrary< T, E, SIGNATURE >::hasLibraryLoaded() const {
	return instance_ != nullptr;
}

template < typename T, typename E, typename SIGNATURE >
T *ADynamicLinkLibrary< T, E, SIGNATURE >::getInstance() const {
	return instance_;
}

template < typename T, typename E, typename SIGNATURE >
void ADynamicLinkLibrary< T, E, SIGNATURE >::setNextKInstance() {
	typename std::vector< LibraryInfo< E > >::iterator nextLibraryInfo = currentLibraryInfo_;
	std::advance(nextLibraryInfo, 1);

	if (nextLibraryInfo == ADynamicLinkLibrary< T, E, SIGNATURE >::libraryInfo.end()) {
		nextLibraryInfo = ADynamicLinkLibrary< T, E, SIGNATURE >::libraryInfo.begin();
	}
	setNextKInstance(nextLibraryInfo->kLibrary);
}
template < typename T, typename E, typename SIGNATURE >
void ADynamicLinkLibrary< T, E, SIGNATURE >::setNextKInstance(E kinstance) {
	kNextInstance_= kinstance;
	nextLibraryInfo_ = std::find_if(libraryInfo.begin(), libraryInfo.end(),
									[this](LibraryInfo< E > const &info){
										return info.kLibrary == kNextInstance_;
									});
}
template < typename T, typename E, typename SIGNATURE >
LibraryInfo< E > const &ADynamicLinkLibrary< T, E, SIGNATURE >::getCurrentLibraryInfo() const {
	return *currentLibraryInfo_;
}
template < typename T, typename E, typename SIGNATURE >
LibraryInfo< E > const &ADynamicLinkLibrary< T, E, SIGNATURE >::getNextLibraryInfo() const {
	return *nextLibraryInfo_;
}

template < typename T, typename E, typename SIGNATURE >
std::vector< LibraryInfo< E > > ADynamicLinkLibrary< T, E, SIGNATURE >::libraryInfo(0);
template < typename T, typename E, typename SIGNATURE >
ADynamicLinkLibrary< T, E, SIGNATURE >::Error::Error() noexcept {}
template < typename T, typename E, typename SIGNATURE >
ADynamicLinkLibrary< T, E, SIGNATURE >::Error::Error(std::string const &s) noexcept : std::runtime_error(s), error_(s) {}
template < typename T, typename E, typename SIGNATURE >
ADynamicLinkLibrary< T, E, SIGNATURE >::Error::Error(ADynamicLinkLibrary< T, E, SIGNATURE >::Error const &src) noexcept : std::runtime_error(error_), error_(src.error_) { error_ = src.error_; }
template < typename T, typename E, typename SIGNATURE >
const char * ADynamicLinkLibrary< T, E, SIGNATURE >::Error::what() const noexcept { return (error_.c_str()); }