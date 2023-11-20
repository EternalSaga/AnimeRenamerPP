///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/dataview.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GuiBase
///////////////////////////////////////////////////////////////////////////////
class GuiBase : public wxFrame
{
private:

protected:
	wxMenuBar* MainMenubar;
	wxMenu* m_menu;
	wxTextCtrl* animeNameInput;
	wxTextCtrl* seasonInput;
	wxButton* recoverButton;
	wxComboBox* regxComboBox;
	wxButton* previewButton;
	wxCheckBox* genBackup;
	wxButton* renameButton;
	wxButton* resetButton;
	wxStaticText* hint;
	wxDataViewListCtrl* renamePreview;

	// Virtual event handlers, override them in your derived class
	virtual void onFileOpen(wxCommandEvent& event) { event.Skip(); }
	virtual void hintInputAnime(wxMouseEvent& event) { event.Skip(); }
	virtual void onLeaveAnimeInput(wxMouseEvent& event) { event.Skip(); }
	virtual void hintInputSeason(wxMouseEvent& event) { event.Skip(); }
	virtual void onLeaveSeason(wxMouseEvent& event) { event.Skip(); }
	virtual void recoverBackup(wxCommandEvent& event) { event.Skip(); }
	virtual void hintRecover(wxMouseEvent& event) { event.Skip(); }
	virtual void onLeaveRecover(wxMouseEvent& event) { event.Skip(); }
	virtual void onRegChoice(wxCommandEvent& event) { event.Skip(); }
	virtual void hintRegex(wxMouseEvent& event) { event.Skip(); }
	virtual void onLeaveRegex(wxMouseEvent& event) { event.Skip(); }
	virtual void onPreviewButton(wxCommandEvent& event) { event.Skip(); }
	virtual void hintPreview(wxMouseEvent& event) { event.Skip(); }
	virtual void onLeavePreview(wxMouseEvent& event) { event.Skip(); }
	virtual void onRenameClick(wxCommandEvent& event) { event.Skip(); }
	virtual void hintRename(wxMouseEvent& event) { event.Skip(); }
	virtual void onLeaveRename(wxMouseEvent& event) { event.Skip(); }
	virtual void onResetClick(wxCommandEvent& event) { event.Skip(); }
	virtual void hintReset(wxMouseEvent& event) { event.Skip(); }
	virtual void onLeaveReset(wxMouseEvent& event) { event.Skip(); }


public:

	GuiBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("AnimeRenamer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(644, 645), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~GuiBase();

};

