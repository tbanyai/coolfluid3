#!/usr/bin/python

import coolfluid as cf

### Global settings

root = cf.Core.root()
env = cf.Core.environment()

env.options().configure_option('assertion_throws', False)
env.options().configure_option('assertion_backtrace', True)
env.options().configure_option('exception_backtrace', True)
env.options().configure_option('exception_aborts', True)
env.options().configure_option('exception_outputs', True)
env.options().configure_option('log_level', 3)
env.options().configure_option('regist_signal_handlers', False)

### create model

wizard = root.create_component('Wizard',  'cf3.RDM.SteadyExplicit')

wizard.create_model(model_name='Model', physical_model='cf3.physics.NavierStokes.NavierStokes3D')
model = root.get_child('Model')

### read mesh

domain = model.get_child('Domain')
domain.load_mesh(file=cf.URI('box-tet-p1-3112.msh'), name='mesh')
internal_regions = [cf.URI('//Model/Domain/mesh/topology/domain')]

### solver

solver = model.get_child('RDSolver')
solver.options().configure_option('update_vars', 'Cons3D')
solver.options().configure_option('solution_space', 'LagrangeP1')

solver.get_child('IterativeSolver').get_child('MaxIterations').options().configure_option('maxiter', 5)
solver.get_child('IterativeSolver').get_child('Update').get_child('Step').options().configure_option('cfl', 0.25)
#solver.get_child('IterativeSolver').get_child('PostActions').get_child('PeriodicWriter').options().configure_option('saverate', 10)
#solver.get_child('IterativeSolver').get_child('PostActions').get_child('PeriodicWriter').options().configure_option('filepath', cf.URI('file:periodic.plt'))

### initial conditions

iconds = solver.get_child('InitialConditions')
iconds.create_initial_condition(name='INIT')
iconds.get_child('INIT').options().configure_option('functions', ['0.500','1.500','1.500','1.500','7.000'])
iconds.get_child('INIT').options().configure_option('regions', internal_regions)

### boundary conditions

#solver.get_child('BoundaryConditions').create_boundary_condition(name='INLET',type='cf3.RDM.BcDirichlet')
#solver.get_child('BoundaryConditions').get_child('INLET').options().configure_option('functions',['0.5','1.183','1.183','3.425'])
# configure Model/RDSolver/BoundaryConditions/INLET "functions:array[string]=0.5,1.183,1.183,3.425"

### domain discretization

solver.get_child('DomainDiscretization').create_cell_term(name='INTERNAL', type='cf3.RDM.Schemes.LDA')
solver.get_child('DomainDiscretization').get_child('CellTerms').get_child('INTERNAL').options().configure_option('regions', internal_regions)

### simulate and write the result

iconds.execute()

fields=[
  cf.URI('//Model/Domain/mesh/geometry/solution'),
  cf.URI('//Model/Domain/mesh/geometry/residual'),
  cf.URI('//Model/Domain/mesh/geometry/wave_speed')]
#  cf.URI('//Model/Domain/mesh/geometry_fields/solution'),
#  cf.URI('//Model/Domain/mesh/geometry_fields/residual'),
#  cf.URI('//Model/Domain/mesh/geometry_fields/wave_speed')]
#  cf.URI('//Model/Domain/mesh/solution/solution'),
#  cf.URI('//Model/Domain/mesh/solution/residual'),
#  cf.URI('//Model/Domain/mesh/solution/wave_speed')]

tecplot_writer = model.create_component('tecplot_writer','cf3.mesh.tecplot.Writer')
tecplot_writer.options().configure_option('mesh',root.access_component('//Model/Domain/mesh'))
tecplot_writer.options().configure_option('fields',fields)
tecplot_writer.options().configure_option('file',cf.URI('file:initial.plt'))
tecplot_writer.execute()

model.simulate()

tecplot_writer.options().configure_option('file',cf.URI('file:final.plt'))
tecplot_writer.execute()
