#pragma once
#include <GFE_API_2.h>
#include <sqlite_orm.h>
#include "Private/sqlite_orm_traits.h"

namespace GFE
{
using std::string;
using std::vector;

namespace Surf {
struct LabelHash_ {
    int id;
    string label;
};
struct FaceHash_ {
    int id;
    string name;        // name of a FaceHash
    std::unordered_map<int, vector<int>> nodes;     // {face label, nodes of an elface(anti-clock)}
};
}

using namespace sqlite_orm;
template<class O, class T, class ...Op>
using col_t = internal::column_t<O, T, const T& (O::*)() const, void (O::*)(T), Op...>;

using settings_t = internal::storage_t<
    // Variable
    internal::table_t<Variable_,
                      col_t<Variable_, decltype(Variable_::quantity)>,
                      col_t<Variable_, decltype(Variable_::component)>,
                      col_t<Variable_, decltype(Variable_::type)>,
                      constraints::primary_key_t<decltype(&Variable_::quantity), decltype(&Variable_::component)>
                      >,
    // Element
    internal::table_t<Element_,
                      col_t<Element_, decltype(Element_::id), constraints::primary_key_t<>>,
                      col_t<Element_, decltype(Element_::name)>,
                      col_t<Element_, decltype(Element_::node_num)>,
                      col_t<Element_, decltype(Element_::face_num)>,
                      col_t<Element_, decltype(Element_::max_face_node_num)>,
                      col_t<Element_, decltype(Element_::face_order)>
                      >,
    // LabelHash
    internal::table_t<Surf::LabelHash_,
                      col_t<Surf::LabelHash_, decltype(Surf::LabelHash_::id), constraints::primary_key_t<>>,
                      col_t<Surf::LabelHash_, decltype(Surf::LabelHash_::label)>
                      >,
    // FaceHash
    internal::table_t<Surf::FaceHash_,
                      col_t<Surf::FaceHash_, decltype(Surf::FaceHash_::id), constraints::primary_key_t<>>,
                      col_t<Surf::FaceHash_, decltype(Surf::FaceHash_::name)>,
                      col_t<Surf::FaceHash_, decltype(Surf::FaceHash_::nodes)>
                      >,
    // ElementSubType
    internal::table_t<ElementSubType_,
                      col_t<ElementSubType_, decltype(ElementSubType_::id), constraints::primary_key_t<>>,
                      col_t<ElementSubType_, decltype(ElementSubType_::name)>,
                      col_t<ElementSubType_, decltype(ElementSubType_::parent)>
                      >
    >;

settings_t& getSettings();
void releaseSettings();

/**
 * @param field_or_hist 0 field else history
 */
vector<string> preselectVar(GFE::VT2 type, int field_or_hist);

}
