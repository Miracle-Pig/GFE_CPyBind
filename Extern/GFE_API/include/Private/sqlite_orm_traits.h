#pragma once
#include <string>
#include <vector>
#include <sqlite_orm.h>
#include <GFE_API_2.h>
#include "macros_def.h"

namespace sqlite_orm {
template<class T>
struct type_is_nullable<std::vector<T>> : public std::true_type {
    bool operator()(const std::vector<T> &t) const {
        return t.empty();
    }
};
};

namespace sqlite_orm
{
template<>
struct type_printer<std::unordered_map<int, std::vector<int>>> : public blob_printer {};

template<>
struct statement_binder<std::unordered_map<int, std::vector<int>>> {
    int bind(sqlite3_stmt *stmt, int index, const std::unordered_map<int, std::vector<int>>& value) {
        if(value.size())
        {
            std::vector<int> value_;
            for(auto it = value.begin(); it != value.end(); ++it){
                value_.push_back(it->first);
                value_.push_back(it->second.size());
                for(auto&& f : it->second)
                    value_.push_back(f);
            }
            return sqlite3_bind_blob(stmt, index, (const void*)&value_[0], value_.size() * sizeof(int), SQLITE_TRANSIENT);
        }
        else
            return sqlite3_bind_blob(stmt, index, "", 0, SQLITE_TRANSIENT);
    }
};

template<>
struct field_printer<std::unordered_map<int, std::vector<int>>> {
    std::string operator()(const std::unordered_map<int, std::vector<int>>& t) const {
        std::stringstream ss;
        ss << std::hex;
        for(auto it = t.cbegin(); it != t.cend(); ++it){
            ss << it->first << it->second.size();
            for(auto&& f : it->second)
                ss << f;
        }
        return ss.str();
    }
};

template<>
struct row_extractor<std::unordered_map<int, std::vector<int>>> {
    std::unordered_map<int, std::vector<int>> extract(const char *row_value, std::size_t len) {
        if(len) {
            std::unordered_map<int, std::vector<int>> ret;
            auto p = reinterpret_cast<const int*>(row_value);
            int index = 0;
            while(index < len / sizeof(int)){
                int key = p[index];
                int size = p[++index];
                ++index;
                ret[key] = std::vector<int>(p+index, p+index+size);
                index += size;
            }
            return ret;

        }
        else
            return {};
    }

    std::unordered_map<int, std::vector<int>> extract(sqlite3_stmt *stmt, int columnIndex) {
        auto data = static_cast<const char *>(sqlite3_column_blob(stmt, columnIndex));
        auto len = sqlite3_column_bytes(stmt, columnIndex);
        return extract(data, len);
    }
};
};

namespace sqlite_orm
{
template<>
struct type_printer<std::unordered_map<std::string, std::string>> : public blob_printer {};

template<>
struct statement_binder<std::unordered_map<std::string, std::string>> {
    int bind(sqlite3_stmt *stmt, int index, const std::unordered_map<std::string, std::string>& value) {
        if(value.size())
        {
            std::size_t size = 0;
            size += sizeof(int);
            for(auto it = value.begin(); it != value.end(); ++it)
            {
                size += sizeof(int);
                size += it->first.length();
                size += sizeof(int);
                size += it->second.length();
            }
            std::vector<char> value_;
            value_.reserve(size);
            char *buf = new char[4];
            int len = value.size();
            IntToFourChar(len, buf, value_);
            for(auto it = value.begin(); it != value.end(); ++it)
            {
                len = it->first.length();
                IntToFourChar(len, buf, value_);
                for(const auto& c : it->first)
                    value_.push_back(c);
                len = it->second.length();
                IntToFourChar(len, buf, value_);
                for(const auto& c : it->second)
                    value_.push_back(c);
            }
            return sqlite3_bind_blob(stmt,
                                     index,
                                     (const void*)&value_.front(),
                                     int(size),
                                     SQLITE_TRANSIENT);
        }
        else
            return sqlite3_bind_blob(stmt, index, "", 0, SQLITE_TRANSIENT);
    }
};

template<>
struct field_printer<std::unordered_map<std::string, std::string>> {
    std::string operator()(const std::unordered_map<std::string, std::string>& t) const {
        std::stringstream ss;
        ss << std::hex;
        for(auto it = t.cbegin(); it != t.cend(); ++it)
            ss << it->first << it->second;
        return ss.str();
    }
};

template<>
struct row_extractor<std::unordered_map<std::string, std::string>> {
    std::unordered_map<std::string, std::string> extract(const char *row_value, std::size_t len) {
        if(len) {
            std::unordered_map<std::string, std::string> ret;
            std::size_t it = 0;
            char *buf = new char[4];
            for(int i = 0; i < 4; ++i)
                buf[i] = row_value[it++];
            int len;
            memcpy(&len, &buf[0], 4);
            for(int i = 0; i < len; ++i)
            {
                for(int j = 0; j < 4; ++j)
                    buf[j] = row_value[it++];
                int sublen1;
                memcpy(&sublen1, &buf[0], 4);
                std::string s1;
                for(int j = 0; j < sublen1; ++j)
                    s1.push_back(row_value[it++]);
                for(int j = 0; j < 4; ++j)
                    buf[j] = row_value[it++];
                int sublen2;
                memcpy(&sublen2, &buf[0], 4);
                std::string s2;
                for(int j = 0; j < sublen2; ++j)
                    s2.push_back(row_value[it++]);
                ret[s1] = s2;
            }
            return ret;

        }
        else
            return {};
    }

