/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include "Binding_Mat.h"
#include <functional>
#include <pybind11/operators.h>

#define BINDING_MAT_MAKE_NAME(R, C)                                            \
    std::string(std::string("Mat") + std::to_string(R) + "x" + std::to_string(C))

using namespace sofa::type;

namespace pyMat {
template <sofa::Size R, sofa::Size C>
std::string __str__(const Mat<R, C, double> &self, bool repr) {
    std::string s = (repr) ? (BINDING_MAT_MAKE_NAME(R, C) + "(") : ("(");
    s += pyVec::__str__(self[0]);
    for (size_t i = 1; i < self.size(); ++i)
        s += std::string(", ") + pyVec::__str__(self[i]);
    s += ")";
    return s;
}
} // namespace pyMat

template <sofa::Size R, sofa::Size C>
static void bindSquaredMat(py::class_<Mat<R, C, double>> & /*p*/) {}

template <sofa::Size S> static void bindSquaredMat(py::class_<Mat<S, S, double>> &p) {
    typedef Mat<S, S, double> MatClass;
    p.def("identity", [](MatClass &mat){ return mat.identity(); });
    p.def_static("Identity", []() { return MatClass::Identity(); });
    p.def("transpose", (void (MatClass::*)()) & MatClass::transpose);
    p.def("inverted", [](MatClass &mat){ return mat.inverted(); });
    p.def("invert", [](MatClass &dest, MatClass &from){ return dest.invert(from); });
}

template <sofa::Size R, sofa::Size C>
static void addMat(py::module & /*m*/, py::class_<Mat<R, C, double>> &p) {
    typedef Mat<R, C, double> MatClass;
    typedef Mat<C, R, double> TMat;
    typedef typename MatClass::Line Row;
    p.def(py::init());
    p.def(py::init<const MatClass &>());
    p.def_property_readonly("rows", &MatClass::getNbLines);
    p.def_property_readonly("cols", &MatClass::getNbCols);
    p.def("clear", &MatClass::clear);
    p.def("fill", &MatClass::fill, "r"_a);
    p.def("row", &MatClass::line, "r"_a);
    p.def("col", &MatClass::col, "r"_a);
    p.def("transpose", (void (MatClass::*)(const TMat &)) & MatClass::transpose,
          "m"_a);
    p.def("transposed", &MatClass::transposed);

    // Line bracket accessor
    p.def("__getitem__",
          [](const MatClass &mat, sofa::Index i) {
        if (i >= mat.size())
            throw py::index_error();
        return mat[i];
    });
    p.def("__setitem__", [](MatClass &mat, sofa::Index i, Row value) {
        if (i >= mat.size())
            throw py::index_error();
        Row &val = mat[i];
        val = value;
        return val;
    });

    // cell bracket accessor
    p.def("__getitem__",
          [](const MatClass &mat, sofa::Index i, sofa::Index j) {
        if (i >= mat.size())
            throw py::index_error();
        return mat(i, j);
    });
    p.def("__setitem__", [](MatClass &mat, sofa::Index i, sofa::Index j, double value) {
        if (i >= mat.size())
            throw py::index_error();
        double &val = mat(i, j);
        val = value;
        return val;
    });

    /// Iterator protocol - for line in mat:
    static sofa::Index value = 0;
    p.def("__iter__", [](MatClass &mat) {
        value = 0;
        return mat;
    });
    p.def("__next__", [](MatClass &mat) {
        if (value == mat.nbLines)
            throw py::stop_iteration();
        else
            return mat[value++];
        return mat[value];
    });

    p.def_static("canSelfTranspose", [](const MatClass &lhs, const TMat &rhs) {
        return MatClass::canSelfTranspose(lhs, rhs);
    });
    p.def("isSymmetric", &MatClass::isSymmetric);
    p.def("isDiagonal", &MatClass::isDiagonal);
    p.def("isTransform", &MatClass::isTransform);

    p.def(py::self == py::self);
    p.def(py::self != py::self);

    // linear algebra
    p.def(py::self + py::self);
    p.def(py::self - py::self);

    p.def(-py::self);

    p.def("__mul__", [](const MatClass &self, const Row &v) { return self * v; });

    p.def("multDiagonal", &MatClass::multDiagonal, "d"_a);

    //<< how should I bind template param P on those methods?
    //  p.def(py::self * py::self); //<< how should I bind template param P?
    //  p.def("multTranspose",
    //        (Row(MatClass::*)(const Col &) const) & MatClass::multTranspose,
    //        "v"_a);
    //  p.def("multTransposed",
    //        (Mat<C, P, double>(MatClass::*)(const Mat<L, P, double> &) const) &
    //            MatClass::multTransposed,
    //        "m"_a);

    p.def("plusTransposed", &MatClass::plusTransposed);
    p.def("minusTransposed", &MatClass::minusTransposed);
    p.def("addTransposed", &MatClass::addTransposed);
    p.def("subTransposed", &MatClass::subTransposed);

    p.def(py::self * double());
    p.def(double() * py::self);
    p.def(py::self / double());
    p.def(py::self *= double());
    p.def(py::self /= double());

    p.def(py::self += py::self);
    p.def(py::self -= py::self);

    // bindings to generic squared matrices methods
    bindSquaredMat(p);

    p.def("__str__", [](MatClass &self) { return pyMat::__str__(self); });
    p.def("__repr__", [](MatClass &self) { return pyMat::__str__(self, true); });
}

