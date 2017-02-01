#pragma once
#include "GameObject.h"

class Wall : public GameObject
{
public:
	Wall(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label = ID_UNKNOWN, POINT2f const & position = { 0, 0 });
	~Wall();

	// Implement inherited private implementation
	void input_impl() override;
	void draw_impl(Canvas & c) override;
	void update_impl(float const & dt) override;
};

using WALL_PTR = std::shared_ptr<Wall>;

