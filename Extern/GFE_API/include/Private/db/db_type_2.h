#pragma once

#include "Private/sqlite_orm_traits.h"
#include "GFE_Struct/GFE_BC.h"
#include "GFE_Struct/GFE_Freq.h"


namespace GFE {
namespace Internal {
using namespace sqlite_orm;
template<class O, class T, class ...Op>
using col_t = internal::column_t<O, T, const T& (O::*)() const, void (O::*)(T), Op...>;

using db_t2 = internal::storage_t<
    // GenericBC
    internal::table_t<GenericBC,
                      col_t<GenericBC, decltype(GenericBC::name), constraints::primary_key_t<>>,
                      col_t<GenericBC, decltype(GenericBC::type), constraints::default_t<int>>,
                      col_t<GenericBC, decltype(GenericBC::is_ref), constraints::default_t<bool>>
                      >,
    // Boundary
    internal::table_t<Boundary,
                      col_t<Boundary, decltype(Boundary::name), constraints::primary_key_t<>>,
                      col_t<Boundary, decltype(Boundary::type), constraints::default_t<int>>,
                      col_t<Boundary, decltype(Boundary::is_node_set), constraints::default_t<bool>>,
                      col_t<Boundary, decltype(Boundary::node_id)>,
                      col_t<Boundary, decltype(Boundary::set_name), constraints::default_t<const char*>>,
                      col_t<Boundary, decltype(Boundary::valid_dof), constraints::default_t<int>>,
                      col_t<Boundary, decltype(Boundary::values)>,
                      col_t<Boundary, decltype(Boundary::func_name), constraints::default_t<const char*>>,
                      col_t<Boundary, decltype(Boundary::values_im)>,
                      col_t<Boundary, decltype(Boundary::func_name_im), constraints::default_t<const char*>>,
                      col_t<Boundary, decltype(Boundary::dist_type), constraints::default_t<int>>,
                      col_t<Boundary, decltype(Boundary::dist_name), constraints::default_t<const char*>>,
                      col_t<Boundary, decltype(Boundary::dist_dir)>
                      >,
    internal::table_t<WaterPressure,
                      col_t<WaterPressure, decltype(WaterPressure::name), constraints::primary_key_t<>>,
                      col_t<WaterPressure, decltype(WaterPressure::type), constraints::default_t<int>>,
                      col_t<WaterPressure, decltype(WaterPressure::surf_name), constraints::default_t<const char*>>,
                      col_t<WaterPressure, decltype(WaterPressure::density)>,
                      col_t<WaterPressure, decltype(WaterPressure::lvl_height)>,
                      col_t<WaterPressure, decltype(WaterPressure::model)>,
                      col_t<WaterPressure, decltype(WaterPressure::sink_wide)>
                      >,
    internal::table_t<Opti::Rload,
                      col_t<Opti::Rload, decltype(Opti::Rload::name), constraints::primary_key_t<>>,
                      col_t<Opti::Rload, decltype(Opti::Rload::sub_type)>,
                      col_t<Opti::Rload, decltype(Opti::Rload::ref_name)>,
                      col_t<Opti::Rload, decltype(Opti::Rload::func_name)>,
                      col_t<Opti::Rload, decltype(Opti::Rload::func_name_im)>,
                      col_t<Opti::Rload, decltype(Opti::Rload::scale)>,
                      col_t<Opti::Rload, decltype(Opti::Rload::phase)>,
                      col_t<Opti::Rload, decltype(Opti::Rload::t)>
                      >,
    internal::table_t<Spectrum,
                      col_t<Spectrum, decltype(Spectrum::name), constraints::primary_key_t<>>,
                      col_t<Spectrum, decltype(Spectrum::type)>,
                      col_t<Spectrum, decltype(Spectrum::data)>
                      >,
    internal::table_t<XYData,
                        col_t<XYData, decltype(XYData::name), constraints::primary_key_t<>>,
                        col_t<XYData, decltype(XYData::xlabel), constraints::default_t<const char*>>,
                        col_t<XYData, decltype(XYData::ylabel), constraints::default_t<const char*>>,
                        col_t<XYData, decltype(XYData::x)>,
                        col_t<XYData, decltype(XYData::y)>,
                        col_t<XYData, decltype(XYData::x_img)>,
                        col_t<XYData, decltype(XYData::y_img)>
                      >

>;

db_t2 GFE_API Open2(const std::string&);
}

}