// Generic bindings for Matrices
template <sofa::Size R, sofa::Size C> struct MATRIX {
    static void addMat(py::module &m) {
        typedef Mat<R, C, double> MatClass;
        py::class_<MatClass> p(m, BINDING_MAT_MAKE_NAME(R, C).c_str());
        ::addMat(m, p);
    }
};

// Prevent bindings with L | C == 0
template <sofa::Size C> struct MATRIX<0, C> {
    static void addMat(py::module & /*m*/) {}
};
template <sofa::Size R> struct MATRIX<R, 0> {
    static void addMat(py::module & /*m*/) {}
};

template <> struct MATRIX<1, 1> {
    static void addMat(py::module &m) {
        typedef Mat<1, 1, double> MatClass;
        //    typedef typename MatClass::Line Row;
        //    typedef typename MatClass::Col Col;

        py::class_<MatClass> p(m, BINDING_MAT_MAKE_NAME(1, 1).c_str());
        p.def(py::init([](py::list l) {
                  MatClass *mat = new MatClass(sofa::type::NOINIT);
                  if (py::isinstance<py::list>(l[0])) // 2D array
                  {
                      for (sofa::Size i = 0; i < MatClass::nbLines; ++i) {
                          py::list r = l[i];
                          for (sofa::Size j = 0; j < MatClass::nbCols; ++j) {
                              (*mat)[i][j] = double(r[j].cast<py::float_>());
                          }
                      }
                  } else // process list as 1D ptr
                  {
                      for (sofa::Size i = 0; i < MatClass::N; ++i)
                      mat->ptr()[i] = double(l[i].cast<py::float_>());
                  }
                  return std::unique_ptr<MatClass>(mat);
              }));
        ::addMat(m, p);
    }
};

template <> struct MATRIX<2, 2> {
    static void addMat(py::module &m) {
        typedef Mat<2, 2, double> MatClass;
        typedef typename MatClass::Line Row;
        //    typedef typename MatClass::Col Col;

        py::class_<MatClass> p(m, BINDING_MAT_MAKE_NAME(2, 2).c_str());
        p.def(py::init<Row, Row>());
        p.def(py::init([](py::list l) {
                  MatClass *mat = new MatClass(sofa::type::NOINIT);
                  if (py::isinstance<py::list>(l[0])) // 2D array
                  {
                      for (sofa::Size i = 0; i < MatClass::nbLines; ++i) {
                          py::list r = l[i];
                          for (sofa::Size j = 0; j < MatClass::nbCols; ++j) {
                              (*mat)[i][j] = double(r[j].cast<py::float_>());
                          }
                      }
                  } else // process list as 1D ptr
                  {
                      for (sofa::Size i = 0; i < MatClass::N; ++i)
                      mat->ptr()[i] = double(l[i].cast<py::float_>());
                  }
                  return std::unique_ptr<MatClass>(mat);
              }));
        ::addMat(m, p);
    }
};

