#include "AppGui.h"

#include <wx/dirdlg.h>

AnimeRenamerFrame::AnimeRenamerFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style), pRenamer(nullptr)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);



	MainMenubar = new wxMenuBar(0);
	m_menu = new wxMenu();
	wxMenuItem* fileOpen;
	fileOpen = new wxMenuItem(m_menu, wxID_ANY, wxString(wxT("打开")), wxEmptyString, wxITEM_NORMAL);
	m_menu->Append(fileOpen);

	MainMenubar->Append(m_menu, wxT("菜单"));

	this->SetMenuBar(MainMenubar);
	
	 
	leftHorizontalSizer = new wxBoxSizer(wxVERTICAL);
	

	animeNameInput = new wxTextCtrl(this, wxID_ANY,"", wxDefaultPosition, wxDefaultSize, 0);
	animeNameInput->SetHint(wxT("输入动画名"));
	animeNameInput->SetWindowStyleFlag(wxTE_PROCESS_ENTER);

	leftHorizontalSizer->Add(animeNameInput, 0, wxALL, 5);
	
	
	seasonInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
	seasonInput->SetWindowStyleFlag(wxTE_PROCESS_ENTER);
	seasonInput->SetHint(wxT("输入季数"));
	leftHorizontalSizer->Add(seasonInput, 0, wxALL, 5);


	regxComboBox = new wxComboBox(this, wxID_ANY, wxT("正则选择器"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	leftHorizontalSizer->Add(regxComboBox, 0, wxALL, 5);

	genBackup = new wxCheckBox(this, wxID_ANY, wxT("生成文件名备份"), wxDefaultPosition, wxDefaultSize, 0);
	genBackup->SetValue(true);
	leftHorizontalSizer->Add(genBackup, 0, wxALL, 5);

	previewButton = new wxButton(this, wxID_ANY, wxT("预览"), wxDefaultPosition, wxDefaultSize, 0);
	leftHorizontalSizer->Add(previewButton, 0, wxALL, 5);

	

	renameButton = new wxButton(this, wxID_ANY, wxT("重命名"), wxDefaultPosition, wxDefaultSize, 0);
	leftHorizontalSizer->Add(renameButton, 0, wxALL, 5);

	recoverButton = new wxButton(this, wxID_ANY, wxT("从备份恢复"), wxDefaultPosition, wxDefaultSize, 0);
	leftHorizontalSizer->Add(recoverButton, 0, wxALL, 5);



	renamePreview = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), 0);
	renamePreview->AppendTextColumn(wxT("原始命名"));
	renamePreview->AppendTextColumn(wxT("新命名"));
	leftHorizontalSizer->Add(renamePreview, 0, wxEXPAND, 5);
	


	this->SetSizer(leftHorizontalSizer);
	this->Layout();

	this->Centre(wxBOTH);
	m_menu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AnimeRenamerFrame::onFileOpen), this, fileOpen->GetId());

	previewButton->Connect(wxEVT_LEFT_DOWN, wxCommandEventHandler(AnimeRenamerFrame::previewButtonOnButtonClick), NULL, this);
	renameButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimeRenamerFrame::onRenameClick), NULL, this);
	recoverButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimeRenamerFrame::onRestoreBackUp), NULL, this);
}

AnimeRenamerFrame::~AnimeRenamerFrame()
{
	recoverButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimeRenamerFrame::onRestoreBackUp), NULL, this);
	previewButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimeRenamerFrame::previewButtonOnButtonClick), NULL, this);
	renameButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimeRenamerFrame::onRenameClick), NULL, this);
}

void AnimeRenamerFrame::OnQuit(wxCommandEvent& event)
{
	wxWindowBase::Close(true);
}

void AnimeRenamerFrame::OnAbout(wxCommandEvent& event)
{
}

void AnimeRenamerFrame::OnQuitUpdate(wxUpdateUIEvent& event)
{
}

void AnimeRenamerFrame::onFileOpen(wxCommandEvent& event)
{
	wxDirDialog dlg(nullptr, wxT("选择你的番剧文件夹"), "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		wpath = dlg.GetPath();
	}
}

void AnimeRenamerFrame::onRenameClick(wxCommandEvent& event) {
	if (!pRenamer) {
		return;
	}
	if (this->genBackup->IsChecked()) {
		this->pRenamer->backUpPathes();
	}
	this->pRenamer->doRename();
}

void AnimeRenamerFrame::onRestoreBackUp(wxCommandEvent& event)
{
	if (!pRenamer) {
		return;
	}
	if (this->wpath != "")
	{
		pRenamer->restoreBackup(fs::path(this->wpath));
	}
}

void AnimeRenamerFrame::previewButtonOnButtonClick(wxCommandEvent& event)
{
	renamePreview->DeleteAllItems();

	auto season = seasonInput->GetLineText(0);
	auto animeName = animeNameInput->GetLineText(0);
	if (season != "" && animeName != "" && wpath != "")
	{
		this->pRenamer = std::make_unique<AnimeRenamemer>(fs::path(wpath), std::string(season), std::string(animeName));
	}
	if (this->pRenamer)
	{
		std::vector<std::pair<fs::path, fs::path>> previewList;
		try
		{
			previewList = pRenamer->getPreviewResult();
		}
		catch (const std::exception& e)
		{
			wxMessageBox(e.what());
			return;
		}
		


		for (const auto& path_old_new : previewList)
		{
			wxVector<wxVariant> data;
			data.push_back(wxString(path_old_new.first.filename().string()));
			data.push_back(wxString(path_old_new.second.filename().string()));
			renamePreview->AppendItem(data);
		}

		leftHorizontalSizer->FitInside(renamePreview);

	}


}


bool AnimeRenamerApp::OnInit()
{

	AnimeRenamerFrame* frame = new AnimeRenamerFrame(nullptr);
	frame->Show();
	return true;
}

AnimeRenamerApp::AnimeRenamerApp()
{
 
}

AnimeRenamerApp::~AnimeRenamerApp()
{

}

