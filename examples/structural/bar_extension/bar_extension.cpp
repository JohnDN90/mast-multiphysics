
// C++ includes
#include <iostream>


// MAST includes
#include "examples/structural/bar_extension/bar_extension.h"
#include "elasticity/structural_system_initialization.h"
#include "elasticity/structural_discipline.h"
#include "elasticity/stress_output_base.h"
#include "property_cards/solid_1d_section_element_property_card.h"
#include "base/parameter.h"
#include "base/constant_field_function.h"
#include "property_cards/isotropic_material_property_card.h"
#include "elasticity/structural_element_base.h"
#include "elasticity/structural_nonlinear_assembly.h"
#include "boundary_condition/dirichlet_boundary_condition.h"
#include "driver/driver_base.h"

// libMesh includes
#include "libmesh/mesh_generation.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/numeric_vector.h"
#include "libmesh/parameter_vector.h"


extern libMesh::LibMeshInit* _init;


MAST::BarExtension::BarExtension() {
    
    
    // create the mesh
    _mesh       = new libMesh::SerialMesh(_init->comm());
    
    // initialize the mesh with one element
    libMesh::MeshTools::Generation::build_line(*_mesh, 10, 0, 10);
    _mesh->prepare_for_use();
    
    // create the equation system
    _eq_sys    = new  libMesh::EquationSystems(*_mesh);
    
    // create the libmesh system
    _sys       = &(_eq_sys->add_system<libMesh::NonlinearImplicitSystem>("structural"));
    
    // FEType to initialize the system
    libMesh::FEType fetype (libMesh::FIRST, libMesh::LAGRANGE);
    
    // initialize the system to the right set of variables
    _structural_sys = new MAST::StructuralSystemInitialization(*_sys,
                                                               _sys->name(),
                                                               fetype);
    _discipline     = new MAST::StructuralDiscipline(*_eq_sys);

    
    // create and add the boundary condition and loads
    _dirichlet      = new MAST::DirichletBoundaryCondition;
    _dirichlet->init(0, _structural_sys->vars());
    _discipline->add_dirichlet_bc(0, *_dirichlet);
    _discipline->init_system_dirichlet_bc(dynamic_cast<libMesh::System&>(*_sys));
    
    // initialize the equation system
    _eq_sys->init();
    
    // create the property functions and add them to the
    
    _thy             = new MAST::Parameter("thy", 0.02);
    _thz             = new MAST::Parameter("thz", 0.04);
    _E               = new MAST::Parameter("E",  72.e9);
    _nu              = new MAST::Parameter("nu",  0.33);
    _zero            = new MAST::Parameter("zero",  0.);
    _press           = new MAST::Parameter( "p",  2.e4);
    
    
    
    // prepare the vector of parameters with respect to which the sensitivity
    // needs to be benchmarked
    _params_for_sensitivity.push_back(_E);
    _params_for_sensitivity.push_back(_nu);
    _params_for_sensitivity.push_back(_thy);
    _params_for_sensitivity.push_back(_thz);
    
    
    
    _thy_f           = new MAST::ConstantFieldFunction("hy",     *_thy);
    _thz_f           = new MAST::ConstantFieldFunction("hz",     *_thz);
    _E_f             = new MAST::ConstantFieldFunction("E",      *_E);
    _nu_f            = new MAST::ConstantFieldFunction("nu",     *_nu);
    _hyoff_f         = new MAST::ConstantFieldFunction("hy_off", *_zero);
    _hzoff_f         = new MAST::ConstantFieldFunction("hz_off", *_zero);
    _press_f         = new MAST::ConstantFieldFunction("pressure", *_press);
    
    // initialize the load
    _p_load          = new MAST::BoundaryConditionBase(MAST::SURFACE_PRESSURE);
    _p_load->add(*_press_f);
    _discipline->add_side_load(1, *_p_load);
    
    // create the material property card
    _m_card         = new MAST::IsotropicMaterialPropertyCard;
    
    // add the material properties to the card
    _m_card->add(*_E_f);
    _m_card->add(*_nu_f);
    
    // create the element property card
    _p_card         = new MAST::Solid1DSectionElementPropertyCard;
    
    // tell the card about the orientation
    libMesh::Point orientation;
    orientation(1) = 1.;
    _p_card->y_vector() = orientation;
    
    // add the section properties to the card
    _p_card->add(*_thy_f);
    _p_card->add(*_thz_f);
    _p_card->add(*_hyoff_f);
    _p_card->add(*_hzoff_f);
    
    // tell the section property about the material property
    _p_card->set_material(*_m_card);
    
    _p_card->init();
        
    _discipline->set_property_for_subdomain(0, *_p_card);
    
    
    // create the output objects, one for each element
    libMesh::MeshBase::const_element_iterator
    e_it    = _mesh->elements_begin(),
    e_end   = _mesh->elements_end();
    
    for ( ; e_it != e_end; e_it++) {
        
        MAST::StressStrainOutputBase * output = new MAST::StressStrainOutputBase;
        
        // tell the object to evaluate the data for this object only
        std::set<const libMesh::Elem*> e_set;
        e_set.insert(*e_it);
        output->set_elements_in_domain(e_set);
        output->set_evaluation_mode(MAST::ELEM_QP);
        _outputs.push_back(output);
        
        _discipline->add_volume_output((*e_it)->subdomain_id(), *output);
    }
}







