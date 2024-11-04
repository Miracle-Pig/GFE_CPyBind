#pragma once

#pragma once

#include <sqlite_orm.h>
#include "GFE_Struct/GFE_Outp.h"

namespace GFE {
namespace Internal {

using namespace sqlite_orm;
template<class O, class T, class ...Op>
using col_t = internal::column_t<O, T, const T& (O::*)() const, void (O::*)(T), Op...>;

using db_outp = internal::storage_t<
    internal::table_t<OutpReq,
                      col_t<OutpReq, decltype(OutpReq::name), constraints::primary_key_t<>>,
                      col_t<OutpReq, decltype(OutpReq::type)>,
                      col_t<OutpReq, decltype(OutpReq::region)>,
                      col_t<OutpReq, decltype(OutpReq::vars)>,
                      col_t<OutpReq, decltype(OutpReq::frq)>,
                      col_t<OutpReq, decltype(OutpReq::inc), constraints::default_t<int>>,
                      col_t<OutpReq, decltype(OutpReq::method)>,
                      col_t<OutpReq, decltype(OutpReq::filter)>
                      >,
    internal::table_t<OutpFilter,
                      col_t<OutpFilter, decltype(OutpFilter::name), constraints::primary_key_t<>>,
                      col_t<OutpFilter, decltype(OutpFilter::cutoff)>,
                      col_t<OutpFilter, decltype(OutpFilter::N)>
                      >,
    internal::table_t<Parameter,
                      col_t<Parameter, decltype(Parameter::name), constraints::primary_key_t<>>,
                      col_t<Parameter, decltype(Parameter::content)>,
                      col_t<Parameter, decltype(Parameter::step_id)>
                      >
    >;

db_outp GFE_API OpenOutpStorage(const string&);
void GFE_API ClearAll(db_outp& db);
void GFE_API ClearResultData(db_outp& db);
void GFE_API ClearModelData(db_outp& db);
}
}
