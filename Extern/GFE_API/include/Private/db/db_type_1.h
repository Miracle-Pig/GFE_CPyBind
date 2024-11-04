#pragma once
#include <GFE_API_2.h>
#include <sqlite_orm.h>
#include "Private/sqlite_orm_traits.h"

namespace GFE {
using namespace sqlite_orm;
template<class O, class T, class ...Op>
using col_t = internal::column_t<O, T, const T& (O::*)() const, void (O::*)(T), Op...>;

struct CellNum {
    int type = CT_UNKNOWN;
    id_t num = 0;
};

struct InternalMesh {
    string name;
    vector<id_t> data;
    vector<data_t> data_f;
};

struct InternalHistoryOutput {
    id_t id;
    id_t point_id;
    int position;
    string variable;
    id_t request_id;    // region和frequency在HistoryOutputRequest表里查询
    vector<data_t> data;
};

struct InternalMaterial {
    InternalMaterial() {}
    InternalMaterial(int _id, const string& _name) : id(_id), name(_name) {}
    id_t id;
    string name;
    // 以下是子表id
    vector<int> subtable_row = vector<int>(Mat_General::General_Type_Size, -1);
};

using db_t = internal::storage_t<
// CellNum
internal::table_t<CellNum,
col_t<CellNum, decltype(CellNum::type), constraints::primary_key_t<>>,
col_t<CellNum, decltype(CellNum::num)>
>,
// InternalMesh
internal::table_t<InternalMesh,
col_t<InternalMesh, decltype(InternalMesh::name), constraints::primary_key_t<>>,
col_t<InternalMesh, decltype(InternalMesh::data)>,
col_t<InternalMesh, decltype(InternalMesh::data_f)>
>,
// InternalMaterial
internal::table_t<InternalMaterial,
col_t<InternalMaterial, decltype(InternalMaterial::id), constraints::primary_key_t<>>,
col_t<InternalMaterial, decltype(InternalMaterial::name), constraints::unique_t<>>,
col_t<InternalMaterial, decltype(InternalMaterial::subtable_row), constraints::default_t<int>>
>,
// Mat_Density
internal::table_t<Mat_Density,
col_t<Mat_Density, decltype(Mat_Density::temp_dp)>,
col_t<Mat_Density, decltype(Mat_Density::n_param)>,
col_t<Mat_Density, decltype(Mat_Density::params)>
>,
// Mat_Elastic
internal::table_t<Mat_Elastic,
col_t<Mat_Elastic, decltype(Mat_Elastic::temp_dp)>,
col_t<Mat_Elastic, decltype(Mat_Elastic::n_param)>,
col_t<Mat_Elastic, decltype(Mat_Elastic::type)>,
col_t<Mat_Elastic, decltype(Mat_Elastic::moduli_time_scale)>,
col_t<Mat_Elastic, decltype(Mat_Elastic::compression)>,
col_t<Mat_Elastic, decltype(Mat_Elastic::tension)>,
col_t<Mat_Elastic, decltype(Mat_Elastic::params)>
>,
// Mat_Plastic
internal::table_t<Mat_Plastic,
col_t<Mat_Plastic, decltype(Mat_Plastic::harden_type)>,
col_t<Mat_Plastic, decltype(Mat_Plastic::rate_dp)>,
col_t<Mat_Plastic, decltype(Mat_Plastic::temp_dp)>,
col_t<Mat_Plastic, decltype(Mat_Plastic::params)>
>,
// Mat_HyperFoam
internal::table_t<Mat_HyperFoam,
col_t<Mat_HyperFoam, decltype(Mat_HyperFoam::test_data)>,
col_t<Mat_HyperFoam, decltype(Mat_HyperFoam::N)>,
col_t<Mat_HyperFoam, decltype(Mat_HyperFoam::temp_dp)>,
col_t<Mat_HyperFoam, decltype(Mat_HyperFoam::moduli_time_scale)>,
col_t<Mat_HyperFoam, decltype(Mat_HyperFoam::params)>,
col_t<Mat_HyperFoam, decltype(Mat_HyperFoam::uniaxial)>,
col_t<Mat_HyperFoam, decltype(Mat_HyperFoam::biaxial)>,
col_t<Mat_HyperFoam, decltype(Mat_HyperFoam::simple_shear)>,
col_t<Mat_HyperFoam, decltype(Mat_HyperFoam::planar)>,
col_t<Mat_HyperFoam, decltype(Mat_HyperFoam::volumetric)>
>,
// Mat_HyperElastic
internal::table_t<Mat_HyperElastic,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::he_type)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::test_data)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::moduli_time_scale)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::has_poisson)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::poisson)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::N)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::temp_dp)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::params)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::uniaxial)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::biaxial)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::planar)>,
col_t<Mat_HyperElastic, decltype(Mat_HyperElastic::volumetric)>
>,
// Mat_Damping
internal::table_t<Mat_Damping,
col_t<Mat_Damping, decltype (Mat_Damping::n_param), constraints::default_t<int>>,
col_t<Mat_Damping, decltype (Mat_Damping::params)>
>,
// Mat_ViscoElastic
internal::table_t<Mat_ViscoElastic,
col_t<Mat_ViscoElastic, decltype (Mat_ViscoElastic::type), constraints::default_t<int>>,
col_t<Mat_ViscoElastic, decltype (Mat_ViscoElastic::n_param), constraints::default_t<int>>,
col_t<Mat_ViscoElastic, decltype (Mat_ViscoElastic::params)>
>,
// Mat_ConcreteDamaged
internal::table_t<Mat_ConcreteDamaged,
col_t<Mat_ConcreteDamaged, decltype (Mat_ConcreteDamaged::n_plasticity), constraints::default_t<int>>,
col_t<Mat_ConcreteDamaged, decltype (Mat_ConcreteDamaged::plasticity)>,
col_t<Mat_ConcreteDamaged, decltype (Mat_ConcreteDamaged::n_comp_harden), constraints::default_t<int>>,
col_t<Mat_ConcreteDamaged, decltype (Mat_ConcreteDamaged::comp_harden)>,
col_t<Mat_ConcreteDamaged, decltype (Mat_ConcreteDamaged::n_comp_damage), constraints::default_t<int>>,
col_t<Mat_ConcreteDamaged, decltype (Mat_ConcreteDamaged::comp_damage)>,
col_t<Mat_ConcreteDamaged, decltype (Mat_ConcreteDamaged::n_tens_stiff), constraints::default_t<int>>,
col_t<Mat_ConcreteDamaged, decltype (Mat_ConcreteDamaged::tens_stiff)>,
col_t<Mat_ConcreteDamaged, decltype (Mat_ConcreteDamaged::n_tens_damage), constraints::default_t<int>>,
col_t<Mat_ConcreteDamaged, decltype (Mat_ConcreteDamaged::tens_damage)>
>,
// Mat_MohrCoulomb
internal::table_t<Mat_MohrCoulomb,
col_t<Mat_MohrCoulomb, decltype (Mat_MohrCoulomb::n_plasticity), constraints::default_t<int>>,
col_t<Mat_MohrCoulomb, decltype (Mat_MohrCoulomb::plasticity)>,
col_t<Mat_MohrCoulomb, decltype (Mat_MohrCoulomb::n_cohesion), constraints::default_t<int>>,
col_t<Mat_MohrCoulomb, decltype (Mat_MohrCoulomb::cohesion)>
>,
// Mat_User
internal::table_t<Mat_User,
col_t<Mat_User, decltype (Mat_User::user_type), constraints::default_t<int>>,
col_t<Mat_User, decltype (Mat_User::n_constants), constraints::default_t<int>>,
col_t<Mat_User, decltype (Mat_User::constants)>
>,
// Mat_TestData
internal::table_t<Mat_TestData,
col_t<Mat_TestData, decltype (Mat_TestData::n_test_data), constraints::default_t<int>>,
col_t<Mat_TestData, decltype (Mat_TestData::test_data)>
>,
// Property
internal::table_t<Property,
col_t<Property, decltype(Property::id), constraints::primary_key_t<>>,
col_t<Property, decltype(Property::name), constraints::unique_t<>>,
col_t<Property, decltype(Property::elset_name), constraints::default_t<const char*>>,
col_t<Property, decltype(Property::ecc)>,
col_t<Property, decltype(Property::type)>,
col_t<Property, decltype(Property::subtable_row), constraints::default_t<int>>
>,
// PropertySolid
internal::table_t<PropertySolid,
col_t<PropertySolid, decltype(PropertySolid::mat_id)>,
col_t<PropertySolid, decltype(PropertySolid::has_thickness), constraints::default_t<bool>>,
col_t<PropertySolid, decltype(PropertySolid::thickness), constraints::default_t<int>>
>,
// PropertyBush
internal::table_t<PropertyBush,
col_t<PropertyBush, decltype(PropertyBush::params)>
>,
// PropertyBeam
internal::table_t<PropertyBeam,
col_t<PropertyBeam, decltype(PropertyBeam::shape), constraints::default_t<int>>,
col_t<PropertyBeam, decltype(PropertyBeam::mat_id), constraints::default_t<int>>,
col_t<PropertyBeam, decltype(PropertyBeam::fiber_num), constraints::default_t<int>>,
col_t<PropertyBeam, decltype(PropertyBeam::shape_params), constraints::default_t<int>>,
col_t<PropertyBeam, decltype(PropertyBeam::params), constraints::default_t<int>>,
col_t<PropertyBeam, decltype(PropertyBeam::direction), constraints::default_t<int>>
>,
// PropertyShell
internal::table_t<PropertyShell,
col_t<PropertyShell, decltype(PropertyShell::mat_id), constraints::default_t<int>>,
col_t<PropertyShell, decltype(PropertyShell::thickness), constraints::default_t<int>>,
col_t<PropertyShell, decltype(PropertyShell::integral_point), constraints::default_t<int>>,
col_t<PropertyShell, decltype(PropertyShell::layer_num), constraints::default_t<int>>,
col_t<PropertyShell, decltype(PropertyShell::params), constraints::default_t<int>>,
col_t<PropertyShell, decltype(PropertyShell::has_rebar), constraints::default_t<bool>>,
col_t<PropertyShell, decltype(PropertyShell::rebar)>
>,
// PropertyMembrane
internal::table_t<PropertyMembrane,
col_t<PropertyMembrane, decltype(PropertyMembrane::mat_id), constraints::default_t<int>>,
col_t<PropertyMembrane, decltype(PropertyMembrane::thickness), constraints::default_t<int>>,
col_t<PropertyMembrane, decltype(PropertyMembrane::has_rebar), constraints::default_t<bool>>,
col_t<PropertyMembrane, decltype(PropertyMembrane::rebar)>
>,
// PropertyConnector
internal::table_t<PropertyConnector,
col_t<PropertyConnector, decltype(PropertyConnector::behavior), constraints::default_t<const char*>>,
col_t<PropertyConnector, decltype(PropertyConnector::orientation), constraints::default_t<const char*>>,
col_t<PropertyConnector, decltype(PropertyConnector::translational_type), constraints::default_t<int>>,
col_t<PropertyConnector, decltype(PropertyConnector::rotational_type), constraints::default_t<int>>
>,
// RigidBody
internal::table_t<RigidBody,
col_t<RigidBody, decltype(RigidBody::id), constraints::primary_key_t<>>,
col_t<RigidBody, decltype(RigidBody::type), constraints::default_t<int>>,
col_t<RigidBody, decltype(RigidBody::name), constraints::default_t<const char*>, constraints::unique_t<>>,
col_t<RigidBody, decltype(RigidBody::set_name), constraints::default_t<const char*>>,
col_t<RigidBody, decltype(RigidBody::thickness), constraints::default_t<int>>,
col_t<RigidBody, decltype(RigidBody::density), constraints::default_t<int>>,
col_t<RigidBody, decltype(RigidBody::ref_node), constraints::default_t<int>>
>,
// NodeSet
internal::table_t<NodeSet,
col_t<NodeSet, decltype(NodeSet::name), constraints::primary_key_t<>>,
col_t<NodeSet, decltype(NodeSet::nodes)>
>,
// ElementSet
internal::table_t<ElementSet,
col_t<ElementSet, decltype(ElementSet::name), constraints::primary_key_t<>>,
col_t<ElementSet, decltype(ElementSet::elements)>
>,
// Surface
internal::table_t<Surface,
col_t<Surface, decltype(Surface::name), constraints::primary_key_t<>>,
col_t<Surface, decltype(Surface::elements)>,
col_t<Surface, decltype(Surface::faces)>,
col_t<Surface, decltype(Surface::nodes)>
>,
// SurfacePair
internal::table_t<SurfacePair,
col_t<SurfacePair, decltype(SurfacePair::name), constraints::primary_key_t<>>,
col_t<SurfacePair, decltype(SurfacePair::type), constraints::default_t<int>>,
col_t<SurfacePair, decltype(SurfacePair::surf_1)>,
col_t<SurfacePair, decltype(SurfacePair::surf_2)>,
col_t<SurfacePair, decltype(SurfacePair::n_param), constraints::default_t<int>>,
col_t<SurfacePair, decltype(SurfacePair::params)>
>,
//// BC_Rload
//internal::table_t<BC_Rload,
//col_t<BC_Rload, decltype(BC_Rload::name), constraints::primary_key_t<>>,
//col_t<BC_Rload, decltype(BC_Rload::func_name), constraints::default_t<const char*>>,
//col_t<BC_Rload, decltype(BC_Rload::func_name_im), constraints::default_t<const char*>>,
//col_t<BC_Rload, decltype(BC_Rload::sub_type), constraints::default_t<int>>,
//col_t<BC_Rload, decltype(BC_Rload::scale), constraints::default_t<double>>,
//col_t<BC_Rload, decltype(BC_Rload::phase), constraints::default_t<double>>,
//col_t<BC_Rload, decltype(BC_Rload::t), constraints::default_t<double>>,
//col_t<BC_Rload, decltype(BC_Rload::ref_name), constraints::default_t<const char*>>
//>,
// MPC
internal::table_t<MPC,
col_t<MPC, decltype(MPC::id), constraints::primary_key_t<>>,
col_t<MPC, decltype (MPC::n_param), constraints::default_t<int>>,
col_t<MPC, decltype(MPC::params)>
>,
// Embed
internal::table_t<Embed,
col_t<Embed, decltype(Embed::id), constraints::primary_key_t<>>,
col_t<Embed, decltype(Embed::name), constraints::default_t<const char *>>,
col_t<Embed, decltype(Embed::host_name), constraints::default_t<const char *>>,
col_t<Embed, decltype(Embed::roundoff_tolerance), constraints::default_t<int>>,
col_t<Embed, decltype(Embed::exterior_tolerance), constraints::default_t<int>>,
col_t<Embed, decltype(Embed::embedded_names)>
>,
// Incident Wave Property
internal::table_t<IncidentWaveProperty,
col_t<IncidentWaveProperty, decltype(IncidentWaveProperty::id), constraints::primary_key_t<>>,
col_t<IncidentWaveProperty, decltype(IncidentWaveProperty::def), constraints::default_t<int>>,
col_t<IncidentWaveProperty, decltype(IncidentWaveProperty::name), constraints::default_t<const char*>>,
col_t<IncidentWaveProperty, decltype(IncidentWaveProperty::data)>
>,
// Incident Wave
internal::table_t<IncidentWave,
col_t<IncidentWave, decltype(IncidentWave::id), constraints::primary_key_t<>>,
col_t<IncidentWave, decltype(IncidentWave::name), constraints::default_t<const char *>>,
col_t<IncidentWave, decltype(IncidentWave::is_node_set), constraints::default_t<bool>>,
col_t<IncidentWave, decltype(IncidentWave::set_name), constraints::default_t<const char *>>,
col_t<IncidentWave, decltype(IncidentWave::node_id), constraints::default_t<int>>,
col_t<IncidentWave, decltype(IncidentWave::surf_name), constraints::default_t<const char *>>,
col_t<IncidentWave, decltype(IncidentWave::time_detonation), constraints::default_t<double>>,
col_t<IncidentWave, decltype(IncidentWave::mag_scale_factor), constraints::default_t<double>>,
col_t<IncidentWave, decltype(IncidentWave::prop_name), constraints::default_t<const char *>>
>,
// Function
internal::table_t<Function,
col_t<Function, decltype(Function::id), constraints::primary_key_t<>>,
col_t<Function, decltype(Function::type), constraints::default_t<int>>,
col_t<Function, decltype(Function::name), constraints::default_t<const char*>>,
col_t<Function, decltype(Function::values)>
>,
// InternalHistoryOutput
internal::table_t<InternalHistoryOutput,
col_t<InternalHistoryOutput, decltype(InternalHistoryOutput::id), constraints::primary_key_t<>>,
col_t<InternalHistoryOutput, decltype(InternalHistoryOutput::point_id)>,
col_t<InternalHistoryOutput, decltype(InternalHistoryOutput::position)>,
col_t<InternalHistoryOutput, decltype(InternalHistoryOutput::variable)>,
col_t<InternalHistoryOutput, decltype(InternalHistoryOutput::request_id)>,
col_t<InternalHistoryOutput, decltype(InternalHistoryOutput::data)>
>,
// HistoryOutputRequest
internal::table_t<HistoryOutputRequest,
col_t<HistoryOutputRequest, decltype(HistoryOutputRequest::id), constraints::primary_key_t<>>,
col_t<HistoryOutputRequest, decltype(HistoryOutputRequest::region)>,
col_t<HistoryOutputRequest, decltype(HistoryOutputRequest::type)>,
col_t<HistoryOutputRequest, decltype(HistoryOutputRequest::variables)>,
col_t<HistoryOutputRequest, decltype(HistoryOutputRequest::frequency)>
>,
// ArtBoundary
internal::table_t<ArtBoundary,
col_t<ArtBoundary, decltype(ArtBoundary::id), constraints::primary_key_t<>>,
col_t<ArtBoundary, decltype(ArtBoundary::name), constraints::default_t<const char *>>,
col_t<ArtBoundary, decltype(ArtBoundary::struct_centre)>,
col_t<ArtBoundary, decltype(ArtBoundary::surf_name), constraints::default_t<const char *>>
>,
// SoilLayer
internal::table_t<SoilLayer,
col_t<SoilLayer, decltype(SoilLayer::id), constraints::primary_key_t<>>,
col_t<SoilLayer, decltype(SoilLayer::name), constraints::default_t<const char *>>,
col_t<SoilLayer, decltype(SoilLayer::n_layer), constraints::default_t<int>>,
col_t<SoilLayer, decltype(SoilLayer::layer_thickness)>,
col_t<SoilLayer, decltype(SoilLayer::layer_mat)>,
col_t<SoilLayer, decltype(SoilLayer::bedrockMat), constraints::default_t<const char *>>
>,
// Vibration Load
internal::table_t<VibLoad,
col_t<VibLoad, decltype(VibLoad::id), constraints::primary_key_t<>>,
col_t<VibLoad, decltype(VibLoad::name), constraints::default_t<const char *>>,
col_t<VibLoad, decltype(VibLoad::param)>
>,
// Spring Dashpot
internal::table_t<SpringDashpot,
col_t<SpringDashpot, decltype(SpringDashpot::id), constraints::primary_key_t<>>,
col_t<SpringDashpot, decltype(SpringDashpot::type), constraints::default_t<int>>,
col_t<SpringDashpot, decltype(SpringDashpot::name), constraints::default_t<const char*>>,
col_t<SpringDashpot, decltype(SpringDashpot::nodes)>,
col_t<SpringDashpot, decltype(SpringDashpot::dof), constraints::default_t<double>>,
col_t<SpringDashpot, decltype(SpringDashpot::dof2), constraints::default_t<double>>,
col_t<SpringDashpot, decltype(SpringDashpot::stiffness), constraints::default_t<double>>,
col_t<SpringDashpot, decltype(SpringDashpot::coefficient), constraints::default_t<double>>
>,
// FO_Frm2Frq
internal::table_t<FO::Frame,
col_t<FO::Frame, decltype(FO::Frame::frame), constraints::primary_key_t<>>,
col_t<FO::Frame, decltype(FO::Frame::frq)>,
col_t<FO::Frame, decltype(FO::Frame::has_output)>
>,
// FO_Region
internal::table_t<FO::Region,
col_t<FO::Region, decltype(FO::Region::set_names), constraints::primary_key_t<>>
>,
// FO_Data
internal::table_t<FO::Data,
col_t<FO::Data, decltype(FO::Data::frame)>,
col_t<FO::Data, decltype(FO::Data::var)>,
col_t<FO::Data, decltype(FO::Data::data)>,
col_t<FO::Data, decltype(FO::Data::data_im)>,
col_t<FO::Data, decltype(FO::Data::region_id)>,
col_t<FO::Data,decltype(FO::Data::data_integ)>,
constraints::primary_key_t<decltype(&FO::Data::frame), decltype(&FO::Data::var)>
>,
// FO_Data_Custom
internal::table_t<FO::Data_Custom,
col_t<FO::Data_Custom, decltype(FO::Data_Custom::frame)>,
col_t<FO::Data_Custom, decltype(FO::Data_Custom::var)>,
col_t<FO::Data_Custom, decltype(FO::Data_Custom::name)>,
col_t<FO::Data_Custom, decltype(FO::Data_Custom::expression)>,
col_t<FO::Data_Custom, decltype(FO::Data_Custom::data)>,
constraints::primary_key_t<decltype(&FO::Data_Custom::frame), decltype(&FO::Data_Custom::var)>
>,

