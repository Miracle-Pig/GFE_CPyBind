#pragma once
#include <vector>
#include "../api_a_Cpy.h"

namespace Post
{
    namespace FO
    {
        std::vector<data_t> GetData(DB *db, const string& var, bool fill_empty_frame, int frame);
    }
}