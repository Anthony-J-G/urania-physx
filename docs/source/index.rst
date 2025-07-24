.. urania-physx documentation master file, created by
   sphinx-quickstart on Sat Apr 26 01:09:45 2025.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Urania Physx
=============
*A physics engine written in C/C++*

This repository provides an exploration of various physics and astrophysics simulations.

The ``examples`` executable currently uses `Raylib <https://github.com/raysan5/raylib>`_ as a backend to handle rendering, sound, input event handling, as well as some other platform specifc functionality. This abstraction layer exists to allow for the main engine library to focus solely on the physics solvers. For more information on the ``raylib`` API, you can reference the `API cheatsheet <https://www.raylib.com/cheatsheet/cheatsheet.html>`_ or the `multitude of examples <https://www.raylib.com/examples.html>`_ hosted on ``raylib``'s official website.



:math:`\sigma_{1}` equals :math:`\sigma_{2}`

.. math::
   \langle \alpha, \beta \rangle
   \in
   \Biggl \lbrace
   {
   M,\text{ if }
      {
      l(\underline{x}) =
         \frac { p(\underline{x}|M ) } { p(\underline{x}|U) }
         \geq
         \frac { p(U) }{ p(M) } }
   \atop
   U, \text{ otherwise }
   }



.. toctree::
   :hidden:
   :maxdepth: 2
   :caption: Administration:

   administration/overview
   administration/style
   administration/design   
   administration/roadmap
   administration/references


.. toctree::
   :hidden:
   :maxdepth: 2
   :caption: Engine:
   
   engine/index
   engine/types
   engine/world


.. toctree::
   :hidden:
   :maxdepth: 2
   :caption: Examples:

   examples/index
   examples/modules
   examples/runtime/index
   examples/editor/index
   examples/scenes/index
   