    std::unordered_map<std::string, std::string> extract(sqlite3_stmt *stmt, int columnIndex) {
        auto data = static_cast<const char *>(sqlite3_column_blob(stmt, columnIndex));
        auto len = sqlite3_column_bytes(stmt, columnIndex);
        return extract(data, len);
    }
};
};

namespace sqlite_orm {

template<>
struct type_printer<GFE::RebarLayer> : public blob_printer {};

template<>
struct statement_binder<GFE::RebarLayer> {
    int bind(sqlite3_stmt *stmt, int index, const GFE::RebarLayer& value) {
        std::size_t size = 0;
        size += sizeof(int);
        size += value.orientation_name.length();
        size += sizeof(int);
        for(int i = 0; i < value.rebar_num; ++i)
        {
            size += sizeof(int);
            size += value.layer_name[i].length();
            size += sizeof(int);
            size += value.mat_name[i].length();
            size += sizeof(double) * 7;
        }
        std::vector<char> value_;
        value_.reserve(size);
        char* buf = new char[4];
        int len = value.orientation_name.size();
        IntToFourChar(len, buf, value_);
        for(const auto& c : value.orientation_name)
            value_.push_back(c);
        len = value.rebar_num;
        IntToFourChar(len, buf, value_);
        for(int i = 0; i < value.rebar_num; ++i)
        {
            len = value.layer_name[i].length();
            IntToFourChar(len, buf, value_);
            for(const auto& c : value.layer_name[i])
                value_.push_back(c);
            len = value.mat_name[i].length();
            IntToFourChar(len, buf, value_);
            for(const auto& c : value.mat_name[i])
                value_.push_back(c);
            for(int j = 0; j < 7; ++j)
            {
                DoubleToEightChar(value.params[i * 7 + j], buf, value_);
            }
        }
        return sqlite3_bind_blob(stmt,
                                 index,
                                 (const void *)&value_.front(),
                                 int(size),
                                 SQLITE_TRANSIENT);
    }
};

template<>
struct field_printer<GFE::RebarLayer> {
    std::string operator()(const GFE::RebarLayer& t) const {
        std::stringstream ss;
        ss << std::hex;
        ss << t.orientation_name;
        ss << t.rebar_num;
        for(int i = 0; i < t.rebar_num; ++i)
        {
            ss << t.layer_name[i];
            ss << t.mat_name[i];
            for(int j = 0; j < 7; ++j)
                ss << t.params[i * 7 + j];
        }
        return ss.str();
    }
};

template<>
struct row_extractor<GFE::RebarLayer> {
    GFE::RebarLayer extract(const char *row_value, std::size_t len) {
        if(len) {
            GFE::RebarLayer res;
            std::size_t it = 0;
            char *buf = new char[4];
            for(int i = 0; i < 4; ++i)
                buf[i] = row_value[it++];
            int len;
            memcpy(&len, &buf[0], 4);
            for(int i = 0; i < len; ++i)
                res.orientation_name.push_back(row_value[it++]);
            for(int i = 0; i < 4; ++i)
                buf[i] = row_value[it++];
            memcpy(&res.rebar_num, &buf[0], 4);
            for(int i = 0; i < res.rebar_num; ++i)
            {
                for(int j = 0; j < 4; ++j)
                    buf[j] = row_value[it++];
                memcpy(&len, &buf[0], 4);
                std::string s;
                for(int j = 0; j < len; ++j)
                    s.push_back(row_value[it++]);
                res.layer_name.push_back(s);
                for(int j = 0; j < 4; ++j)
                    buf[j] = row_value[it++];
                memcpy(&len, &buf[0], 4);
                s = "";
                for(int j = 0; j < len; ++j)
                    s.push_back(row_value[it++]);
                res.mat_name.push_back(s);
                delete [] buf;
                buf = new char[8];
                for(int j = 0; j < 7; ++j)
                {
                    for(int k = 0; k < 8; ++k)
                        buf[k] = row_value[it++];
                    double para;
                    memcpy(&para, &buf[0], 8);
                    res.params.push_back(para);
                }
            }
            return res;
        }
        else
            return GFE::RebarLayer();
    }

