#pragma once
#include <vector>
#include "Vector2.h"
#include "Window.h"

using namespace std;

class Shape2D {
protected:
	static void RatioCorrection(Vector2& shapeDimension){
		float ratio = Window::Dimension.x / Window::Dimension.y;
		if (ratio > 1) {
			shapeDimension.y*= ratio;
		}
		else {
			shapeDimension.x *= ratio;
		}
	}
public:
	static inline void CreateRectangle(vector<float>& vertices, Vector2 center, Vector2 dimensions) {
		RatioCorrection(dimensions);

		vertices.push_back(center.x - dimensions.x / 2);
		vertices.push_back(center.y + dimensions.y / 2);
		vertices.push_back(0);

		vertices.push_back(center.x - dimensions.x / 2);
		vertices.push_back(center.y - dimensions.y / 2);
		vertices.push_back(0);

		vertices.push_back(center.x + dimensions.x / 2);
		vertices.push_back(center.y + dimensions.y / 2);
		vertices.push_back(0);

		vertices.push_back(center.x - dimensions.x / 2);
		vertices.push_back(center.y - dimensions.y / 2);
		vertices.push_back(0);

		vertices.push_back(center.x + dimensions.x / 2);
		vertices.push_back(center.y + dimensions.y / 2);
		vertices.push_back(0);

		vertices.push_back(center.x + dimensions.x / 2);
		vertices.push_back(center.y - dimensions.y / 2);
		vertices.push_back(0);
	}



	static inline void CreateEllipse(vector<float>& vertices, Vector2 center, Vector2 dimensions, int pointsCount)
	{
		RatioCorrection(dimensions);
		float theta = 2.0f * 3.1415926f / pointsCount;
		float cosine = cosf(theta);
		float sine = sinf(theta);


		float x = 1;//we start at angle = 0 
		float y = 0;
		float t = x;

		for (int vert = 0; vert < pointsCount; vert++)
		{
			//apply radius and offset
			vertices.push_back(x * dimensions.x + center.x);
			vertices.push_back(y * dimensions.y + center.y);

			//apply the rotation matrix
			t = x;
			x = cosine * x - sine * y;
			y = sine * t + cosine * y;
		}
	}


	static inline void CreateCircle(vector<float>& vertices, Vector2 center, float radius, int pointsCount) {
		CreateEllipse(vertices, center, Vector2(radius, radius), pointsCount);
	}

	static inline int DrawRectangle(int startIndex) {
		//We will always be drawing 4 vertices
		glDrawArrays(GL_TRIANGLE_STRIP, startIndex, 6);
		return startIndex + 6;
	}
	static inline int DrawEllipse(int startIndex, int vertexCount) {
		//We use the GL_TRIANGLE_FAN to create triangle that have one point (the first) in common
		//We draw from vertex 0 and we will be drawing 20 vertices
		glDrawArrays(GL_TRIANGLE_FAN, startIndex, vertexCount);
		return startIndex + vertexCount;
	}

	static inline int DrawCircle(int startIndex, int vertexCount) {
		DrawEllipse(startIndex, vertexCount);
		return startIndex + vertexCount;
	}



};