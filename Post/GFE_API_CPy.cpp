#include "GFE_API_CPy.h"

Post::DB *Post::open(const std::string &db_path, bool sync = false)
{
    auto db = GFE::open(db_path, sync);
    return new DB(db);
}

int Post::getNodeNum(DB *db)
{
    return GFE::getNodeNum(db->m_db);
}


