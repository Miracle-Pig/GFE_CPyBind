#pragma once
#include <string>
#include <algorithm>
#include <regex>
#if defined _WIN32 || __GNUC__ > 7
#include <filesystem>
namespace filesystem = std::filesystem;
#else
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#endif

namespace GFE {
namespace FileInfo {

//! @brief 文件目录
template <class T>
std::basic_string<T> Dir(const std::basic_string<T>& fp) {
    std::basic_string<T> tmp;
    tmp.resize(fp.size());
    std::replace_copy_if(fp.begin(), fp.end(), tmp.begin(),
        [](T c){ return c == '\\';}, '/');
    auto _1 = tmp.rfind('/');
    return tmp.substr(0,_1+1);
}

//! @brief 文件名
template <class T>
std::basic_string<T> Name(const std::basic_string<T>& fp) {
    std::basic_string<T> tmp;
    tmp.resize(fp.size());
    std::replace_copy_if(fp.begin(), fp.end(), tmp.begin(),
        [](T c){ return c == '\\';}, '/');
    auto p1 = tmp.rfind('/');
    auto p2 = tmp.rfind('.');
    if(p1 != tmp.npos) {
        if(p2 == tmp.npos)
            return tmp.substr(p1+1);
        return tmp.substr(p1+1, p2-p1-1);
    }
    return tmp.substr(0, p2);
}

//! @brief 文件后缀
template <class T>
std::basic_string<T> Suffix(const std::basic_string<T>& fp) {
    std::basic_string<T> tmp;
    tmp.resize(fp.size());
    std::replace_copy_if(fp.begin(), fp.end(), tmp.begin(),
        [](T c){ return c == '\\';}, '/');
    auto _1 = tmp.rfind('/');
    auto _2 = tmp.substr(_1+1);
    auto _3 = _2.find('.');
    if(_3 == tmp.npos)
        return std::basic_string<T>();
    return _2.substr(_3);
}

//! @brief 含后缀的文件名
template <class T>
std::basic_string<T> FullName(const std::basic_string<T>& fp) {
    std::basic_string<T> tmp;
    tmp.resize(fp.size());
    std::replace_copy_if(fp.begin(), fp.end(), tmp.begin(),
        [](T c){ return c == '\\';}, '/');
    auto _1 = tmp.rfind('/');
    return tmp.substr(_1+1);
}

//! @brief 不含后缀的文件路径
template <class T>
std::basic_string<T> PathNoSuffix(const std::basic_string<T>& fp) {
    return Dir(fp)+Name(fp);
}

//! @brief 替换后缀名的文件路径
template <class T>
std::basic_string<T> PathRpSuffix(const std::basic_string<T>& fp, const std::basic_string<T>& newSuffix) {
    return PathNoSuffix(fp) + newSuffix;;
}

//! @brief 目录是否存在
template <class T>
bool IsDirExist(const std::basic_string<T>& fp) {
    return filesystem::is_directory(fp);
}

//! @brief 创建目录
//! @param rec 递归创建
template <class T>
bool CreateDir(const std::basic_string<T>& fp, bool rec) {
    if(IsDirExist(fp))
        return true;
    std::error_code ec;
    if(rec)
        filesystem::create_directories(fp, ec);
    else
        filesystem::create_directory(fp, ec);
    return ec.value() == 0;
}

//! @brief 根据正则表达式删除路径下的文件
template <class T>
void DeleteFiles(const std::basic_string<T>& fp, const std::basic_string<T>& reg)
{
    filesystem::directory_iterator itr(fp);
    std::basic_regex<T> pattern(reg);
    for(const auto& it : itr)
        if(std::regex_match(it.path().filename().string<T>(), pattern))
            filesystem::remove(it);
}

}

#ifdef _WIN32
std::wstring GetExePath();
inline std::wstring GetExeDir() {
    return FileInfo::Dir(GetExePath());
}
#else
std::string GetExePath();
inline std::string GetExeDir() {
    return FileInfo::Dir(GetExePath());
}
#endif

}

class GFE_FileInfo
{
public:
    explicit GFE_FileInfo(const std::string& fp);
    std::string Dir();
    std::string Name();
    std::string Suffix();
    std::string FullName();
    std::string PathNoSuffix();
    std::string PathRpSuffix(const std::string& newSuffix);

private:
    std::string m_fp;
    std::string m_fDir, m_fName, m_suffix, m_fullName;
    std::string m_fpNoSuffix;
};

class GFE_FileInfoW
{
public:
    explicit GFE_FileInfoW(const std::wstring& fp);
    std::wstring Dir();
    std::wstring Name();
    std::wstring Suffix();
    std::wstring FullName();
    std::wstring PathNoSuffix();
    std::wstring PathRpSuffix(const std::wstring& newSuffix);

private:
    std::wstring m_fp;
    std::wstring m_fDir, m_fName, m_suffix, m_fullName;
    std::wstring m_fpNoSuffix;
};

#ifdef _WIN32
#define GFE_FileInfoCP GFE_FileInfoW
#else
#define GFE_FileInfoCP GFE_FileInfo
#endif
