#include "Framework.h"

Material::Material(wstring shaderFile)
{
    diffuseMap = Texture::Add(L"Resources/Textures/Colors/White.png", L"DM");
    specularMap = Texture::Add(L"Resources/Textures/Colors/White.png", L"SM");
    normalMap = Texture::Add(L"Resources/Textures/Colors/White.png", L"NM");

	SetShader(shaderFile);

	buffer = new MaterialBuffer();

    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;
}

Material::~Material()
{
	delete buffer;
}

void Material::Set()
{
	buffer->SetPS(0);

	diffuseMap->PSSet(0);
	specularMap->PSSet(1);
	normalMap->PSSet(2);

	vertexShader->Set();
	pixelShader->Set();	
}

void Material::Save(string path)
{
    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    tinyxml2::XMLElement* material = document->NewElement("Material");
    material->SetAttribute("Name", name.c_str());
    document->InsertFirstChild(material);

    tinyxml2::XMLElement* shader = document->NewElement("Shader");
    string shaderFile;
    if (vertexShader)
        shaderFile = Utility::ToString(vertexShader->GetFile());
    shader->SetAttribute("Vertex", shaderFile.c_str());
    if (pixelShader)
        shaderFile = Utility::ToString(pixelShader->GetFile());
    shader->SetAttribute("Pixel", shaderFile.c_str());
    material->InsertFirstChild(shader);

    tinyxml2::XMLElement* texture = document->NewElement("Texture");

    tinyxml2::XMLElement* map = document->NewElement("DiffuseMap");
    string textureFile = Utility::ToString(diffuseMap->GetFile());
    map->SetAttribute("Diffuse", textureFile.c_str());
    texture->InsertEndChild(map);
    map = document->NewElement("SpecularMap");
    textureFile = Utility::ToString(specularMap->GetFile());
    map->SetAttribute("Specular", textureFile.c_str());
    texture->InsertEndChild(map);
    map = document->NewElement("NormalMap");
    textureFile = Utility::ToString(normalMap->GetFile());
    map->SetAttribute("Normal", textureFile.c_str());
    texture->InsertEndChild(map);
    material->InsertEndChild(texture);

    tinyxml2::XMLElement* property = document->NewElement("Property");
    tinyxml2::XMLElement* diffuse = document->NewElement("Diffuse");
    diffuse->SetAttribute("R", buffer->GetData()->diffuse.x);
    diffuse->SetAttribute("G", buffer->GetData()->diffuse.y);
    diffuse->SetAttribute("B", buffer->GetData()->diffuse.z);
    diffuse->SetAttribute("A", buffer->GetData()->diffuse.w);
    property->InsertEndChild(diffuse);

    tinyxml2::XMLElement* specular = document->NewElement("Specular");
    specular->SetAttribute("R", buffer->GetData()->specular.x);
    specular->SetAttribute("G", buffer->GetData()->specular.y);
    specular->SetAttribute("B", buffer->GetData()->specular.z);
    specular->SetAttribute("A", buffer->GetData()->specular.w);
    property->InsertEndChild(specular);

    tinyxml2::XMLElement* ambient = document->NewElement("Ambient");
    ambient->SetAttribute("R", buffer->GetData()->ambient.x);
    ambient->SetAttribute("G", buffer->GetData()->ambient.y);
    ambient->SetAttribute("B", buffer->GetData()->ambient.z);
    ambient->SetAttribute("A", buffer->GetData()->ambient.w);
    property->InsertEndChild(ambient);

    tinyxml2::XMLElement* emissive = document->NewElement("Emissive");
    emissive->SetAttribute("R", buffer->GetData()->emissive.x);
    emissive->SetAttribute("G", buffer->GetData()->emissive.y);
    emissive->SetAttribute("B", buffer->GetData()->emissive.z);
    emissive->SetAttribute("A", buffer->GetData()->emissive.w);
    property->InsertEndChild(emissive);

    property->SetAttribute("Shininess", buffer->GetData()->shininess);
    property->SetAttribute("HasNormalMap", buffer->GetData()->hasNormalMap);

    material->InsertEndChild(property);

    document->SaveFile(path.c_str());

    delete document;
}

