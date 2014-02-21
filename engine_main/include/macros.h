#ifndef __MACROS_H__
#define __MACROS_H__

#define SE_TOSTR2(x) #x
#define SE_TOSTR(x) SE_TOSTR2(x) 

#define SAFE_DEL(x)					if (x) { delete x; x = 0; }
#define SAFE_DEL_ARRAY(x)			if (x) { delete[] x; x = 0; }


#endif