    GFE::RebarLayer extract(sqlite3_stmt *stmt, int columnIndex) {
        auto data = static_cast<const char *>(sqlite3_column_blob(stmt, columnIndex));
        auto len = sqlite3_column_bytes(stmt, columnIndex);
        return extract(data, len);

    }
};
};

namespace sqlite_orm
{
template<>
struct type_printer<std::vector<std::string>> : public blob_printer {};

template<>
struct statement_binder<std::vector<std::string>> {
    int bind(sqlite3_stmt *stmt, int index, const std::vector<std::string>& value) {
        if(value.size())
        {
            std::size_t size = 0;
            size += sizeof(int);
            for(int i = 0; i < value.size(); ++i)
            {
                size += sizeof(int);
                size += value[i].length();
            }
            std::vector<char> value_;
            value_.reserve(size);
            char *buf = new char[4];
            int len = value.size();
            IntToFourChar(len, buf, value_);
            for(int i = 0; i < value.size(); ++i)
            {
                len = value[i].length();
                IntToFourChar(len, buf, value_);
                for(const auto& c : value[i])
                    value_.push_back(c);
            }
            return sqlite3_bind_blob(stmt,
                                     index,
                                     (const void *)&value_.front(),
                                     int(size),
                                     SQLITE_TRANSIENT);
        }
        else
            return sqlite3_bind_blob(stmt, index, "", 0, SQLITE_TRANSIENT);
    }
};

template<>
struct field_printer<std::vector<std::string>> {
    std::string operator()(const std::vector<std::string>& t) const {
        std::stringstream ss;
        ss << std::hex;
        for(int i = 0; i < t.size(); ++i)
            ss << t[i];
        return ss.str();
    }
};

template<>
struct row_extractor<std::vector<std::string>> {
    std::vector<std::string> extract(const char *row_value, std::size_t len) {
        if(len) {
            std::vector<std::string> res;
            std::size_t it = 0;
            char *buf = new char[4];
            for(int i = 0; i < 4; ++i)
                buf[i] = row_value[it++];
            int len;
            memcpy(&len, &buf[0], 4);
            for(int i = 0; i < len; ++i)
            {
                for(int j = 0; j < 4; ++j)
                    buf[j] = row_value[it++];
                int sublen;
                memcpy(&sublen, &buf[0], 4);
                std::string s;
                for(int j = 0; j < sublen; ++j)
                    s.push_back(row_value[it++]);
                res.push_back(s);
            }
            return res;
        }
        else
            return {};
    }

    std::vector<std::string> extract(sqlite3_stmt *stmt, int columnIndex) {
        auto data = static_cast<const char *>(sqlite3_column_blob(stmt, columnIndex));
        auto len = sqlite3_column_bytes(stmt, columnIndex);
        return extract(data, len);
    }
};
};