MAST::BarExtension::~BarExtension() {
    
    delete _m_card;
    delete _p_card;
    
    delete _p_load;
    delete _dirichlet;
    
    delete _thy_f;
    delete _thz_f;
    delete _E_f;
    delete _nu_f;
    delete _hyoff_f;
    delete _hzoff_f;
    delete _press_f;
    
    delete _thy;
    delete _thz;
    delete _E;
    delete _nu;
    delete _zero;
    delete _press;
    
    
    
    
    delete _eq_sys;
    delete _mesh;
    
    delete _discipline;
    delete _structural_sys;
    
    // iterate over the output quantities and delete them
    std::vector<MAST::StressStrainOutputBase*>::iterator
    it   =   _outputs.begin(),
    end  =   _outputs.end();
    
    for ( ; it != end; it++)
        delete *it;
    
    _outputs.clear();
}




const libMesh::NumericVector<Real>&
MAST::BarExtension::solve() {
    

    // create the nonlinear assembly object
    MAST::StructuralNonlinearAssembly   assembly;
    
    assembly.attach_discipline_and_system(*_discipline, *_structural_sys);
    
    libMesh::NonlinearImplicitSystem&      nonlin_sys   =
    dynamic_cast<libMesh::NonlinearImplicitSystem&>(assembly.system());
    
    nonlin_sys.solve();
    
    // evaluate the outputs
    //assembly.calculate_outputs(*(_sys->solution));

    assembly.clear_discipline_and_system();
    
    // write the solution for visualization
    //libMesh::ExodusII_IO(mesh).write_equation_systems("mesh.exo", eq_sys);
    
    return *(_sys->solution);
}





const libMesh::NumericVector<Real>&
MAST::BarExtension::sensitivity_solve(MAST::Parameter& p) {
    
    
    // create the nonlinear assembly object
    MAST::StructuralNonlinearAssembly   assembly;
    
    assembly.attach_discipline_and_system(*_discipline, *_structural_sys);

    libMesh::NonlinearImplicitSystem&      nonlin_sys   =
    dynamic_cast<libMesh::NonlinearImplicitSystem&>(assembly.system());

    libMesh::ParameterVector params;
    params.resize(1);
    params[0]  =  p.ptr();

    nonlin_sys.sensitivity_solve(params);

    assembly.clear_discipline_and_system();

    
    // write the solution for visualization
    //libMesh::ExodusII_IO(mesh).write_equation_systems("mesh.exo", eq_sys);
    
    return _sys->get_sensitivity_solution(0);
}



