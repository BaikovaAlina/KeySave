#include "mainframe.h"

#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <iostream>

#define INDEX_LABEL_TITLE 0
#define INDEX_LABEL_LOGIN 1
#define INDEX_LABEL_PASSWORD 2
#define INDEX_LABEL_TYPE 3

CMainFrame::CMainFrame()
        : wxFrame(nullptr, wxID_ANY, wxT("Password manager"))
        , mDataEncrypt { 0 }
        , mSizeEncrypt(0)
        , mDataDecrypt{ 0 }
        , mSizeDecrypt(0)
{
    wxButton* btnLoad = new wxButton(this, wxID_ANY, wxT("Load"));
    wxButton* btnUpload = new wxButton(this, wxID_ANY, wxT("Upload"));
    wxButton* btnAppend = new wxButton(this, wxID_ANY, wxT("New"));
    mSearchButton = new wxButton(this, wxID_ANY, wxT("Search"));
    mSearchCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mGeneratePasswordButton = new wxButton(this, wxID_ANY, wxT("Generate Password"));

    mGrid = new wxGrid(this, wxID_ANY);
    mGrid->CreateGrid(0, 4);
    mGrid->SetColLabelValue(INDEX_LABEL_TITLE, "title");
    mGrid->SetColLabelValue(INDEX_LABEL_LOGIN, "login");
    mGrid->SetColLabelValue(INDEX_LABEL_PASSWORD, "password");
    mGrid->SetColLabelValue(INDEX_LABEL_TYPE, "type");
    mGrid->HideRowLabels();

    wxBoxSizer* hBox = nullptr;
    wxBoxSizer* mainBox = new wxBoxSizer(wxVERTICAL);

    hBox = new wxBoxSizer(wxHORIZONTAL);
    hBox->Add(btnLoad, 1);
    hBox->Add(btnUpload, 1);
    mainBox->Add(hBox, 0, wxEXPAND);
    mainBox->Add(btnAppend, 0, wxEXPAND);

    hBox = new wxBoxSizer(wxHORIZONTAL);
    hBox->Add(mSearchCtrl, 1);
    hBox->Add(mSearchButton, 0);
    mainBox->Add(hBox, 0, wxEXPAND);

    mainBox->Add(mGeneratePasswordButton, 0, wxEXPAND);
    mainBox->Add(mGrid, 1, wxEXPAND);

    SetSizerAndFit(mainBox);

    Bind(wxEVT_BUTTON, &CMainFrame::OnLoad, this, btnLoad->GetId());
    Bind(wxEVT_BUTTON, &CMainFrame::OnUpload, this, btnUpload->GetId());
    Bind(wxEVT_BUTTON, &CMainFrame::OnAppend, this, btnAppend->GetId());
    Bind(wxEVT_BUTTON, &CMainFrame::OnSearch, this, mSearchButton->GetId());
    Bind(wxEVT_TEXT_ENTER, &CMainFrame::OnSearch, this, mSearchCtrl->GetId());
    Bind(wxEVT_BUTTON, &CMainFrame::OnGeneratePassword, this, mGeneratePasswordButton->GetId());

    try {
        mJNICloudLib.init();
    } catch (const char* message) {
        std::cout << message << std::endl;
    }
}

void CMainFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{
    int nRows = 0;

    mItems.clear();
    memset(mDataDecrypt, 0, mSizeDecrypt = SIZE_BUFFER);
    memset(mDataEncrypt, 0, mSizeEncrypt = SIZE_BUFFER);
    nRows = mGrid->GetNumberRows();
    if(nRows != 0)
        mGrid->DeleteRows(0, mGrid->GetNumberRows());

    try {

        mJNICloudLib.run("load", TOKEN, "test/keys_encrypt_upload.xml", RESOURCES "/keys_encrypt_load.xml");

        CXMLFile::read(RESOURCES "/keys_encrypt_load.xml", mDataEncrypt, mSizeEncrypt);

        CCipher::decrypt("0011", mDataEncrypt, mSizeEncrypt, mDataDecrypt, mSizeDecrypt);

        CXMLFile::parse(mDataDecrypt, mSizeDecrypt, mItems);

    } catch(const char* message) {
        std::cout << message << std::endl;
    }

    mFilteredItems = mItems;
    for(auto& item : mFilteredItems) {

        nRows = mGrid->GetNumberRows();
        mGrid->AppendRows();
        mGrid->SetCellValue(nRows, INDEX_LABEL_TITLE, item.title);
        mGrid->SetCellValue(nRows, INDEX_LABEL_LOGIN, item.login);
        mGrid->SetCellValue(nRows, INDEX_LABEL_PASSWORD, item.password);
        mGrid->SetCellValue(nRows, INDEX_LABEL_TYPE, item.type);
    }

    this->Fit();
}

