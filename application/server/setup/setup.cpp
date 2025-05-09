#include "setup/setup.h"

_namespace_server_begin

Setup::Setup()
	: _version(_M_server_version)
{

}


Setup::Setup(const Setup& setup)
	: Setup()
{
	operator=(setup);
}


Setup::~Setup()
{

}


bool Setup::operator==(const Setup& setup) const
{
	if (_version == setup._version) {
		return true;
	}
	else {
		return false;
	}
}


bool Setup::operator!=(const Setup& setup) const
{
	return !operator==(setup);
}


Setup& Setup::operator=(const Setup& setup)
{
	_version = setup._version;

	return *this;
}

_namespace_server_end