#pragma once
#include <wx/wx.h>
#include <string>
#include <memory>
#include "AnimeRenamer.h"

#include "GuiBase.h"

class AnimeRenamerFrame : public GuiBase
{
private:

	bool previewClicked = false;
	wxString wpath;
	std::unique_ptr<AnimeRenamemer> pRenamer;


	void onFileOpen(wxCommandEvent& event) override;
	void hintInputAnime(wxMouseEvent& event) override;
	void onLeaveAnimeInput(wxMouseEvent& event) override;
	void hintInputSeason(wxMouseEvent& event) override;
	void onLeaveSeason(wxMouseEvent& event) override;
	void recoverBackup(wxCommandEvent& event) override;
	void hintRecover(wxMouseEvent& event) override;
	void onLeaveRecover(wxMouseEvent& event) override;
	void onRegChoice(wxCommandEvent& event) override;
	void hintRegex(wxMouseEvent& event) override;
	void onLeaveRegex(wxMouseEvent& event) override;
	void onPreviewButton(wxCommandEvent& event) override;
	void hintPreview(wxMouseEvent& event) override;
	void onLeavePreview(wxMouseEvent& event) override;
	void onRenameClick(wxCommandEvent& event) override;
	void hintRename(wxMouseEvent& event) override;
	void onLeaveRename(wxMouseEvent& event) override;
	void onResetClick(wxCommandEvent& event) override;
	void onLeaveReset(wxMouseEvent& event) override;
	void hintReset(wxMouseEvent& event) override;
public:

	AnimeRenamerFrame(wxWindow* parent);

	~AnimeRenamerFrame();

};





class AnimeRenamerApp : public wxAppBase {
	AnimeRenamerFrame* frame;

public:
	AnimeRenamerApp();
	~AnimeRenamerApp();

	bool OnInit();
};
