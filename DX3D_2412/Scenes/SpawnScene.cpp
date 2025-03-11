#include "Framework.h"
#include "SpawnScene.h"

SpawnScene::SpawnScene()
{
	//objects.reserve(SIZE);
	//
	//for (int i = 0; i < SIZE; i++)
	//{
	//	objects.push_back(new Sphere());
	//}

	objects.resize(SIZE);

	//for (int i = 0; i < SIZE; i++)
	//{
	//	objects[i] = new Sphere();
	//}
    for (GameObject<VertexType>*& object : objects)
	{
		object = new GameObject<VertexType>();
		object->SetActive(false);
	}

	LoadTextures();
    LoadMeshes();

    plane = new Plane(Vector2(30, 30), 10, 10);
    plane->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Dirt2.png");
}

SpawnScene::~SpawnScene()
{
    for (GameObject<VertexType>* object : objects)
		delete object;

    delete plane;
}

void SpawnScene::Update()
{
	time += DELTA;

	if (time >= SPAWN_INTERVAL)
	{
		time -= SPAWN_INTERVAL;

		Spawn();
	}
}

void SpawnScene::PreRender()
{
}

void SpawnScene::Render()
{
    for (GameObject<VertexType>* object : objects)
		object->Render();

    plane->Render();
}

void SpawnScene::PostRender()
{
    plane->GetMaterial()->Edit();
}

void SpawnScene::GUIRender()
{
}

void SpawnScene::Spawn()
{
	for (GameObject<VertexType>* object : objects)
	{
		if (!object->IsActive())
		{
			object->SetActive(true);
			object->SetLocalPosition(GameMath::Random(MIN_POS, MAX_POS));
            object->SetLocalScale(GameMath::Random(Vector3(1, 1, 1), Vector3(5, 5, 5)));
			object->GetMaterial()->SetDiffuseMap(textures[GameMath::Random(0, textures.size())]);
            object->SetMesh(meshes[GameMath::Random(0, meshes.size())]);           

			object->UpdateWorld();

			return;
		}
	}
}

void SpawnScene::LoadTextures()
{
	textures.push_back(Texture::Add(L"Resources/Textures/Landscape/Box.png"));
	textures.push_back(Texture::Add(L"Resources/Textures/Landscape/Bricks.png"));
	textures.push_back(Texture::Add(L"Resources/Textures/Landscape/Dirt.png"));
	textures.push_back(Texture::Add(L"Resources/Textures/Landscape/Dirt2.png"));
	textures.push_back(Texture::Add(L"Resources/Textures/Landscape/Dirt3.png"));
}

void SpawnScene::LoadMeshes()
{
    MakeCubeMesh();
    MakeSphereMesh();
}

void SpawnScene::MakeCubeMesh()
{
    Mesh<VertexType>* mesh = new Mesh<VertexType>();

    vector<VertexType>& vertices = mesh->GetVertices();

    Vector3 size(1, 1, 1);

    //Front
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 0);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 10, 0);//1
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, 10);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 10, 10);//3

    //Right
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 0);//1
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 1, 0);//5
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, 1);//3
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 1, 1);//7

    //Up
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 0, 0);//4
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 1, 0);//5
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 1);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 1, 1);//1

    //Back
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 0, 0);//4    
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 0, 1);//6
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 1, 0);//5
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 1, 1);//7

    //Left
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 0);//0    
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, 1);//2
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 1, 0);//4
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 1, 1);//6

    //Down
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 0, 0);//6    
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, 1);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 1, 0);//7
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 1, 1);//3

    vector<UINT>& indices = mesh->GetIndices();
    indices = {
        //Front
        0, 1, 2, 2, 1, 3,
        //Right
        4, 5, 6, 6, 5, 7,
        //Up
        8, 9, 10, 10, 9, 11,
        //Back
        12, 13, 14, 14, 13, 15,
        //Left
        16, 17, 18, 18, 17, 19,
        //Down
        20, 21, 22, 22, 21, 23
    };

    mesh->CreateMesh();

    meshes.push_back(mesh);
}

void SpawnScene::MakeSphereMesh()
{
    Mesh<VertexType>* mesh = new Mesh<VertexType>();    

    UINT sliceCount = 32;
    UINT stackCount = 16;

    float radius = 1.0f;

    float thetaStep = XM_2PI / sliceCount;
    float phiStep = XM_PI / stackCount;

    vector<VertexType>& vertices = mesh->GetVertices();

    vertices.reserve((sliceCount + 1) * (stackCount + 1));

    for (UINT i = 0; i <= stackCount; i++)
    {
        float phi = i * phiStep;

        for (int j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;

            VertexType vertex;

            vertex.pos.x = sin(phi) * cos(theta);
            vertex.pos.y = cos(phi);
            vertex.pos.z = sin(phi) * sin(theta);

            vertex.pos = Vector3(vertex.pos) * radius;

            vertex.uv.x = (float)j / sliceCount;
            vertex.uv.y = (float)i / stackCount;

            vertices.push_back(vertex);
        }
    }

    vector<UINT>& indices = mesh->GetIndices();

    indices.reserve(sliceCount * stackCount * 6);

    for (UINT z = 0; z < stackCount; z++)
    {
        for (UINT x = 0; x < sliceCount; x++)
        {
            indices.push_back((sliceCount + 1) * z + x);//0
            indices.push_back((sliceCount + 1) * z + x + 1);//3
            indices.push_back((sliceCount + 1) * (z + 1) + x);//1

            indices.push_back((sliceCount + 1) * (z + 1) + x);//1            
            indices.push_back((sliceCount + 1) * z + x + 1);//3
            indices.push_back((sliceCount + 1) * (z + 1) + x + 1);//2
        }
    }

    mesh->CreateMesh();

    meshes.push_back(mesh);
}
