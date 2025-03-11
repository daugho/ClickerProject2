#include "Framework.h"

vector<string> Utility::SplitString(string origin, const string& tok, const bool& includeLast)
{
    vector<string> result;

    size_t cutAt = 0;

    while ((cutAt = origin.find_first_of(tok)) != origin.npos)
    {
        if (cutAt > 0)
            result.push_back(origin.substr(0, cutAt));

        origin = origin.substr(cutAt + 1);
    }

    if (includeLast && origin.size() > 0)
        result.push_back(origin);

    return result;
}

void Utility::Replace(string& origin, string comp, string rep)
{
    size_t at = 0;
    while ((at = origin.find(comp, at)) != string::npos)
    {
        origin.replace(at, comp.length(), rep);
        at += rep.length();
    }
}

bool Utility::ExistDirectory(string file)
{
    DWORD fileValue = GetFileAttributesA(file.c_str());

    BOOL temp = (fileValue != INVALID_FILE_ATTRIBUTES
        && (fileValue & FILE_ATTRIBUTE_DIRECTORY));

    return temp == TRUE;
}

string Utility::ToString(wstring str)
{
    string result;
    result.assign(str.begin(), str.end());

    return result;
}

wstring Utility::ToWString(string str)
{
    wstring result;
    result.assign(str.begin(), str.end());

    return result;
}

wstring Utility::GetExtension(wstring file)
{
    size_t index = file.find_last_of('.');

    return file.substr(index + 1);
}

wstring Utility::GetFileName(string file)
{
    Replace(file, "\\", "/");
    wstring temp = ToWString(file);
    size_t index = temp.find_last_of('/');    

    return temp.substr(index + 1);
}

wstring Utility::GetFileNameWithoutExtension(string file)
{
    wstring fileName = GetFileName(file);

    size_t index = fileName.find_last_of('.');

    return fileName.substr(0, index);
}

void Utility::CreateFolders(wstring file)
{
    vector<string> folders = SplitString(ToString(file), "/", false);

    string temp = "";

    for (string folder : folders)
    {
        temp += folder + "/";

        if (!ExistDirectory(temp))
            CreateDirectoryA(temp.c_str(), 0);
    }
}

void Utility::CreateFolders(string file)
{    
    vector<string> folders = SplitString(file, "/", false);

    string temp = "";

    for (string folder : folders)
    {
        temp += folder + "/";

        if (!ExistDirectory(temp))
            CreateDirectoryA(temp.c_str(), 0);
    }
}
