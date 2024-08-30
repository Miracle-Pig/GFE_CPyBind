#pragma once
#include "Private/db_type_Cpy.h"

#include <GFE_API.h>

namespace Post
{
    /* GFE_API */
    DB* open(const std::string& db_path, bool sync);
    int getNodeNum(DB *db);
}
