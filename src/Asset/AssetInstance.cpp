/*
* Dream::Asset::AssetInstance
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "AssetInstance.h"

namespace Dream {
namespace Asset {

	AssetInstance::AssetInstance(AssetDefinition* definition) {
		mDefinition = definition;
	}
	
	std::string AssetInstance::getName() {
		return mDefinition->getName();
	}
	
	std::string AssetInstance::getUUID() {
		return mDefinition->getUUID();
	}
	
	std::string AssetInstance::getNameAndUUIDString() {
		return mDefinition->getNameAndUUIDString();
	}
	
	Scene::SceneObject* AssetInstance::getParentSceneObject() {
		return mParent;
	}
	
	void AssetInstance::setParentSceneObject(Scene::SceneObject *parent) {
		mParent = parent;
	}

} // End of Asset
} // End of Dream
