#pragma once
#include "BaseClasses.h"
#include "Scene.h"

class GameObject : public Sprite, public Dynamic, public Rect, public Controllable, public GameEntity, public std::enable_shared_from_this<GameObject>
{
public:
	GameObject(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label = ID_UNKNOWN, POINT2f const & position = { 0, 0 });
	~GameObject();

	template <class T>
	std::shared_ptr<T> Instantiate(unsigned int _width, unsigned int _height, LABEL_TYPE _label = ID_UNKNOWN);

	// Allow game objects to destroy themselves
	void Destroy();
	bool IsDestroyed();

	int GetWidth();
	int GetHeight();

	VECTOR2f GetCentre();

	void SetWidth(int w);
	void SetHeight(int h);

	// If we should skip physics calculations. The OnCollision event will still be called however
	bool SkipCollisions();
	void SetSkipCollisions(bool _skipCollisions);

	// If the objects is static, it cannot be moved
	bool IsStatic();
	void SetIsStatic(bool _isStatic);

	virtual void OnCollision(GAMEOBJECT_PTR otherGameObject);

protected:
	Scene* scene;
	bool skipCollisions;
	bool isDestroyed;
	bool isStatic;
};

// Instantiate other games objects
template<class T>
inline std::shared_ptr<T> GameObject::Instantiate(unsigned int _width, unsigned int _height, LABEL_TYPE _label)
{
	static_assert(std::is_base_of<GameObject, T>::value, "Can Only instantiate GameObjects");

	std::shared_ptr<T> gameObject = std::make_shared<T>(_width, _height, m_pInput, scene, _label);

	scene->AddEntity(gameObject);

	return gameObject;
}
