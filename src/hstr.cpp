
#include "hstr.h"
#include <cstdarg>
#include <algorithm>


namespace HUICPP {

HStr HStr::Ltrim(HCSZ csz) {
	HIGNORE_RETURN(Ltrim(*this, csz));
	return *this;
}

HStr& HStr::Rtrim (HCSZ csz) {
	HIGNORE_RETURN(Rtrim(*this, csz));
	return *this;
}

HStr& HStr::Trim(HCSZ csz) {
	HIGNORE_RETURN(Trim(*this, csz));
	return *this;
}

HStr& HStr::Upper() {
	HIGNORE_RETURN(Upper(*this));
	return *this;
}

HStr& HStr::Lower() {
	HIGNORE_RETURN(Lower(*this));
	return *this;
}


HStr& HStr::RemoveRight(HSIZE len) {
	RemoveRight(*this, len);
	return *this;
}


HStr& HStr::RemoveLeft(HSIZE len) {
	RemoveLeft(*this, len);
	return *this;
}


HStr& HStr::Replace(HCSTRR srckey, HCSTRR dstKey) {
	HIGNORE_RETURN(Replace(*this, srckey, dstKey));
	return *this;
}

HStr& HStr::Replaces(HCSTRR srcKey, HCSTRR dstKey) {
	HIGNORE_RETURN(Replaces(*this, srcKey, dstKey));
	return *this;
}

HStr& HStr::Remove(HCSTRR srcKey) {
	HIGNORE_RETURN(Remove(*this, srcKey));
	return *this;
}

HStr& HStr::Removes(HCSTRR srcKey) {
	HIGNORE_RETURN(Removes(*this, srcKey));
	return *this;
}

HStr& HStr::AppendFormat(const value_type *fmt, ...) {

	HStr newstr;
	va_list ap;
	va_start(ap, fmt);
	newstr = vFormat(fmt, ap);
	va_end(ap);
	append(newstr);
	return *this;

}
       
HRET HStr::Split(HCSTRR str, HCSTRR key, HVSTRR res) {

	HASSERT_RETURN(not str.empty() and not key.empty(), INVL_PARA);

	HSIZE beg = 0, pos = 0, len = key.length();
	while ((pos = str.find(key, beg)) != HSTR::npos) {
		res.push_back(str.substr(beg, pos - beg));
		beg = pos + len;
	}

	if(pos != str.length() - 1){
		res.push_back(str.substr(beg));
	}

	HRETURN_OK;

}    

RET_T HStr::Ltrim(HSTRR str, HCSZ sz) {

	HASSERT_RETURN(not str.empty(), INVL_PARA);

	HSTR not_find_str(s_tr_str);

	if (nullptr != sz) {
		not_find_str.append(sz);
	}

	size_type pos = str.find_first_not_of(not_find_str);
	if (pos == HSTR::npos) {

		str.clear();

	} else {

		str = str.substr(pos);

	}

	HRETURN_OK;

}

RET_T HStr::Rtrim(HSTRR str, HCSZ sz) {

	HASSERT_RETURN(not str.empty(), INVL_PARA);

	HSTR not_find_str(s_tr_str);

	if (nullptr != sz) {
	    not_find_str.append(sz);
	}

	str = str.substr(0, str.find_last_not_of(not_find_str) + 1);

	HRETURN_OK;

}

RET_T HStr::Trim(HSTRR str, HCSZ sz ) {
	    
	HNOTOK_RETURN(Ltrim(str, sz));
	HNOTOK_RETURN(Rtrim(str, sz));

	HRETURN_OK;

}

RET_T HStr::Upper (HSTRR str) {

	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	HRETURN_OK;

}

RET_T HStr::Lower(HSTRR str) {

	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	HRETURN_OK;

}

bool HStr::IsAllDigit(HCSTRR str) noexcept{

	return str.find_first_not_of("1234567890") == HSTR::npos;
		
}

bool HStr::IsAllDigitAndDos (HCSTRR str) noexcept{

	return str.find_first_not_of("1234567890.") == HSTR::npos;

}

bool HStr::IsAllDigitAndChar (HCSTRR str) noexcept{

	HCSTR cstr = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	return str.find_first_not_of(cstr) == HSTR::npos;

}

void HStr::RemoveRight(HSTRR str, HSIZE len) {

	HSIZE size = str.length();
	if (len >= size) {

	    str.clear();
		
	} else {

	    str = str.substr(0, size - len);

	}

}


void HStr::RemoveLeft(HSTRR str, HSIZE len) {

	HSIZE size = str.size();
	if (len > size) {
		str.clear();
	} else {
		str = str.substr(len);
	}
	
}


RET_T HStr::Replace (HSTRR src, HCSTRR srcKey, HCSTRR dstKey) {

	HASSERT_RETURN(not src.empty() and not srcKey.empty() and not dstKey.empty(), 
		INVL_PARA);

	// get the position of the first match
	size_type pos = src.find(srcKey);
	HASSERT_RETURN(pos != npos, OVER_FLOW);

	size_type len = srcKey.length();

	src.replace(pos, len, dstKey);

	HRETURN_OK;

}

RET_T HStr::Replaces (HSTRR src, HCSTRR srcKey, HCSTRR dstkey) {

	while (IS_OK(Replace(src, srcKey, dstkey))) ;

	HRETURN_OK;
}

bool HStr::IsIn(HCSTRR src, HCSTRR key) noexcept {

	return src.find(key) != npos;

}


bool HStr::IsBeginWith (HCSTRR src, HCSTRR key) noexcept {

	return src.find(key) == 0;

}


bool HStr::IsEndWith (HCSTRR src, HCSTRR key) noexcept {

	return src.find(key) == src.length() - key.length() - 1;

}


HSIZE HStr::Counter (HCSTRR src, HCSTRR key) noexcept {

	HSIZE res = 0;
	HSIZE pos = 0;

	while ((pos = src.find(key, pos)) != npos and pos < src.length()) {

		++ res;
		++ pos;

	}

	return res;

}

RET_T HStr::Remove(HSTRR src, HCSTRR strKey) {

	HASSERT_RETURN(not src.empty() and not strKey.empty(), 
		INVL_PARA);

	size_type pos = src.find(strKey);
	HASSERT_RETURN(pos != npos, OVER_FLOW);

	HSIZE len = strKey.length();

	src.erase(pos, len);

	HRETURN_OK;
}

RET_T HStr::Removes(HSTRR src, HCSTRR strKey) {

	RET_T cb = ERR_NO(OK);

	while(IS_OK(Remove(src, strKey))) ;
	
	HRETURN_OK;
}

HStr HStr::V2Str (HCVSTRR strs, HCSTR l) {
	
	HStr res;

	for (size_type i = 0; i < strs.size(); ++i) {

		res.append(strs[i]);
		res.append(l);

	}

	if (not l.empty()) {
		res = res.substr(0, res.length()-1);
	}

	return res;
}
    
HStr HStr::Format(const value_type* fmt, ...) {

	HStr res;
	va_list ap;
	va_start(ap, fmt);
	res = vFormat(fmt, ap);
	va_end(ap);
	return res;

}

HStr HStr::vFormat(const value_type* fmt, va_list ap) {

	HStr res;
	int cb = 0;
#ifdef _GUN_SOURCE
	char* buf = NULL;
	cb = vasprintf(&buf, fmt, ap);

	if ( cb >= 0 && buf != NULL) {
	    // vasprintf success!
	    
	    res = buf;
	    free(buf);

	}

#else

	HSTRING_BUF buf = {0};
	cb = vsprintf(buf, fmt, ap);
	if (cb >= 0) {

	    // vsprintf success
	    res = buf;

	}

#endif

	return res;

}

HStr HStr::s_tr_str = " \n\r\t";

}