void Material::Load(string path)
{
    this->file = path;

    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    document->LoadFile(file.c_str());
    tinyxml2::XMLElement* material = document->FirstChildElement();
    name = material->Attribute("Name");

    tinyxml2::XMLElement* shader = material->FirstChildElement();
    wstring shaderFile = Utility::ToWString(shader->Attribute("Vertex"));    
    vertexShader = Shader::AddVS(shaderFile);
    shaderFile = Utility::ToWString(shader->Attribute("Pixel"));    
    pixelShader = Shader::AddPS(shaderFile);

    tinyxml2::XMLElement* texture = shader->NextSiblingElement();
    tinyxml2::XMLElement* map = texture->FirstChildElement();
    wstring textureFile = Utility::ToWString(map->Attribute("Diffuse"));
    SetDiffuseMap(textureFile);
    map = map->NextSiblingElement();
    textureFile = Utility::ToWString(map->Attribute("Specular"));
    SetSpecularMap(textureFile);
    map = map->NextSiblingElement();
    textureFile = Utility::ToWString(map->Attribute("Normal"));
    SetNormalMap(textureFile);

    tinyxml2::XMLElement* property = texture->NextSiblingElement();
    tinyxml2::XMLElement* diffuse = property->FirstChildElement();
    buffer->GetData()->diffuse.x = diffuse->FloatAttribute("R");
    buffer->GetData()->diffuse.y = diffuse->FloatAttribute("G");
    buffer->GetData()->diffuse.z = diffuse->FloatAttribute("B");
    buffer->GetData()->diffuse.w = diffuse->FloatAttribute("A");

    tinyxml2::XMLElement* specular = diffuse->NextSiblingElement();
    buffer->GetData()->specular.x = specular->FloatAttribute("R");
    buffer->GetData()->specular.y = specular->FloatAttribute("G");
    buffer->GetData()->specular.z = specular->FloatAttribute("B");
    buffer->GetData()->specular.w = specular->FloatAttribute("A");

    tinyxml2::XMLElement* ambient = specular->NextSiblingElement();
    buffer->GetData()->ambient.x = ambient->FloatAttribute("R");
    buffer->GetData()->ambient.y = ambient->FloatAttribute("G");
    buffer->GetData()->ambient.z = ambient->FloatAttribute("B");
    buffer->GetData()->ambient.w = ambient->FloatAttribute("A");

    tinyxml2::XMLElement* emissive = ambient->NextSiblingElement();
    buffer->GetData()->emissive.x = emissive->FloatAttribute("R");
    buffer->GetData()->emissive.y = emissive->FloatAttribute("G");
    buffer->GetData()->emissive.z = emissive->FloatAttribute("B");
    buffer->GetData()->emissive.w = emissive->FloatAttribute("A");

    buffer->GetData()->shininess = property->FloatAttribute("Shininess");
    buffer->GetData()->hasNormalMap = property->IntAttribute("HasNormalMap");

    delete document;
}

void Material::Edit()
{
	string title = name + "_Material";

	if (ImGui::TreeNode(title.c_str()))
	{
		char str[128];
		strcpy_s(str, 128, editName.c_str());
		ImGui::InputText("Name", str, 128);
		editName = str;

        string vertexPath = Utility::ToString(vertexShader->GetFile());
        ImGui::Text(vertexPath.c_str());
        string pixelPath = Utility::ToString(pixelShader->GetFile());
        ImGui::Text(pixelPath.c_str());
        SelectShader();

		ImGui::ColorEdit4("Diffuse", (float*)&buffer->GetData()->diffuse);
		ImGui::ColorEdit4("Specular", (float*)&buffer->GetData()->specular);
		ImGui::ColorEdit4("Ambient", (float*)&buffer->GetData()->ambient);
        ImGui::ColorEdit4("Emissive", (float*)&buffer->GetData()->emissive);

        ImGui::SliderFloat("Shininess", &buffer->GetData()->shininess, 1, 50);

        SelectMap("DM", DIFFUSE);
        ImGui::SameLine();
        UnselectMap(DIFFUSE);
        SelectMap("SM", SPECULAR);
        ImGui::SameLine();
        UnselectMap(SPECULAR);
        SelectMap("NM", NORMAL);
        ImGui::SameLine();
        UnselectMap(NORMAL);

        SaveDialog();
        LoadDialog();

		ImGui::TreePop();
	}
}

void Material::SetShader(wstring shaderFile)
{
	vertexShader = Shader::AddVS(shaderFile);
	pixelShader = Shader::AddPS(shaderFile);
}

