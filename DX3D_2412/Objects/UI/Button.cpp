#include "Framework.h"

Button::Button(wstring textureFile)
{
    Texture* texture = Texture::Add(textureFile);
    image = new Quad(textureFile);  
    image->SetParent(this);
    CreateMesh(texture->GetSize());
}

Button::Button(Vector2 size) : RectCollider(size)
{   
    image = new Quad(size);
    image->SetParent(this);
}

Button::~Button()
{
    delete image;
}

void Button::Update()
{
    if (!isActive) return;

    if (IsPointCollision(mousePos))
    {
        if (KEY->Down(VK_LBUTTON))
            isDownCheck = true;

        if (KEY->Press(VK_LBUTTON))
            state = DOWN;
        else
            state = OVER;

        if (isDownCheck && KEY->Up(VK_LBUTTON))
        {
            if (event != nullptr)
                event();

            if (paramEvent != nullptr)
                paramEvent(object);

            isDownCheck = false;
        }
    }
    else
    {
        state = NONE;

        if (KEY->Up(VK_LBUTTON))
            isDownCheck = false;
    }

    switch (state)
    {
    case Button::NONE:
        image->GetMaterial()->GetData()->diffuse = NONE_COLOR;
        break;
    case Button::DOWN:
        image->GetMaterial()->GetData()->diffuse = DOWN_COLOR;
        break;
    case Button::OVER:
        image->GetMaterial()->GetData()->diffuse = OVER_COLOR;
        break;    
    }

    UpdateWorld();
    image->UpdateWorld();
}

void Button::Render()
{
    RectCollider::Render();
    image->Render();
}
