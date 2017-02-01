#include "GameObject.h"
#include "Scene.h"

GameObject::GameObject(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label, POINT2f const & position)
	: Sprite(position), Dynamic(), Rect(position, POINT2f(position.x + width, position.y + height)), Controllable(ui), GameEntity(label), scene(scene)
{
	skipCollisions = false;
	isDestroyed = false;
	isStatic = false;
}


GameObject::~GameObject()
{
}

void GameObject::Destroy()
{
	scene->MarkObjectForRemoval(GetUID());
	isDestroyed = true;
}

bool GameObject::IsDestroyed()
{
	return isDestroyed;
}


int GameObject::GetWidth()
{

	return (int)(br.x - ul.x);
}
int GameObject::GetHeight()
{
	return (int)(br.y - ul.y);
}

VECTOR2f GameObject::GetCentre()
{
	return VECTOR2f(ul.x + (GetWidth() / 2), ul.y + (GetHeight() / 2));
}

void GameObject::SetWidth(int w)
{
	br.x = ul.x + w;
}
void GameObject::SetHeight(int h)
{
	br.y = ul.y + h;
}

bool GameObject::SkipCollisions()
{
	return skipCollisions;
}

void GameObject::SetSkipCollisions(bool _skipCollisions)
{
	skipCollisions = _skipCollisions;
}

bool GameObject::IsStatic()
{
	return isStatic;
}

void GameObject::SetIsStatic(bool _isStatic)
{
	isStatic = _isStatic;
}

void GameObject::OnCollision(GAMEOBJECT_PTR otherGameObject)
{
}



