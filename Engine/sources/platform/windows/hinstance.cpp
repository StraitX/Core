#include "platform/windows/hinstance.hpp"

namespace StraitX {
namespace Windows {

static HInstance instance;

HInstance::HInstance():
	mInstanceHandle(0)
{}

Result HInstance::Initialize() {
	mInstanceHandle = (HINSTANCE)GetModuleHandle(nullptr);
	// for now;
	return Result::Success;
}


HINSTANCE& HInstance::Handle() {
	return mInstanceHandle;
}

HInstance& HInstance::Instance() {
	return instance;
}

};// namespace Windows::
};// namespace StraitX::