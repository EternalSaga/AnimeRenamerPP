///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GuiBase.h"

GuiBase::GuiBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	MainMenubar = new wxMenuBar(0);
	m_menu = new wxMenu();
	wxMenuItem* fileOpen;
	fileOpen = new wxMenuItem(m_menu, wxID_ANY, wxString(wxT("打开")), wxEmptyString, wxITEM_NORMAL);
	m_menu->Append(fileOpen);

	MainMenubar->Append(m_menu, wxT("菜单"));

	this->SetMenuBar(MainMenubar);

	wxBoxSizer* topSizer;
	topSizer = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* controlSizer;
	controlSizer = new wxBoxSizer(wxVERTICAL);

	controlSizer->SetMinSize(wxSize(160, -1));
	animeNameInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	controlSizer->Add(animeNameInput, 0, wxALL, 5);

	seasonInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	controlSizer->Add(seasonInput, 0, wxALL, 5);

	recoverButton = new wxButton(this, wxID_ANY, wxT("从备份恢复"), wxDefaultPosition, wxDefaultSize, 0);
	controlSizer->Add(recoverButton, 0, wxALL, 5);

	regxComboBox = new wxComboBox(this, wxID_ANY, wxT("正则选择器"), wxDefaultPosition, wxSize(100, -1), 0, NULL, 0);
	regxComboBox->Append(wxT("[集数]"));
	regxComboBox->Append(wxT("S季E集数"));
	regxComboBox->Append(wxT("集数"));
	regxComboBox->Append(wxT("自动选择"));
	regxComboBox->Append(wxEmptyString);
	controlSizer->Add(regxComboBox, 0, wxALL, 5);

	previewButton = new wxButton(this, wxID_ANY, wxT("预览"), wxDefaultPosition, wxDefaultSize, 0);
	controlSizer->Add(previewButton, 0, wxALL, 5);

	genBackup = new wxCheckBox(this, wxID_ANY, wxT("生成文件名备份"), wxDefaultPosition, wxDefaultSize, 0);
	genBackup->SetValue(true);
	controlSizer->Add(genBackup, 0, wxALL, 5);

	renameButton = new wxButton(this, wxID_ANY, wxT("重命名"), wxDefaultPosition, wxDefaultSize, 0);
	controlSizer->Add(renameButton, 0, wxALL, 5);

	resetButton = new wxButton(this, wxID_ANY, wxT("重置状态"), wxDefaultPosition, wxDefaultSize, 0);
	controlSizer->Add(resetButton, 0, wxALL, 5);

	hint = new wxStaticText(this, wxID_ANY, wxT("提示"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	hint->Wrap(-1);
	controlSizer->Add(hint, 0, wxALL, 5);


	topSizer->Add(controlSizer, 0, wxALIGN_LEFT | wxALIGN_TOP, 5);

	wxBoxSizer* previewSizer;
	previewSizer = new wxBoxSizer(wxVERTICAL);

	previewSizer->SetMinSize(wxSize(480, -1));
	renamePreview = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), 0);
	renamePreview->SetMinSize(wxSize(440, 330));

	previewSizer->Add(renamePreview, 0, wxALIGN_LEFT | wxALIGN_TOP | wxEXPAND, 5);


	topSizer->Add(previewSizer, 1, wxALIGN_LEFT | wxEXPAND, 5);


	this->SetSizer(topSizer);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	m_menu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GuiBase::onFileOpen), this, fileOpen->GetId());
	animeNameInput->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintInputAnime), NULL, this);
	animeNameInput->Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveAnimeInput), NULL, this);
	seasonInput->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintInputSeason), NULL, this);
	seasonInput->Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveSeason), NULL, this);
	recoverButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiBase::recoverBackup), NULL, this);
	recoverButton->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintRecover), NULL, this);
	recoverButton->Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveRecover), NULL, this);
	regxComboBox->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(GuiBase::onRegChoice), NULL, this);
	regxComboBox->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintRegex), NULL, this);
	regxComboBox->Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveRegex), NULL, this);
	previewButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiBase::onPreviewButton), NULL, this);
	previewButton->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintPreview), NULL, this);
	previewButton->Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeavePreview), NULL, this);
	renameButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiBase::onRenameClick), NULL, this);
	renameButton->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintRename), NULL, this);
	renameButton->Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveRename), NULL, this);
	resetButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiBase::onResetClick), NULL, this);
	resetButton->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintReset), NULL, this);
	resetButton->Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveReset), NULL, this);
}

GuiBase::~GuiBase()
{
	// Disconnect Events
	animeNameInput->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintInputAnime), NULL, this);
	animeNameInput->Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveAnimeInput), NULL, this);
	seasonInput->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintInputSeason), NULL, this);
	seasonInput->Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveSeason), NULL, this);
	recoverButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiBase::recoverBackup), NULL, this);
	recoverButton->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintRecover), NULL, this);
	recoverButton->Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveRecover), NULL, this);
	regxComboBox->Disconnect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(GuiBase::onRegChoice), NULL, this);
	regxComboBox->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintRegex), NULL, this);
	regxComboBox->Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveRegex), NULL, this);
	previewButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiBase::onPreviewButton), NULL, this);
	previewButton->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintPreview), NULL, this);
	previewButton->Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeavePreview), NULL, this);
	renameButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiBase::onRenameClick), NULL, this);
	renameButton->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintRename), NULL, this);
	renameButton->Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveRename), NULL, this);
	resetButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiBase::onResetClick), NULL, this);
	resetButton->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GuiBase::hintReset), NULL, this);
	resetButton->Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(GuiBase::onLeaveReset), NULL, this);

}
