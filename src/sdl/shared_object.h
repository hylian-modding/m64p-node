#pragma once

#include <filesystem>
#include <memory>

namespace SDL {

class SharedObject {
public:
	SharedObject();
	explicit SharedObject(const std::filesystem::path& path);

	void* Get() const;

    template <class T>
    T LoadFunction(const char* name) const
    {
        return reinterpret_cast<T>(LoadFunctionReal(name));
    }

private:
	std::unique_ptr<void, void(*)(void*)> m_ptr;

	void* LoadFunctionReal(const char* name) const;
};

}
