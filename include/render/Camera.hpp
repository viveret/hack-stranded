#ifndef SURVIVOR_ENGINE_RENDER_CAMERA
#define SURVIVOR_ENGINE_RENDER_CAMERA

#include "SE/Core.hpp"
#include "SE/math/Vector.hpp"

/*struct SE_Render_Camera
{
	// Matrix<float, 4, 4> 
};*/

namespace SE_Render_Camera
{
	struct Camera;
	typedef Camera* Cam_ID;

	Cam_ID New();

	Vector3f& Position( Cam_ID This );
	float& Zoom( Cam_ID This );

	Vector3f& Rotation( Cam_ID This );

	// Remove previous gl / camera data then apply
	void Switch( Cam_ID This );

	// Apply camera
	void Mult( Cam_ID This );
}


#endif
