#pragma once

template <typename T>
class PoolingManager
{
public:
	PoolingManager() = default;
	~PoolingManager();

	void Update();
	void Render();

	void Add(T* object);
	T* Pop();

	void Create(UINT size);

private:
	vector<T*> objects;

	int curIndex = 0;
};

template<typename T>
inline PoolingManager<T>::~PoolingManager()
{
	for (T* object : objects)
		delete object;

	objects.clear();
}

template<typename T>
inline void PoolingManager<T>::Update()
{
	for (T* object : objects)
		object->Update();
}

template<typename T>
inline void PoolingManager<T>::Render()
{
	for (T* object : objects)
		object->Render();
}

template<typename T>
inline void PoolingManager<T>::Add(T* object)
{
	objects.push(object);
}

template<typename T>
inline T* PoolingManager<T>::Pop()
{	
	T* curObject = objects[curIndex];

	curIndex = ++curIndex % objects.size();

	return curObject;
}

template<typename T>
inline void PoolingManager<T>::Create(UINT size)
{
	objects.reserve(size);
	for (int i = 0; i < size; i++)
	{
		T* object = new T();
		object->SetActive(false);
		objects.push_back(object);
	}
}