template <> struct MATRIX<3, 3> {
    static void addMat(py::module &m) {
        typedef Mat<3, 3, double> MatClass;
        typedef typename MatClass::Line Row;
        //    typedef typename MatClass::Col Col;

        py::class_<MatClass> p(m, BINDING_MAT_MAKE_NAME(3, 3).c_str());
        p.def(py::init<Row, Row, Row>());
        p.def(py::init([](py::list l) {
                  MatClass *mat = new MatClass(sofa::type::NOINIT);
                  if (py::isinstance<py::list>(l[0])) // 2D array
                  {
                      for (sofa::Size i = 0; i < MatClass::nbLines; ++i) {
                          py::list r = l[i];
                          for (sofa::Size j = 0; j < MatClass::nbCols; ++j) {
                              (*mat)[i][j] = double(r[j].cast<py::float_>());
                          }
                      }
                  } else // process list as 1D ptr
                  {
                      for (sofa::Size i = 0; i < MatClass::N; ++i)
                      mat->ptr()[i] = double(l[i].cast<py::float_>());
                  }
                  return std::unique_ptr<MatClass>(mat);
              }));
        ::addMat(m, p);
    }
};

template <> struct MATRIX<4, 4> {
    static void addMat(py::module &m) {
        typedef Mat<4, 4, double> MatClass;
        typedef typename MatClass::Line Row;
        //    typedef typename MatClass::Col Col;

        py::class_<MatClass> p(m, BINDING_MAT_MAKE_NAME(4, 4).c_str());
        p.def(py::init<Row, Row, Row, Row>());
        p.def(py::init([](py::list l) {
                  MatClass *mat = new MatClass(sofa::type::NOINIT);
                  if (py::isinstance<py::list>(l[0])) // 2D array
                  {
                      for (sofa::Size i = 0; i < MatClass::nbLines; ++i) {
                          py::list r = l[i];
                          for (sofa::Size j = 0; j < MatClass::nbCols; ++j) {
                              (*mat)[i][j] = double(r[j].cast<py::float_>());
                          }
                      }
                  } else // process list as 1D ptr
                  {
                      for (sofa::Size i = 0; i < MatClass::N; ++i)
                      mat->ptr()[i] = double(l[i].cast<py::float_>());
                  }
                  return std::unique_ptr<MatClass>(mat);
              }));
        ::addMat(m, p);
    }
};

template <> struct MATRIX<3, 4> {
    static void addMat(py::module &m) {
        typedef Mat<3, 4, double> MatClass;
        typedef typename MatClass::Line Row;
        //    typedef typename MatClass::Col Col;

        py::class_<MatClass> p(m, BINDING_MAT_MAKE_NAME(3, 4).c_str());
        p.def(py::init<Row, Row, Row>());
        p.def(py::init([](py::list l) {
                  MatClass *mat = new MatClass(sofa::type::NOINIT);
                  if (py::isinstance<py::list>(l[0])) // 2D array
                  {
                      for (sofa::Size i = 0; i < MatClass::nbLines; ++i) {
                          py::list r = l[i];
                          for (sofa::Size j = 0; j < MatClass::nbCols; ++j) {
                              (*mat)[i][j] = double(r[j].cast<py::float_>());
                          }
                      }
                  } else // process list as 1D ptr
                  {
                      for (sofa::Size i = 0; i < MatClass::N; ++i)
                      mat->ptr()[i] = double(l[i].cast<py::float_>());
                  }
                  return std::unique_ptr<MatClass>(mat);
              }));
        ::addMat(m, p);
    }
};
void moduleAddMat(py::module &m) {
    MATRIX<1, 1>::addMat(m);
    MATRIX<2, 2>::addMat(m);
    MATRIX<3, 3>::addMat(m);
    MATRIX<3, 4>::addMat(m);
    MATRIX<4, 4>::addMat(m);
}
