

#include "hfilehandle.h"
#include <unistd.h>

namespace HUICPP {

HFileHandle::HFileHandle() 
	: m_fd(invalid_file_value) {

}


HFileHandle::HFileHandle(HCSTRR strFileName, SYS_T flags, mode_t modes) 
	: m_fd(invalid_file_value) {

	m_fd = open(strFileName.c_str(), flags, modes);

}


HFileHandle::HFileHandle(HFileHandle&& _right)
	: m_fd(_right.m_fd) {

	_right.SetAsInvalid();

}



HFileHandle::~HFileHandle() {

	Close();

}


HFileHandle& HFileHandle::operator= (HFileHandle&& _right) {

	m_fd = _right.m_fd;
	_right.SetAsInvalid();
	return *this;

}



void HFileHandle::Close() {

	if (m_fd != invalid_file_value) {
		close(m_fd);
		m_fd = invalid_file_value;
	}
	
}


}