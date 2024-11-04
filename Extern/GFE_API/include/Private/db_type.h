#include "GFE_Struct/GFE_Mesh.h"
#include "db/db_type_1.h"
#include "db/db_type_2.h"
#include "db/db_type_3.h"
#include "db/db_type_outp.h"
#include <GFE_StrUtil.h>

namespace GFE {

class DB {
public:
    DB(const db_t& _db, const Internal::db_t2& _db2, bool sync);
    ~DB();
    
    std::array<int, 3> Version();
    std::string VersionStr();

    static std::array<int, 3> ApiVersion();
    static std::string ApiVerStr();

    // True: ver1 >= ver2
    // False: ver1 < ver2
    static bool CompareVersion(std::array<int, 3> ver1, std::array<int, 3> ver2);

    void RigionDivision(std::shared_ptr<GFE::DB> db, GFE::ModelInfo& model_info);
    const ModelInfo& GetModelInfo();

    db_t internal_db;
    Internal::db_t2 idb2;
    Internal::db_t3 idb3;
    Internal::db_outp idb_outp;
    std::map<std::pair<int, int>, std::shared_ptr<vector<GFE::id_t>>> allRegionIDs;
    std::optional<ModelInfo> model_info;
    FILE* fp_sqllog = nullptr;
};

}
