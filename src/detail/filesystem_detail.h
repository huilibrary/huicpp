/*
 * Created: Joseph Hui Sun Sep 26 2021
 * Description: 
  		Wrapper for huicpp file system detail.
*/

#ifndef __H_HUICPP_FILE_SYSTEM_DETAIL_H__
#define __H_HUICPP_FILE_SYSTEM_DETAIL_H__


#include "../huicpp.h"

namespace HUICPP {

namespace detail {

class filesystem_detail {
public:
    typedef HCH path_elem_type;
    typedef HSTR path_str_type;
    typedef HULL filesize_t;
    static constexpr const path_elem_type root_elem = '/';
    static constexpr const path_elem_type po_elem = '.';

protected:
    filesystem_detail() noexcept { }

    explicit filesystem_detail(const path_str_type& strpath) noexcept
        : m_name(strpath) { }

    virtual ~ filesystem_detail() {  }

public:
    const path_str_type& GetPath() const noexcept { return m_name; }

    void SetPath (const path_str_type& _path) noexcept { m_name = _path; }

    void Clear() noexcept { m_name.clear(); }

protected:
    bool exists() const noexcept {
        return exists(m_name);
    }

    bool is_dir() const noexcept {
        return is_dir(m_name);
    }

    bool is_file() const noexcept {
        return is_file(m_name);
    }

    HRET copy (const path_str_type& strPathFileName) const noexcept {
        return copy(m_name, strPathFileName);
    }

    HRET remove() const noexcept {
        return remove(m_name);
    }

    HRET file_rename(const path_str_type& strDstPathFileName) noexcept {

        HSTR src_name = m_name;
        HNOTOK_RETURN(file_rename(src_name, strDstPathFileName));
        m_name = src_name;
        HRETURN_OK;

    }

    void setas_current() noexcept {
        HIGNORE_RETURN(get_current_path(m_name));
    }

    HRET get_parent_path(path_str_type& strParentPath) const noexcept {
        return get_parent_path(m_name, strParentPath);
    }

    HRET create_directory() const noexcept {
        return create_directory(m_name);
    }

    filesize_t get_file_size() const noexcept {
        return get_file_size(m_name);
    }

    path_str_type path_just_filename() const noexcept {
        return path_just_filename(m_name);
    }

    path_str_type path_only_filename() const noexcept {
        return path_only_filename(m_name);
    }

    path_str_type get_file_extern_name() const noexcept {
        return get_file_extern_name(m_name);
    }

    void setas_absolute() noexcept {
        HIGNORE_RETURN(setas_absolute(m_name));
    }

    bool is_absolute() const noexcept {
        return is_absolute(m_name);
    }

    bool is_relative() const noexcept {
        return is_relative(m_name);
    }

    void append(const path_str_type& new_path) noexcept {
        m_name =  append(m_name, new_path);
    }

public:
    static bool exists(const path_str_type& strPathFileName) noexcept;

    static bool is_dir(const path_str_type& strPathFileName) noexcept;

    static bool is_file(const path_str_type& strPathFileName) noexcept;

    static HRET copy(const path_str_type& srcpath, 
        const path_str_type& dstpath) noexcept;

    static HRET remove(const path_str_type& strPathFileName) noexcept;

    static HRET file_rename(const path_str_type& strPathFileName, const path_str_type& strDstPathFileName) noexcept;

    static HRET get_current_path (path_str_type& strPathName) noexcept;

    static HRET get_parent_path(const path_str_type& strCurrentPath,
        path_str_type& strParentPath) noexcept;

    static HRET setas_absolute(path_str_type& strPath) noexcept;

    static HRET create_directory(const path_str_type& strPathFileName) noexcept;

    static filesize_t get_file_size(const path_str_type& strPathFileName) noexcept;

    static path_str_type path_just_filename(const path_str_type& strPathFileName) noexcept;

    static path_str_type path_only_filename(const path_str_type& strPathFileName) noexcept;

    static path_str_type get_file_extern_name(const path_str_type& strPathFileName) noexcept;

    static bool is_absolute(const path_str_type& strPathFileName) noexcept;

    static bool is_relative(const path_str_type& strPathFileName) noexcept;

    static path_str_type append(const path_str_type& base_path,
        const path_str_type& new_path) noexcept;

    static bool is_root_begin(const path_str_type& strPathFileName) noexcept;

    static bool is_root_end(const path_str_type& strPathFileName) noexcept;

private:
    bool is_root_begin() const noexcept {
        return is_root_begin(m_name);
    }

    bool is_root_end() const noexcept {
        return is_root_end(m_name);
    }
    
private:
    path_str_type m_name;
};


}

}

#endif // __H_HUICPP_FILE_SYSTEM_DETAIL_H__