void CMainFrame::OnUpload(wxCommandEvent& WXUNUSED(event))
{
    int nRows = 0;

    mItems.clear();
    memset(mDataDecrypt, 0, mSizeDecrypt = SIZE_BUFFER);
    memset(mDataEncrypt, 0, mSizeEncrypt = SIZE_BUFFER);

    nRows = mGrid->GetNumberRows();
    for(int iRow = 0; iRow < nRows; iRow++) {

        SItem item;
        item.title = mGrid->GetCellValue(iRow, INDEX_LABEL_TITLE);
        item.login = mGrid->GetCellValue(iRow, INDEX_LABEL_LOGIN);
        item.password = mGrid->GetCellValue(iRow, INDEX_LABEL_PASSWORD);
        item.type = mGrid->GetCellValue(iRow, INDEX_LABEL_TYPE);
        mItems.emplace_back(item);
    }

    try {

        if(mItems.empty() == true)
            throw "error: items is empty";

        CXMLFile::collect(mItems, mDataDecrypt, mSizeDecrypt);

        CCipher::encrypt("0011", mDataDecrypt, mSizeDecrypt, mDataEncrypt, mSizeEncrypt);

        CXMLFile::write(RESOURCES "/keys_encrypt_upload.xml", mDataEncrypt, mSizeEncrypt);

        mJNICloudLib.run("upload", TOKEN, "test", RESOURCES "/keys_encrypt_upload.xml");

    } catch(const char* message) {
        std::cout << message << std::endl;
    }
}

void CMainFrame::OnAppend(wxCommandEvent& WXUNUSED(event))
{
    int nRows = mGrid->GetNumberRows();
    mGrid->AppendRows();
    mGrid->SetCellValue(nRows, INDEX_LABEL_TYPE, "block");
    this->Fit();
}

void CMainFrame::OnSearch(wxCommandEvent& WXUNUSED(event))
{
    wxString searchText = mSearchCtrl->GetValue();
    FilterItems(searchText);
}

void CMainFrame::FilterItems(const wxString& searchText)
{
    mFilteredItems.clear();

    std::string searchStr = searchText.ToStdString();

    for (const auto& item : mItems) {
        if (item.title.find(searchStr) != std::string::npos ||
            item.login.find(searchStr) != std::string::npos ||
            item.password.find(searchStr) != std::string::npos) {
            mFilteredItems.push_back(item);
        }
    }

    int nRows = mGrid->GetNumberRows();
    if (nRows != 0)
        mGrid->DeleteRows(0, nRows);

    for (auto& item : mFilteredItems) {
        nRows = mGrid->GetNumberRows();
        mGrid->AppendRows();
        mGrid->SetCellValue(nRows, INDEX_LABEL_TITLE, item.title);
        mGrid->SetCellValue(nRows, INDEX_LABEL_LOGIN, item.login);
        mGrid->SetCellValue(nRows, INDEX_LABEL_PASSWORD, item.password);
        mGrid->SetCellValue(nRows, INDEX_LABEL_TYPE, item.type);
    }

    this->Fit();
}

std::string CMainFrame::GenerateRandomPassword(int length, bool useSpecialChars)
{
    const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::string specialChars = "!@#$%^&*()_+-=[]{}|;:,.<>?";
    std::string allChars = chars;
    if (useSpecialChars) {
        allChars += specialChars;
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, allChars.size() - 1);

    std::string password;
    for (int i = 0; i < length; ++i) {
        password += allChars[distribution(generator)];
    }

    return password;
}

void CMainFrame::OnGeneratePassword(wxCommandEvent& WXUNUSED(event))
{
    int length = wxNumberEntryDialog(this, wxT("Enter password length:"), wxT("Length"), wxT("Password Length"), 12, 6, 32).GetValue();
    bool useSpecialChars = wxMessageBox(wxT("Use special characters?"), wxT("Special Characters"), wxYES_NO | wxICON_QUESTION) == wxYES;

    std::string password = GenerateRandomPassword(length, useSpecialChars);

    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxTextDataObject(password));
        wxTheClipboard->Close();
        wxMessageBox(wxT("Password copied to clipboard!"), wxT("Success"), wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox(wxT("Failed to open clipboard."), wxT("Error"), wxOK | wxICON_ERROR);
    }
}