/*
 * Created: Joseph Hui Sun Sep 26 2021
 * Description: 
  		Wrapper for huicpp application.
*/


#ifndef __H_HUICPP_FILENAME_H__
#define __H_HUICPP_FILENAME_H__

#include "huicpp.h"
#include "hstr.h"

#ifndef HWIN
#include "detail/filesystem_detail.h"
#endif 

namespace HUICPP {

#ifndef HWIN 
using file_base = detail::filesystem_detail;
#endif 

class HFileNameBase : public file_base {
public:
	using base = file_base;
	using path_elem_type = base::path_elem_type;
	using path_str_type = base::path_str_type;
	using filesize_t = base::filesize_t;

public:
	enum class filename_type {
		fn_base,
		fn_dic,
		fn_file,
		fn_end,
	};

public:
	HFileNameBase () noexcept {}

	explicit HFileNameBase (const path_str_type& str) noexcept : base(str) {}

	HFileNameBase(const HFileNameBase& fn) noexcept
		: base(fn.GetPath()) {  }

	virtual ~HFileNameBase() { }

	HFileNameBase& operator=(const HFileNameBase& fn) noexcept {
		SetPath(fn.GetPath());
		return *this;
	}
	
	virtual filename_type GetType() const noexcept { return filename_type::fn_base; }

	bool Compare (const HFileNameBase& fn) const { 
		return GetPath().compare(fn.GetPath());
	}

	bool Empty () const noexcept { return GetPath().empty(); }

	bool Exists () const noexcept {
		return exists();
	}
	
	HRET Remove () const noexcept {
		return remove();
	}

};


class HFileName;
class HDirection : public HFileNameBase {
public:
	using base = HFileNameBase;

public:
	HDirection () noexcept {}

	explicit HDirection(const path_str_type& s) noexcept : base(s) {}

	HDirection (const HDirection& dic) noexcept : base(dic.GetPath()) {}

	~HDirection() { }

	HDirection& operator= (const HDirection& dic) noexcept {
		SetPath(dic.GetPath());
		return *this;
	}

	virtual filename_type GetType() const noexcept { return filename_type::fn_dic; }

	void SetAsCurrentPath () noexcept {
		setas_current();
	}

	bool IsAbsoluteDic () const noexcept  { return is_absolute(); }

	bool IsRelativeDic () const noexcept { return is_relative(); }

	HDirection& Append (const path_str_type& strName) noexcept {
		append(strName);
		return *this;
	}

	void GoToParent () noexcept;

	HDirection GetParent () const noexcept;

	HRET CreateDic() const noexcept; 

	HFileName GetDicFile(const path_str_type& strFileName) const noexcept;

};

class HFileName : public HFileNameBase {
public:
	using base = HFileNameBase;

public:
	HFileName () noexcept{}

	HFileName(const path_str_type& s) noexcept : base(s) {}
	
	HFileName (const HFileName& fn) noexcept : base(fn.GetPath()) {}
	
	~ HFileName() { }
	
	HFileName& operator= (const HFileName& fn) noexcept {
		SetPath(fn.GetPath());
		return *this;
	}

	virtual filename_type GetType() const noexcept { return filename_type::fn_file; }

	HDirection GetParentDic () const noexcept;

	path_str_type GetExtenName () const noexcept;
    
    path_str_type JustFileName () const noexcept;


};


}


#endif //__H_HUICPP_FILENAME_H__

