/*
 * Created: Joseph Sep-26-2021
 * Description: 
  		Wrapper for huicpp file handle. 
*/


#ifndef __H_HUICPP_FILE_HANDLE_H__
#define __H_HUICPP_FILE_HANDLE_H__

#include "huicpp.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace HUICPP {

class HFileHandle {
public:
	using native_file_type = SYS_T;
	static constexpr native_file_type invalid_file_value = INVALID_FD;

public:
	HFileHandle();

	explicit HFileHandle(HCSTRR strFileName, SYS_T flags = O_RDWR | O_CREAT,
		mode_t modes = S_IRUSR | S_IWUSR);

	HFileHandle(const HFileHandle&) = delete;

	HFileHandle(HFileHandle&& _right);

	virtual ~HFileHandle();

	HFileHandle& operator= (const HFileHandle&) = delete;

	HFileHandle& operator= (HFileHandle&& _right);

public:
	void SetAsInvalid() { m_fd = invalid_file_value; }

	void Close();

	native_file_type GetHandle() const { return m_fd; }

private:
	native_file_type m_fd;
};

}



#endif // __H_HUICPP_FILE_HANDLE_H__

