/**********************************************************
 * Created: Joseph Mar-26-2021
 * Description: 
 *      Define some constant types.
 * 
 **********************************************************/

#ifndef __H_CONSTANT_IN_H__
#define __H_CONSTANT_IN_H__

#include "basictypes_in.h"
#include "commonmacro_in.h"

namespace HUICPP {

    // define common constant
    STATIC_CONST_VAL(HN) HWORD_LEN = 128;
    typedef HCH HWORD_BUF[HWORD_LEN];

#define DECLARE_LEN_BUF_BASE_WORD(nn,cc) STATIC_CONST_VAL(HN) H##nn##_LEN = HWORD_LEN * cc; \
    typedef HCH H##nn##_BUF[H##nn##_LEN]

    DECLARE_LEN_BUF_BASE_WORD(NAME, 2);

    DECLARE_LEN_BUF_BASE_WORD(PATH, 8);

    DECLARE_LEN_BUF_BASE_WORD(STRING, 16);

#undef DECLARE_LEN_BUF_BASE_WORD

    STATIC_CONST_VAL(SYS_T) INVALID_FD = -1;

#define IS_VALID_FD(fd) (fd > 0)

#ifndef CLOSE_FD
#define __CLOSE_FD(fd) do {\
	if(IS_VALID_FD(fd)) {\
	    ::close((fd)); \
	    (fd) = INVALID_FD; \
	}}while(0)

#define CLOSE_FD(fd) __CLOSE_FD(fd)
#endif //HCLOSE_FD

}


#endif //__H__HUICPP_CONSTANT_H__
