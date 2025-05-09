#ifndef _CIRCLE_SERVER_SETUP_SETUP_H_
#define _CIRCLE_SERVER_SETUP_SETUP_H_

#include "define.h"

_namespace_server_begin

class Setup
{
public :
	Setup();
	Setup(const Setup& setup);
	virtual ~Setup();

public :
	const String& version() const;

public :
	bool operator==(const Setup& setup) const;
	bool operator!=(const Setup& setup) const;

	Setup& operator=(const Setup& setup);

private :
	String _version;
};	// class Setup

inline const String& Setup::version() const { return _version; }

_namespace_server_end

#endif	// _CIRCLE_SERVER_SETUP_SETUP_H_