void Material::SetVertexShader(wstring shaderFile)
{
	vertexShader = Shader::AddVS(shaderFile);
}

void Material::SetPixelShader(wstring shaderFile)
{
	pixelShader = Shader::AddPS(shaderFile);
}

void Material::SetDiffuseMap(wstring diffuseFile)
{
    if (diffuseFile.size() > 0)
        diffuseMap = Texture::Add(diffuseFile);
    else
        diffuseMap = Texture::Add(L"Resources/Textures/Colors/White.png", L"DM");
}

void Material::SetSpecularMap(wstring specularFile)
{
    if (specularFile.size() > 0)
        specularMap = Texture::Add(specularFile);
    else
        specularMap = Texture::Add(L"Resources/Textures/Colors/White.png", L"SM");
}

void Material::SetNormalMap(wstring normalFile)
{
    if (normalFile.length() > 0)
    {
        normalMap = Texture::Add(normalFile);
        buffer->GetData()->hasNormalMap = 1;
    }
    else
    {        
        normalMap = Texture::Add(L"Resources/Textures/Color/White.png", L"NM");
        buffer->GetData()->hasNormalMap = 0;
    }
}

void Material::SelectMap(string name, MapType type)
{
	ImGui::SetWindowFontScale(2.0f);
	ImGui::TextColored(ImVec4(1, 0.5f, 0.8f, 1), name.c_str());
	ImGui::SetWindowFontScale(1.0f);
	ImGui::SameLine();

    void* textureID = nullptr;

    switch (type)
    {
    case Material::DIFFUSE:
        textureID = (ImTextureID*)diffuseMap->GetSRV();
        break;
    case Material::SPECULAR:
        textureID = (ImTextureID*)specularMap->GetSRV();
        break;
    case Material::NORMAL:
        textureID = (ImTextureID*)normalMap->GetSRV();
        break;
    default:
        break;
    }

    if (ImGui::ImageButton(name.c_str(), (ImTextureID)textureID, ImVec2(50, 50)))
    {
        DIALOG->OpenDialog(this->name + name, name, ".png,.jpg,.tga");
    }

    if (DIALOG->Display(this->name + name))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            char path[128];
            GetCurrentDirectoryA(128, path);

            file = file.substr(strlen(path) + 1, file.length());

            switch (type)
            {
            case Material::DIFFUSE:
                SetDiffuseMap(Utility::ToWString(file));
                break;
            case Material::SPECULAR:
                SetSpecularMap(Utility::ToWString(file));
                break;
            case Material::NORMAL:
                SetNormalMap(Utility::ToWString(file));
                break;
            default:
                break;
            }
        }

        DIALOG->Close();
    }
}

void Material::UnselectMap(MapType type)
{
    void* textureID = Texture::Add(L"Resources/Textures/UI/Cancel.png")->GetSRV();

    string key;
    switch (type)
    {
    case Material::DIFFUSE:        
        key = "DMCancel";
        break;
    case Material::SPECULAR:        
        key = "SMCancel";
        break;
    case Material::NORMAL:        
        key = "NMCancel";
        break;
    }

    if (ImGui::ImageButton(key.c_str(), (ImTextureID)textureID, ImVec2(20, 20)))
    {
        switch (type)
        {
        case Material::DIFFUSE:
            SetDiffuseMap(L"");
            break;
        case Material::SPECULAR:
            SetSpecularMap(L"");
            break;
        case Material::NORMAL:
            SetNormalMap(L"");
            break;
        default:
            break;
        }
    }
}

void Material::SelectShader()
{
    string key = name +  "Shader";

    if (ImGui::Button(key.c_str()))
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".hlsl");

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            string shaderPath = "Resources/Shaders/";
            file = file.substr(projectPath.size() + shaderPath.size() + 1, file.size());

            SetShader(Utility::ToWString(file));
        }

        DIALOG->Close();
    }
}

void Material::SaveDialog()
{
    string key = "Save";

    if (ImGui::Button("Save"))
    {
        if (file.empty())
            Save("Resources/Materials/" + name + ".mat");
        else
            Save(file);
    }

    ImGui::SameLine();

    if (ImGui::Button("SaveAs"))
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".mat");

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Save(file);
        }

        DIALOG->Close();
    }
}

void Material::LoadDialog()
{
    string key = "Load";

    if (ImGui::Button("Load"))
    {
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".mat");
    }

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Load(file);
        }

        DIALOG->Close();
    }
}