namespace sqlite_orm
{
template<>
struct type_printer<std::vector<std::pair<int, int>>> : public blob_printer {};

template<>
struct statement_binder<std::vector<std::pair<int, int>>> {
    int bind(sqlite3_stmt *stmt, int index, const std::vector<std::pair<int, int>>& value) {
        if(value.size())
        {
            std::vector<int> value_;
            for(int i = 0; i < value.size(); ++i)
            {
                value_.push_back(value[i].first);
                value_.push_back(value[i].second);
            }
            return sqlite3_bind_blob(stmt, index, (const void*)&value_.front(), int(value_.size() * sizeof(int)), SQLITE_TRANSIENT);
        }
        else
            return sqlite3_bind_blob(stmt, index, "", 0, SQLITE_TRANSIENT);
    }
};

template<>
struct field_printer<std::vector<std::pair<int, int>>> {
    std::string operator()(const std::vector<std::pair<int, int>>& t) const {
        std::stringstream ss;
        ss << std::hex;
        for(int i = 0; i < t.size(); ++i)
            ss << t[i].first << t[i].second;
        return ss.str();
    }
};

template<>
struct row_extractor<std::vector<std::pair<int, int>>> {
    std::vector<std::pair<int, int>> extract(const int *row_value, std::size_t len) {
        if(len) {
            std::vector<std::pair<int, int>> res;
            int index = 0;
            while(index < len / sizeof(int)){
                int first = row_value[index++];
                int second = row_value[index++];
                res.push_back({first, second});
            }
            return res;

        }
        else
            return {};
    }

    std::vector<std::pair<int, int>> extract(sqlite3_stmt *stmt, int columnIndex) {
        auto data = static_cast<const int *>(sqlite3_column_blob(stmt, columnIndex));
        auto len = sqlite3_column_bytes(stmt, columnIndex);
        return extract(data, len);
    }
};

/** vector<array> **/

template<class T, size_t N>
struct type_printer<std::vector<std::array<T, N>>> : public blob_printer {
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");
};

template<class T, size_t N>
struct statement_binder<std::vector<std::array<T, N>>> {
    int bind(sqlite3_stmt *stmt, int index, const std::vector<std::array<T, N>>& value) {
        if(value.size())
        {
            std::vector<T> value_;
            value_.reserve(value.size()*N);
            for(int i = 0; i < value.size(); ++i)
                for(int j = 0; j < N; j++)
                    value_.push_back(value[i][j]);
            return sqlite3_bind_blob(stmt, index, (const void*)&value_.front(), int(value_.size() * sizeof(T)), SQLITE_TRANSIENT);
        }
        else
            return sqlite3_bind_blob(stmt, index, "", 0, SQLITE_TRANSIENT);
    }
};

template<class T, size_t N>
struct field_printer<std::vector<std::array<T, N>>> {
    std::string operator()(const std::vector<std::array<T, N>>& t) const {
        std::stringstream ss;
        ss << std::hex;
        for(int i = 0; i < t.size(); ++i)
            for(int j = 0; j < N; ++j)
                ss << t[i][j];
        return ss.str();
    }
};

template<class T, size_t N>
struct row_extractor<std::vector<std::array<T, N>>> {
    std::vector<std::array<T, N>> extract(const T *row_value, std::size_t len) {
        if(len) {
            std::vector<std::array<T, N>> res;
            int index = 0;
            auto size = len / sizeof(T) / N;
            res.reserve(size);
            std::array<T, N> tmp;
            for(size_t i = 0; i < size; ++i) {
                for(size_t j = 0; j < N; ++j) {
                    tmp[j] = row_value[index++];
                }
                res.push_back(tmp);
            }
            return res;

        }
        else
            return {};
    }

    std::vector<std::array<T, N>> extract(sqlite3_stmt *stmt, int columnIndex) {
        auto data = static_cast<const T *>(sqlite3_column_blob(stmt, columnIndex));
        auto len = sqlite3_column_bytes(stmt, columnIndex);
        return extract(data, len);
    }
};
};

/**
 * std::vector<T>
 */
StdVectorTraitsDefMacro(short)
StdVectorTraitsDefMacro(int)
StdVectorTraitsDefMacro(float)
StdVectorTraitsDefMacro(double)
