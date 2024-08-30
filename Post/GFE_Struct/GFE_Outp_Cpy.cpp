#include "GFE_Outp_Cpy.h"
#include "GFE_Struct/GFE_Outp.h"
#include "../Private/db_type_Cpy.h"

std::vector<Post::data_t> Post::FO::GetData(DB *db, const string& var, bool fill_empty_frame, int frame)
{
    return GFE::FO::GetData(db->m_db, var, fill_empty_frame, frame);
}
