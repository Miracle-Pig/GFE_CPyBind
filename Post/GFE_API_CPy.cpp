#include "GFE_API_CPy.h"

GFE_Post_CPy::DB_CPy *GFE_Post_CPy::open(const std::string &db_path, bool sync = false)
{
    auto db = GFE::open(db_path, sync);
    return new DB_CPy(db);
}

int GFE_Post_CPy::getNodeNum(DB_CPy *db)
{
    return GFE::getNodeNum(db->m_db);
}

std::string GFE_Post_CPy::getQuantityDCRP(const std::string& s){
    return GFE::getQuantityDCRP(s);
}

int GFE_Post_CPy::add(int a, int b)
{
    return a + b;
}
