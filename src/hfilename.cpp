

#include "hfilename.h"

namespace HUICPP {

void HDirection::GoToParent () noexcept {

	HSTR src_name = GetPath();
	IF_NOTOK(get_parent_path(src_name)) {
		return ;
	}
    SetPath(src_name);

}


HDirection HDirection::GetParent () const noexcept {

	HDirection res;
	path_str_type str;
	IF_OK(get_parent_path(str)) {
		res.SetPath(str);
	}

	return res;
}


HRET HDirection::CreateDic() const noexcept {

	return create_directory();

}


HFileName HDirection::GetDicFile(const path_str_type& strFileName) const noexcept {

	return HFileName(append(GetPath(), strFileName));

}


HDirection HFileName::GetParentDic () const noexcept {

	HDirection res;
	path_str_type str;
	IF_OK(get_parent_path(str)) {
		res.SetPath(str);
	}

	return res;

}


HFileName::path_str_type HFileName::GetExtenName () const noexcept {

	return get_file_extern_name();

}


HFileName::path_str_type HFileName::JustFileName () const noexcept {

	return path_only_filename();

}


}
