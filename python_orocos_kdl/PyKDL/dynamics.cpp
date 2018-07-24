//Copyright  (C)  2007  Ruben Smits <ruben dot smits at mech dot kuleuven dot be>
//
//Version: 1.0
//Author: Ruben Smits <ruben dot smits at mech dot kuleuven dot be>
//Author: Zihan Chen <zihan dot chen dot jhu at gmail dot com>
//Maintainer: Ruben Smits <ruben dot smits at mech dot kuleuven dot be>
//URL: http://www.orocos.org/kdl
//
//This library is free software; you can redistribute it and/or
//modify it under the terms of the GNU Lesser General Public
//License as published by the Free Software Foundation; either
//version 2.1 of the License, or (at your option) any later version.
//
//This library is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//Lesser General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public
//License along with this library; if not, write to the Free Software
//Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


#include <iostream>
#include <iomanip>
#include <kdl/chaindynparam.hpp>
#include <kdl/jntspaceinertiamatrix.hpp>
#include "PyKDL.h"

namespace py = pybind11;
using namespace KDL;


void init_dynamics(pybind11::module &m)
{
    py::class_<JntSpaceInertiaMatrix>(m, "JntSpaceInertiaMatrix")
            .def(py::init<>())
            .def(py::init<int>())
            .def(py::init<const JntSpaceInertiaMatrix&>())
            .def("resize", &JntSpaceInertiaMatrix::resize)
            .def("rows", &JntSpaceInertiaMatrix::rows)
            .def("columns", &JntSpaceInertiaMatrix::columns)
            .def("__getitem__", [](const JntSpaceInertiaMatrix &jm, std::tuple<int, int> idx) {
                int i = std::get<0>(idx);
                int j = std::get<1>(idx);
                if (i < 0 || i > jm.rows() || j < 0 || j > jm.columns()) {
                    throw py::index_error("Inertia index out of range");
                }
                return jm((unsigned int)i, (unsigned int)j);
            })
            .def("__repr__", [](const JntSpaceInertiaMatrix &jm) {
                std::ostringstream oss;
                for (size_t r = 0; r < jm.rows(); r++) {
                    for (size_t c = 0; c < jm.columns(); c++) {
                        oss << std::setw(KDL_FRAME_WIDTH) << jm(r, c);
                    }
                    oss << std::endl;
                }
                return oss.str();
            })
            .def(py::self == py::self);
    m.def("Add", (void (*)(const JntSpaceInertiaMatrix&, const JntSpaceInertiaMatrix&, JntSpaceInertiaMatrix&)) &KDL::Add);
    m.def("Subtract", (void (*)(const JntSpaceInertiaMatrix&, const JntSpaceInertiaMatrix&,JntSpaceInertiaMatrix&)) &KDL::Subtract);
    m.def("Multiply", (void (*)(const JntSpaceInertiaMatrix&, const double&, JntSpaceInertiaMatrix&)) &KDL::Multiply);
    m.def("Divide", (void (*)(const JntSpaceInertiaMatrix&, const double&, JntSpaceInertiaMatrix&)) &KDL::Divide);
    m.def("Multiply", (void (*)(const JntSpaceInertiaMatrix&, const JntArray&, JntArray&)) &KDL::Multiply);
    m.def("SetToZero", (void (*)(JntSpaceInertiaMatrix&)) &KDL::SetToZero);
    m.def("Equal", (bool (*)(const JntSpaceInertiaMatrix&, const JntSpaceInertiaMatrix&, double)) &KDL::Equal,
          py::arg("src1"), py::arg("src2"), py::arg("eps")=epsilon);

    py::class_<ChainDynParam>(m, "ChainDynParam")
            .def(py::init<const Chain&, Vector>())
            .def("JntToCoriolis", &ChainDynParam::JntToCoriolis)
            .def("JntToMass", &ChainDynParam::JntToMass)
            .def("JntToGravity", &ChainDynParam::JntToGravity);
}