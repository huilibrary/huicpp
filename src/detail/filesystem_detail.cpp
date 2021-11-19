

#include "filesystem_detail.h"
#include "../hstr.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace HUICPP {

namespace detail {



bool filesystem_detail::exists(const path_str_type& strPathFileName) noexcept {

    struct stat _st = {0};
    return stat(strPathFileName.c_str(), &_st) == 0;

}


bool filesystem_detail::is_dir(const path_str_type& strPathFileName) noexcept {

    struct stat _st = {0};
    if (stat(strPathFileName.c_str(), &_st) != 0) {
        return false;
    }

    return _st.st_mode & S_IFMT == S_IFDIR;
}


bool filesystem_detail::is_file(const path_str_type& strPathFileName) noexcept {

    return not is_dir(strPathFileName);

}


HRET filesystem_detail::copy(const path_str_type& srcpath, 
        const path_str_type& dstpath) noexcept {

    HASSERT_RETURN(symlink(srcpath.c_str(), dstpath.c_str()) == 0, SYS_FAILED);

    HRETURN_OK;

}


HRET filesystem_detail::remove(const path_str_type& strPathFileName) noexcept {

    HASSERT_RETURN(remove(strPathFileName.c_str()) == 0, SYS_FAILED);
    HRETURN_OK;

}


HRET filesystem_detail::file_rename(const path_str_type& strPathFileName, const path_str_type& strDstPathFileName) noexcept {

    HASSERT_RETURN(::rename(strPathFileName.c_str(), strDstPathFileName.c_str()) == 0, INVL_PARA);
    HRETURN_OK;

}


HRET filesystem_detail::get_current_path (path_str_type& strPathName) noexcept {

    HPATH_BUF buf = {0};
    HSZ res = nullptr;

    HASSERT_RETURN((res = getcwd(buf, HPATH_LEN)) != nullptr, SYS_FAILED);
    strPathName.clear();
    strPathName.assign(buf);

    HRETURN_OK; 

}


HRET filesystem_detail::get_parent_path(const path_str_type& strCurrentPath,
        path_str_type& strParentPath) noexcept {
    
    HASSERT_RETURN(exists(strCurrentPath), INVL_PARA);
    
    HSTR strSrcPath = strCurrentPath;
    if (not is_absolute(strCurrentPath)) {
        HASSERT_RETURN(setas_absolute(strSrcPath), INVL_PARA);
    }

    if (is_root_end(strSrcPath)) {
        strSrcPath = strSrcPath.substr(0, strSrcPath.length() - 1);
    }

    HCSZ pos = strrchr(strSrcPath.c_str(), root_elem);
    HASSERT_RETURN(pos != nullptr, INVL_PARA);

    strParentPath.assign(++pos);

    HRETURN_OK;
}


HRET filesystem_detail::setas_absolute(path_str_type& strPath) noexcept {

    if (is_absolute(strPath)) {
        HRETURN_OK;
    }

    HPATH_BUF buf = {0};
    HASSERT_RETURN(realpath(strPath.c_str(), buf) != nullptr, SRC_ERR);
    
    strPath.assign(buf);
    HRETURN_OK;

}


HRET filesystem_detail::create_directory(const path_str_type& strPathFileName) noexcept {

    HASSERT_RETURN(strPathFileName.c_str() == 0, INVL_PARA);
    HRETURN_OK;

}


filesystem_detail::filesize_t filesystem_detail::get_file_size(const path_str_type& strPathFileName) noexcept{

    struct stat buf = {0};
    if (stat(strPathFileName.c_str(), &buf) != 0) {
        return 0;
    }
    return static_cast<filesize_t>(buf.st_size);

}


filesystem_detail::path_str_type filesystem_detail::path_just_filename(const path_str_type& strPathFileName) noexcept {
    
    HCSZ pos = strrchr(strPathFileName.c_str(), root_elem);
    if (pos == nullptr) {
        return strPathFileName;
    }

    return path_str_type(++pos);
}


filesystem_detail::path_str_type filesystem_detail::path_only_filename(
    const path_str_type& strPathFileName) noexcept {

    path_str_type strFileName = path_just_filename(strPathFileName);
    HCSZ pos = strrchr(strFileName.c_str(), po_elem);
    if (pos == nullptr) {
        return strFileName;
    } 

    return path_str_type(strFileName.c_str(), pos);
}


filesystem_detail::path_str_type filesystem_detail::get_file_extern_name(const path_str_type& strPathFileName) noexcept {

    HCSZ po_pos = strrchr(strPathFileName.c_str(), po_elem);
    if (po_pos == nullptr) {
        return path_str_type();
    }

    HCSZ root_pos = strchr(strPathFileName.c_str(), root_elem);
    if (po_pos < root_pos) {
        return path_str_type();
    }

    return path_str_type(++po_pos);

}


bool filesystem_detail::is_absolute(const path_str_type& strPathFileName) noexcept {

    return is_root_begin(strPathFileName);

}


bool filesystem_detail::is_relative(const path_str_type& strPathFileName) noexcept {

    return not is_absolute(strPathFileName);

}


filesystem_detail::path_str_type filesystem_detail::append (
    const path_str_type& base_path, const path_str_type& new_path) noexcept {
    
    path_str_type res;
    if (base_path.empty()) {
        return res;
    }

    if (new_path.empty()) {
        return base_path;
    }

    path_str_type newpath = new_path;
    if (is_root_begin(newpath)) {
        HStr::RemoveLeft(newpath, 1);
    }
    if (is_root_end(newpath)) {
        HStr::RemoveRight(newpath, 1);
    }

    res = base_path;
    res.push_back(root_elem);
    res.append(newpath);

    return res;
}



bool filesystem_detail::is_root_begin(const path_str_type& strPathFileName) noexcept {

    if (strPathFileName.empty()) {
        return false;
    }

    return strPathFileName[0] == root_elem;
}



bool filesystem_detail::is_root_end(const path_str_type& strPathFileName) noexcept {

    if (strPathFileName.empty()) {
        return false;
    }

    return strPathFileName[strPathFileName.length() - 1] == root_elem;
}


}

}