#pragma once

#include <GFE_API.h>
#include <db_type.h>

namespace GFE_Post_CPy
{
    class DB_CPy
    {
    public:
        explicit DB_CPy(std::shared_ptr<GFE::DB> _db) : m_db(_db) {}
        std::shared_ptr<GFE::DB> m_db;
    };

    DB_CPy* open(const std::string& db_path, bool sync);

    int getNodeNum(DB_CPy *db);

    std::string getQuantityDCRP(const std::string&);
    int add(int a, int b);
}
