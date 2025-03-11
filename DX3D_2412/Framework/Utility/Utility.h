#pragma once

class Utility
{
public:
    static vector<string> SplitString(string origin, const string& tok, const bool& includeLast = true);
    static void Replace(string& origin, string comp, string rep);

    static bool ExistDirectory(string file);

    static string ToString(wstring str);
    static wstring ToWString(string str);

    static wstring GetExtension(wstring file);
    static wstring GetFileName(string file);
    static wstring GetFileNameWithoutExtension(string file);
    static void CreateFolders(wstring file);
    static void CreateFolders(string file);
};