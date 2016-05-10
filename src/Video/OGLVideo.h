/*
* OGLVideo
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

#ifndef OGLVIDEO_H
#define OGLVIDEO_H

#include "../Interfaces/VideoInterface.h"
#include <GLFW/glfw3.h>

namespace Dream {
	namespace Video {
		class OGLVideo : public VideoInterface {
		private:
			GLFWwindow *mWindow;
		public:
			OGLVideo(void);
			~OGLVideo(void);
			bool init(void);
		}; // End of OGLVideo
	} // End of Video
} // End of Dream

#endif // End of OGLVIDEO_H
