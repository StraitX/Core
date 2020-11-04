#include "platform/windows/hinstance.hpp"

namespace StraitX {
namespace Windows {

static HInstance instance;

HInstance::HInstance():
	mInstanceHandle(0)
{}

Error HInstance::Initialize() {
	mInstanceHandle = (HINSTANCE)GetModuleHandle(nullptr);
	// for now;
	return ErrorCode::Success;
}


HINSTANCE& HInstance::Handle() {
	return mInstanceHandle;
}

HInstance& HInstance::Instance() {
	return instance;
}

};// namespace Windows::
};// namespace StraitX::