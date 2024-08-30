#include "GFE_Outp2.h"
#include "GFE_Outp2_CPy.h"
#include "../Private/db_type_Cpy.h"

GFE::GridData Post::FO::GetData_Node(Post::DB *db, const string &variable, int frame, bool inc_sm)
{
    return GFE::FO::GetData_Node(db->m_db, variable, frame, inc_sm);
}