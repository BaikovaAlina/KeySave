#pragma once

#include <vector>
#include <wx/string.h>

struct SItem {
    wxString title;
    wxString login;
    wxString password;
    wxString type;
};

using TVectorItems = std::vector<SItem>;

class CXMLFile {
public:
    static void read(const wxString& filePath, unsigned char* data, unsigned int& size);
    static void write(const wxString& filePath, const unsigned char* data, unsigned int size);
    static void parse(const unsigned char* data, unsigned int size, TVectorItems& items);
    static void collect(const TVectorItems& items, unsigned char* data, unsigned int& size);
};