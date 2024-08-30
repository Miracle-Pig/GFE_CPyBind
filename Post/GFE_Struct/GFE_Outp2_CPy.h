#pragma once

#include <GFE_Struct/GFE_Outp2.h>
#include "../api_a_Cpy.h"

namespace GFE{
class GridData;
}

namespace Post{
    class DB;
    namespace FO{
        GFE::GridData GetData_Node(DB* db, const string& variable, int frame, bool inc_sm = false);
    }
}