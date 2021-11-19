

#include "hfilestr.h"
#include <sstream>

namespace HUICPP {

HFileStr::HFileStr() 
    : m_stream () {

}


HFileStr::HFileStr(HCSTRR strFileName, std::ios_base::openmode mode) 
    : m_stream(strFileName, mode) {

}


HFileStr::~ HFileStr() {
    
    Close();

}


HRET HFileStr::InOpen(HCSTRR strFileName) {

    return Open(strFileName, std::ios_base::in);

}


HRET HFileStr::OutOpen(HCSTRR strFileName) {

    return Open(strFileName, std::ios_base::out);

}


HRET HFileStr::Open(HCSTRR strFileName, std::ios_base::openmode mode) {
    
    m_stream.open(strFileName, mode);

    HRETURN_OK;

}


void HFileStr::Close() {

    if (m_stream.is_open()) {
        m_stream.close();
    }

}


HRET HFileStr::ReadToString (HSTRR str) {

    HASSERT_RETURN(m_stream.is_open(), ERR_STATUS);

    std::stringstream ss;
    ss << m_stream.rdbuf();

    str = ss.str();

    HRETURN_OK;

}


HRET HFileStr::WriteFromString(HCSTRR str) {

    HASSERT_RETURN(m_stream.is_open(), ERR_STATUS);

    m_stream << str;

    HRETURN_OK;

}


}