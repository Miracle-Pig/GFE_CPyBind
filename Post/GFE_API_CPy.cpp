#include "GFE_API_CPy.h"

Post_CPy::DB *Post_CPy::open(const std::string &db_path, bool sync = false)
{
    auto db = GFE::open(db_path, sync);
    return new DB(db);
}

int Post_CPy::getNodeNum(DB *db)
{
    return GFE::getNodeNum(db->m_db);
}

std::string Post_CPy::getQuantityDCRP(const std::string& s){
    return GFE::getQuantityDCRP(s);
}

