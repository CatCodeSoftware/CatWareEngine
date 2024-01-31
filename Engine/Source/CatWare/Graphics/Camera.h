#pragma once

#include <glm/glm.hpp>

#include "CatWare/Core.h"
#include "CatWare/Types/Vector.h"

namespace CatWare
{
	class Camera
	{
	public:
		inline glm::mat4 GetProjectionMatrix( ) { return projection; }
		inline glm::mat4 GetViewMatrix( ) { return view; }

		virtual void RecalculateViewMatrix( ) = 0;

	protected:
		glm::mat4 projection;
		glm::mat4 view;
	};

	class CATWARE_API OrthoCamera : public Camera
	{
	public:
		OrthoCamera( int screenWidth, int screenHeight );

		inline void SetFocus( Vector2D focus )
		{
			RecalculateViewMatrix( );
			this->focus = focus;
		}
		inline void SetRotation( float rotation ) { this->rotation = rotation; RecalculateViewMatrix( ); }
		inline void SetScale( float scale ) { this->scale = scale; RecalculateViewMatrix( ); }

		inline Vector2D GetFocus( ) { return focus; }
		inline float GetRotation( ) { return rotation; }
		inline float GetScale( ) { return  scale; }

		void RecalculateViewMatrix( ) override;

	private:
		Vector2D focus = { 0, 0 };
		float rotation = 0;
		float scale = 1;
	};
}