#pragma once

#ifdef __GNUC__
    #define FuncSig __PRETTY_FUNCTION__
#elif defined _MSC_VER
    #define FuncSig __FUNCSIG__
#endif

#define EnumDefMacro(name, _namespace) \
    namespace sqlite_orm {  \
        using _namespace::name;  \
        template<>  \
        struct type_printer<name> : public text_printer {}; \
        template<>  \
        struct statement_binder<name> {     \
            int bind(sqlite3_stmt *stmt, int index, const name& value) {    \
                return statement_binder<std::string>().bind(stmt, index, name##ToString(value));  \
            }   \
        };  \
        template<>  \
        struct field_printer<name> {    \
            std::string operator()(const name &t) const {   \
                return name##ToString(t); \
            }   \
        };  \
        template<>  \
        struct row_extractor<name> {    \
            name extract(const char *row_value) {   \
                if(auto enum_ = name##FromString(row_value)) {    \
                    return *enum_;  \
                } else {    \
                    throw std::runtime_error("incorrect ##name## string (" + std::string(row_value) + ")"); \
                }   \
            }   \
            name extract(sqlite3_stmt *stmt, int columnIndex) { \
                auto str = sqlite3_column_text(stmt, columnIndex);  \
                return this->extract((const char *)str);    \
            }   \
        };  \
    }

#define StdVectorTraitsDefMacro(type)   \
namespace sqlite_orm {  \
    template<>  \
    struct type_printer<std::vector<type>> : public blob_printer {};    \
    template<>  \
    struct statement_binder<std::vector<type>> {    \
        int bind(sqlite3_stmt *stmt, int index, const std::vector<type> &value) {   \
            if(value.size()) {  \
                return sqlite3_bind_blob(stmt,  \
                                         index, \
                                         (const void *)&value.front(),  \
                                         int(value.size()*sizeof(type)),    \
                                         SQLITE_TRANSIENT); \
            } else {    \
                return sqlite3_bind_blob(stmt, index, "", 0, SQLITE_TRANSIENT); \
            }   \
        }   \
    };      \
    template<>  \
    struct field_printer<std::vector<type>> {   \
        std::string operator()(const std::vector<type> &t) const {  \
            std::stringstream ss;   \
            ss << std::hex; \
            for(auto c: t) {    \
                ss << c;    \
            }   \
            return ss.str();    \
        }   \
    };  \
    template<>  \
    struct row_extractor<std::vector<type>> {   \
        std::vector<type> extract(const type *row_value, std::size_t len) { \
            if(len) {   \
                std::vector<type> res;    \
                res.reserve(len);   \
                std::copy(row_value, row_value + len, std::back_inserter(res)); \
                return res; \
            } else {    \
                return {};  \
            }   \
        }   \
        std::vector<type> extract(sqlite3_stmt *stmt, int columnIndex) {    \
            auto data = static_cast<const type *>(sqlite3_column_blob(stmt, columnIndex));  \
            auto len = sqlite3_column_bytes(stmt, columnIndex); \
            return extract(data, len/sizeof(type));   \
        }   \
    };  \
}

#define IntToFourChar(I, B, V)   \
    delete [] B;   \
    B = new char[4];    \
    memcpy(B, &I, 4);   \
    for(auto it = 0; it < 4; ++it) \
        V.push_back(B[it]);

#define DoubleToEightChar(D, B, V)  \
    delete [] B;   \
    B = new char[8];    \
    memcpy(B, &D, 8);   \
    for(auto it = 0; it < 8; ++it) \
        V.push_back(B[it]);

