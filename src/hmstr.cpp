


#include "hmstr.h"
#include "hstr.h"
#include <sstream>
#include <cstdarg>

namespace HUICPP{


HMstr::HMstr (const HMstr& _right) 
    : base (_right) {

}


HMstr::HMstr(const key_compare& _kcp) 
    : base(_kcp) {

}


HMstr::HMstr(const key_compare& kcp, const allocator_type& al) 
    : base (kcp, al) { }


HMstr::HMstr(HMstr&& right) 
    : base(std::move(right)) { }


HMstr& HMstr::operator=(const HMstr& other)  {

    base::operator=(other);
    return *this;

}


void HMstr::SetValue (HCSTRR key, HCSTRR val) noexcept {

    iterator fit = find(key);

    if (fit != end()) {

        fit->second = val;

    } else {

        insert(value_type(key, val));

    }

}


void HMstr::SetValue (HCSTRR key, HN val) {

    SetValue(key, HStr::FromN(val));

}


HCSTRR HMstr::GetValue (HCSTRR key) const  {
    
    static HCSTR s_str;
    return GetValue(key, s_str);

}


HCSTRR HMstr::GetValue (HCSTRR key, HCSTRR default_val) const noexcept {

    const_iterator cfit = find(key);
    if (cfit == cend()) {
        return default_val;
    }

    return cfit->second;

}


HN HMstr::GetInt (HCSTRR key) const {

    return HStr::ToN(GetValue(key));

}


HN HMstr::GetInt (HCSTRR key, HN n) const noexcept{

    const_iterator cfit = find(key);
    if (cfit == cend()) {
        return n;
    }

    return HStr::ToN(cfit->second);

}


bool HMstr::IsDigit (HCSTRR key) const {

    return HStr::IsAllDigit(GetValue(key));

}


bool HMstr::Exists (HCSTRR key) const {

    return find(key) != cend();

}


HMstr::size_type HMstr::Remove(HCSTRR key) {

    return erase(key);

}


void HMstr::Search (HCSTRR key, HVSTRR vals) const {

    for (const_iterator cfit = cbegin(); cfit != cend(); ++cfit) {

        if (HStr::IsIn(cfit->first, key)) {
            vals.push_back(cfit->second);
        }

    }

}


HMstr& HMstr::merge(const HMstr &ps) {

    for (const_iterator cit = ps.begin(); cit != ps.end(); cit++ ) {

        SetValue(cit->first, cit->second);

    }

    return *this;

}


HSTR HMstr::ToParamString() const {

    std::stringstream ss;

    for (const_iterator it = begin(); it != end(); ++it) {

        ss << it->first << "=" << it->second << "&";

    }

    HSTR res = ss.str();

    res = res.substr(0, res.length() - 1);

    return res;

}


void HMstr::SetValueFormat(HCSTRR key, HCSZ fmt, ...) {

    va_list ap;

    va_start(ap, fmt);

    SetValue(key, HStr::vFormat(fmt, ap));

    va_end(ap);

}


HSTR HMstr::ToJson () const {

    std::stringstream ss;
    ss << "[ ";

    for (const_iterator cit = cbegin(); cit != cend(); ++cit) {
        ss << "{\"name\":\"" << cit->first << "\", \"value\":\"" << cit->second << "\"},";
    }

    HSTR res = ss.str();
    res = res.substr(0, res.length() - 1);
    res += "]";

    return res;
}


}
