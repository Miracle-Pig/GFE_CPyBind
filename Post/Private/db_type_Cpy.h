#include <db_type.h>

namespace Post
{
    class DB
    {
    public:
        explicit DB(std::shared_ptr<GFE::DB> _db) : m_db(_db) {}
        std::shared_ptr<GFE::DB> m_db;
    };
}