// EO_Region
internal::table_t<EO::Region,
col_t<EO::Region, decltype(EO::Region::set_names), constraints::primary_key_t<>>
>,
// EO_Data
internal::table_t<EO::Data,
col_t<EO::Data, decltype(EO::Data::method)>,
col_t<EO::Data, decltype(EO::Data::var)>,
col_t<EO::Data, decltype(EO::Data::data)>,
col_t<EO::Data, decltype(EO::Data::region_id)>,
col_t<EO::Data, decltype(EO::Data::frame)>,
constraints::primary_key_t<decltype(&EO::Data::method), decltype(&EO::Data::var)>
>,
// Connector
internal::table_t<InternalConnectorBehavior,
col_t<InternalConnectorBehavior, decltype(InternalConnectorBehavior::name), constraints::primary_key_t<>>,
col_t<InternalConnectorBehavior, decltype(InternalConnectorBehavior::subtable_type_row)>
>,
internal::table_t<ConnectorElastic,
col_t<ConnectorElastic, decltype(ConnectorElastic::component), constraints::default_t<int>>,
col_t<ConnectorElastic, decltype(ConnectorElastic::extrapolation), constraints::default_t<const char*>>,
col_t<ConnectorElastic, decltype(ConnectorElastic::nonlinear), constraints::default_t<bool>>,
col_t<ConnectorElastic, decltype(ConnectorElastic::rigid), constraints::default_t<bool>>,
col_t<ConnectorElastic, decltype(ConnectorElastic::stiffness)>,
col_t<ConnectorElastic, decltype(ConnectorElastic::definition), constraints::default_t<const char*>>,
col_t<ConnectorElastic, decltype(ConnectorElastic::tensile_stiffness)>
>,
internal::table_t<ConnectorPlastic,
col_t<ConnectorPlastic, decltype(ConnectorPlastic::component), constraints::default_t<int>>
>,
internal::table_t<ConnectorHardening,
col_t<ConnectorHardening, decltype(ConnectorHardening::type), constraints::default_t<const char*>>,
col_t<ConnectorHardening, decltype(ConnectorHardening::definition), constraints::default_t<const char*>>,
col_t<ConnectorHardening, decltype(ConnectorHardening::values)>
>,
internal::table_t<ConnectorDamping,
col_t<ConnectorDamping, decltype(ConnectorDamping::type), constraints::default_t<const char*>>,
col_t<ConnectorDamping, decltype(ConnectorDamping::component), constraints::default_t<int>>,
col_t<ConnectorDamping, decltype(ConnectorDamping::extrapolation), constraints::default_t<const char*>>,
col_t<ConnectorDamping, decltype(ConnectorDamping::nolinear), constraints::default_t<bool>>,
col_t<ConnectorDamping, decltype(ConnectorDamping::values)>
>
>;

db_t internal_open(const string& path);
}
