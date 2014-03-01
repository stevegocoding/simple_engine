#ifndef __UTILS_H__
#define __UTILS_H__

#include <map>
#include "prerequisites.h"
#include "boost/any.hpp"
#include "boost/shared_ptr.hpp"

typedef std::map<std::string, boost::any> NameValueMap; 

////////////////////////////////////////////////////////////////////////// 

class NonCopyable {
protected:
	NonCopyable() {}
	~NonCopyable() {}
private:
	NonCopyable(const NonCopyable&);
	const NonCopyable& operator=(const NonCopyable&);
};

class SingletonDeleteTrick : private NonCopyable
{
public:	
	SingletonDeleteTrick() : mIsDeleted(false){}
	virtual ~SingletonDeleteTrick(){ mIsDeleted = true; }
protected:
	// Trick to find out if a static instance has been destroyed (at least it
	// means the destructor was called). 
	//
	// This should prevent using a singleton after it's destructor was called
	// even if the static memory instance is still valid (object destroyed, 
	// but not freed)
	bool	mIsDeleted;
};

template < typename T >
class Singleton : public SingletonDeleteTrick
{
public:	
	static T* GetInstance() {
		static T inst;
		return (inst.mIsDeleted) ? 0 : &inst;
	}
};



#endif


