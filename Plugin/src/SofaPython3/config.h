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

#pragma once

#include <sofa/config.h>
#include <pybind11/pybind11.h>


/**
 * This macro HAS to be placed in front of function prototypes containing
 * pybind11 symbols. By default pybind exports its attributes as hidden symbols
 * which causes linking against libs using them impossible (undefined refs).
 * It will also do the job of adding the dllexport / dllimport declaration on
 * Windows Systems.
 **/

 // __attribute__(visibility("default")) && __declspec(dllexport)
#ifdef SOFA_BUILD_SOFAPYTHON3
	#define SOFAPYTHON3_API PYBIND11_EXPORT
#else
	#define SOFAPYTHON3_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

// pybind11 already bind the attributeError starting from 2.8.1 version.
// so if the version is >= 2.8.1, macro does nothing, otherwise bind attribute_error
#if not(PYBIND11_VERSION_MAJOR >= 2 && PYBIND11_VERSION_MINOR >= 8 && PYBIND11_VERSION_PATCH >= 1)
#define SOFAPYTHON3_BIND_ATTRIBUTE_ERROR() namespace pybind11 { PYBIND11_RUNTIME_EXCEPTION(attribute_error, PyExc_AttributeError) }
#else
#define SOFAPYTHON3_BIND_ATTRIBUTE_ERROR()
#endif // not(PYBIND11_VERSION >= 2.8.1)
