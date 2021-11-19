/**********************************************************
 * Created: Joseph Mar-26-2021
 * Description: 
 *      Define some common macro tools.
 * 
 **********************************************************/


#ifndef __H_COMMON_MACRO_IN_H__
#define __H_COMMON_MACRO_IN_H__

#include <string.h>

namespace HUICPP {

#ifndef STATIC_CONST_VAL
#define __STATIC_CONST_VAL(ty) static constexpr const ty
#define STATIC_CONST_VAL(ty) __STATIC_CONST_VAL(ty)
#endif // HSTATIC_CONST_VAL


#ifndef HDELP
#define __HDELP(p) do {\
		if((p) != nullptr) {\
			delete (p); \
			(p) = nullptr;\
	}} while(0)

#define HDELP(p) __HDELP(p)  
#endif //HDELP



#ifndef HDEL_ARR
#define __HDEL_ARR(lp) do { \
	if ((lp) != nullptr) { \
	    delete [] (lp); \
	    (lp) = nullptr; \
	} \
} while(0) 

#define HDEL_ARR(lp) __HDEL_ARR(lp)
#endif ///HDEL_ARR



#ifndef HFREE
#define __HFREE(p) do { \
	if ((p) != nullptr) { \
		free((p)); \
		(p) = nullptr; \
	} \
} while(0)

#define HFREE(p) __HFREE(p)
#endif //__HFREE


#ifndef JTO_INT
#define __JTO_INT(n) static_cast<JINT>(n)

#define JTO_INT(n) __JTO_INT(n)
#endif


#ifndef ZERO_MEM
#define __ZERO_MEM(buf, len) memset((buf), 0, (len))
#define ZERO_MEM(buf, len) __ZERO_MEM(buf, len)    
#endif   

#ifndef CHECK_NEWPOINT
#define __CHECK_NEWPOINT(p) HASSERT_THROW((p)!=nullptr, MEMORY_OUT)
#define CHECK_NEWPOINT(p) __CHECK_NEWPOINT(p)
#endif //CHECK_NEWPOINT

#ifndef HRET
#define __HRET __attribute__((warn_unused_result)) RET_T
#define HRET __HRET
#endif //RET_T

#ifndef HIGNORE_RETURN
#define _HIGNORE_RETURN(ss) do{	\
	auto _ret = (ss); \
	(void) _ret;	\
} while(0) 
#define HIGNORE_RETURN(ss) _HIGNORE_RETURN(ss)
#endif //HIGNORE_RETURN


#ifndef HOLDER_GENERATE
#define _HOLDER_GENERATE(ty,dosth,undosth) class  ty ## _ ## dosth ## _ ## undosth {	\
public:	\
	explicit ty ## _ ## dosth ## _ ## undosth ( ty & __m): m_device(__m) {	\
		HIGNORE_RETURN(m_device. dosth ());	\
	}	\
	~ ty ## _ ## dosth ## _ ## undosth ()	\
    { HIGNORE_RETURN(m_device. undosth ()); }	\
    ty ## _ ## dosth ## _ ## undosth (const ty ## _ ## dosth ## _ ## undosth &) = delete;	\
    ty ## _ ## dosth ## _ ## undosth & operator=(const ty ## _ ## dosth ## _ ## undosth &) = delete; \
private:	\
	ty & m_device;	\
}
#define HOLDER_GENERATE(t,d,u) _HOLDER_GENERATE(t,d,u)
#endif //HOLDER_GENERATE


}

#endif //__H_HUICPP_COMMON_MACRO_H__

