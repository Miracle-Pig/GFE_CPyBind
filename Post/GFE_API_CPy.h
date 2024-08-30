#pragma once

#include <GFE_API.h>
#include <db_type.h>

namespace Post_CPy
{
    class DB
    {
    public:
        explicit DB(std::shared_ptr<GFE::DB> _db) : m_db(_db) {}
        std::shared_ptr<GFE::DB> m_db;
    };

    /* GFE_API */
    DB* open(const std::string& db_path, bool sync);
    int getNodeNum(DB *db);
    std::string getQuantityDCRP(const std::string&);
}
