/*
* Dream::Resource::Model::Model
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

#ifndef MODEL_H
#define MODEL_H

#include "../Resource.h"

#define RESOURCE_TYPE_MODEL "Model"
#define RESOURCE_DIR_MODEL  "model"

namespace Dream {
	namespace Resource {
		namespace Model {
			class Model : public Resource {
			public:
				Model(void);
				Model(nlohmann::json);
				virtual ~Model(void) {};
			}; // End of Model
		} // End of Model
	}// End of Resource
} // End of Dream

#endif // End of MODEL_H
