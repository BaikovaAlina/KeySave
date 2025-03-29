#pragma once

#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/clipbrd.h>
#include <wx/numdlg.h>
#include <wx/msgdlg.h>
#include <random>

#include "cipher.h"
#include "jnicloudelib.h"
#include "xmlfile.h"

#define SIZE_BUFFER 8192

class CMainFrame : public wxFrame
{
private:
    wxGrid* mGrid;
    wxTextCtrl* mSearchCtrl;
    wxButton* mSearchButton;
    wxButton* mGeneratePasswordButton;

    TVectorItems mItems;
    TVectorItems mFilteredItems;

    unsigned char mDataEncrypt[SIZE_BUFFER];
    unsigned int mSizeEncrypt;

    unsigned char mDataDecrypt[SIZE_BUFFER];
    unsigned int mSizeDecrypt;

    CJNICloudLib mJNICloudLib;

    void OnLoad(wxCommandEvent& event);
    void OnUpload(wxCommandEvent& event);
    void OnAppend(wxCommandEvent& event);
    void OnSearch(wxCommandEvent& event);
    void OnGeneratePassword(wxCommandEvent& event);

    void FilterItems(const wxString& searchText);
    std::string GenerateRandomPassword(int length, bool useSpecialChars);

public:
    CMainFrame();
    ~CMainFrame() override = default;
};