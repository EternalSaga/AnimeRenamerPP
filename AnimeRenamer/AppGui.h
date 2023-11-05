#pragma once
#include <wx/wx.h>
#include <string>
#include <memory>
#include "AnimeRenamer.h"



#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/string.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dataview.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AnimeRenamerFrame
///////////////////////////////////////////////////////////////////////////////
class AnimeRenamerFrame : public wxFrame
{
private:
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnQuitUpdate(wxUpdateUIEvent& event);
	
	void onFileOpen(wxCommandEvent& event);

	void onPreviewButton(wxCommandEvent& event);
	void onRenameClick(wxCommandEvent& event);
	void onRestoreBackUp(wxCommandEvent& event);
	void onRegChoice(wxCommandEvent& event);
	void onReset(wxCommandEvent& event);
	bool previewClicked = false;
	wxString wpath;
	std::unique_ptr<AnimeRenamemer> pRenamer;
protected:
	wxMenuBar* MainMenubar;
	wxMenu* m_menu;
	wxMenu* openFile;
	wxMenu* quit;

	wxTextCtrl* animeNameInput;
	wxTextCtrl* seasonInput;
	wxComboBox* regxComboBox;
	wxCheckBox* genBackup;
	wxButton* previewButton;
	wxButton* renameButton;
	wxButton* recoverButton;
	wxButton* resetButton;
	wxDataViewListCtrl* renamePreview;

	wxBoxSizer* leftHorizontalSizer;

public:

	AnimeRenamerFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("AnimeRenamer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(1280, 720), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~AnimeRenamerFrame();

};





class AnimeRenamerApp : public wxAppBase {

	
public:
	AnimeRenamerApp();
	~AnimeRenamerApp();

	virtual bool OnInit();
};
