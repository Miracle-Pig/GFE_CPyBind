#pragma once

#include "Private/sqlite_orm_traits.h"
#include "GFE_Struct/GFE_Contact.h"
#include "GFE_Struct/GFE_Surf.h"


namespace GFE {
namespace Internal {

struct SurfaceInteraction
{
    string name;
    int cohesive_behavior = 0;
    int damage_evolution = 0;
    int damage_initiation = 0;
};

using namespace sqlite_orm;
template<class O, class T, class ...Op>
using col_t = internal::column_t<O, T, const T& (O::*)() const, void (O::*)(T), Op...>;

using db_t3 = internal::storage_t<
    internal::table_t<CohesiveBehavior,
                      col_t<CohesiveBehavior, decltype(CohesiveBehavior::eligibility)>,
                      col_t<CohesiveBehavior, decltype(CohesiveBehavior::datas)>
                      >,
    internal::table_t<DamageEvolution,
                      col_t<DamageEvolution, decltype(DamageEvolution::type)>,
                      col_t<DamageEvolution, decltype(DamageEvolution::softening)>,
                      col_t<DamageEvolution, decltype(DamageEvolution::mixed_mode_behavior)>,
                      col_t<DamageEvolution, decltype(DamageEvolution::datas)>
                      >,
    internal::table_t<DamageInitiation,
                      col_t<DamageInitiation, decltype(DamageInitiation::criterion)>,
                      col_t<DamageInitiation, decltype(DamageInitiation::datas)>
                      >,
    internal::table_t<SurfaceInteraction,
                      col_t<SurfaceInteraction, decltype(SurfaceInteraction::name), constraints::primary_key_t<>>,
                      col_t<SurfaceInteraction, decltype(SurfaceInteraction::cohesive_behavior), constraints::default_t<int>>,
                      col_t<SurfaceInteraction, decltype(SurfaceInteraction::damage_evolution), constraints::default_t<int>>,
                      col_t<SurfaceInteraction, decltype(SurfaceInteraction::damage_initiation), constraints::default_t<int>>
                      >,
    internal::table_t<ContPropAssign,
                      col_t<ContPropAssign, decltype(ContPropAssign::step)>,
                      col_t<ContPropAssign, decltype(ContPropAssign::datas)>
                      >,
    internal::table_t<SurfPartition,
                      col_t<SurfPartition, decltype(SurfPartition::origin)>,
                      col_t<SurfPartition, decltype(SurfPartition::partition_id)>,
                      col_t<SurfPartition, decltype(SurfPartition::splited_surf)>,
                      constraints::primary_key_t<decltype(&SurfPartition::origin), decltype(&SurfPartition::partition_id)>
                      >

>;

db_t3 GFE_API Open3(const string&);
void GFE_API Clear(db_t3& db);

}
}
