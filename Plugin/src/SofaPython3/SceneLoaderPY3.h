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

#include <string>
#include <map>

#include <sofa/simulation/SceneLoaderFactory.h>
#include <sofa/simulation/Visitor.h>
#include <sofa/simulation/Node.h>

#include <SofaPython3/config.h>

namespace sofapython3
{
using sofa::simulation::Node;
using sofa::simulation::SceneLoader;

/// The scene loader for python3 scene files
class SOFAPYTHON3_API SceneLoaderPY3 : public SceneLoader
{
public:
    virtual ~SceneLoaderPY3(){}

    /// Pre-loading check
    virtual bool canLoadFileExtension(const char *extension) override;

    /// Pre-saving check
    virtual bool canWriteFileExtension(const char *extension) override;

    /// load the file
    virtual Node::SPtr doLoad(const std::string& filename, const std::vector<std::string>& sceneArgs=std::vector<std::string>(0)) override;

    void loadSceneWithArguments(const char *filename,
                                const std::vector<std::string>& arguments=std::vector<std::string>(0),
                                Node::SPtr root_out = nullptr);

    /// get the file type description
    virtual std::string getFileTypeDesc() override;

    /// get the list of file extensions
    virtual void getExtensionList(ExtensionList* list) override;

    bool syntaxForAddingRequiredPlugin(const std::string& pluginName, const std::vector<std::string>& listComponents,
                                       std::ostream& ss, sofa::simulation::Node* nodeWhereAdded) override;
};

} // namespace sofapython3
