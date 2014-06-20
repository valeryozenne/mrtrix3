/*
    Copyright 2008 Brain Research Institute, Melbourne, Australia

    Written by J-Donald Tournier, 27/06/08.

    This file is part of MRtrix.

    MRtrix is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MRtrix is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MRtrix.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "command.h"
#include "timer.h"
#include "math/constrained_least_squares.h"


using namespace MR;
using namespace App;




void usage ()
{

  DESCRIPTION
    + "test ICLS";

  ARGUMENTS
    + Argument ("problem", "the problem matrix.").type_file()
    + Argument ("constraint", "the constraint matrix.").type_file()
    + Argument ("b", "the RHS vector.").type_file();
}






void run ()
{
  Math::Matrix<double> H (argument[0]);
  Math::Matrix<double> A (argument[1]);
  Math::Vector<double> b;
  b.load (argument[2]);

  Math::ICLS3::Problem<double> icls_problem (H, A, 1.0, 10.0, 1.0e8, 1.0e-8);
  Math::ICLS3::Solver<double> icls_solver (icls_problem);

  Math::Vector<double> x;
  Timer timer;
  icls_solver (x, b);
  VAR (timer.elapsed());
  x.save ("x.txt");
}

