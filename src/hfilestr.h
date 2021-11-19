/*
 * Created: Joseph Aug-29-2021
 * Description: 
  		Wrapper for huicpp file string object. 
*/


#ifndef __H_HUICPP_FILE_STRING_H__
#define __H_HUICPP_FILE_STRING_H__

#include "huicpp.h"
#include <fstream>

namespace HUICPP {

class HFileStr {
public:
    using stream_t = std::fstream;
    using char_type = stream_t::char_type;
    using int_type = stream_t::int_type;
    using pos_type = stream_t::pos_type;
    using off_type = stream_t::off_type;

public:
    HFileStr();

    HFileStr(HCSTRR strFileName, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

    HFileStr(const HFileStr&) = delete;

    HFileStr(HFileStr&&) = delete;

    ~ HFileStr();

    HFileStr& operator=(const HFileStr&) = delete;

    HFileStr& operator=(HFileStr&&) = delete;

public:
    HRET InOpen(HCSTRR strFileName);

    HRET OutOpen(HCSTRR strFileName);

    HRET Open(HCSTRR strFileName, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

    void Close();

    HRET ReadToString (HSTRR str);

    HRET WriteFromString(HCSTRR str);

    template<class _Ty>
    RET_T operator >> (_Ty& ot) {

        m_stream >> ot;
        HRETURN_OK;

    }

    template<class _Ty>
    RET_T operator << (const _Ty& it) {

        m_stream << it;
        HRETURN_OK;

    }

private:
    stream_t m_stream;
};

}

#endif // __H_HUICPP_FILE_STRING_H__

