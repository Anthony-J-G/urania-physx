2D Euler Fluid Simulation
=========================

Overview
--------
This scene contains an implementation of an Eulerian fluid simulation on a flat plane. The name "Eulerian" is derived from the mathematician Leonhard Euler and indicates that the simulation uses a grid-based (aka cell-based) approach. This is in direct contrast to a Lagrangian methodology (named after Joseph-Louis Lagrange) which instead models the particles of the fluid directly.

We assume that the fluid, regardless of being a liquid or gas, is incompressible. Another important assumption is that the fluid is inviscid, viscosity is zero.


Collocated vs. Staggered 
________________________
In a collocated grid, the velocity of the fluid is stored in the center of each cell. 


Classes
-------

.. doxygenclass:: EulerFluid
   :project: physics
   :members:


.. cpp:function:: bool namespaced::theclass::method(int arg1, std::string arg2)

   Describes a method with parameters and types.


