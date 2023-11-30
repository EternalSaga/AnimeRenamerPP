#include "AppGui.h"

#include <wx/dirdlg.h>

AnimeRenamerFrame::AnimeRenamerFrame(wxWindow* parent) : GuiBase(parent),pRenamer(nullptr)
{
	animeNameInput->SetHint(wxT("输入动画名"));
	seasonInput->SetHint(wxT("输入季数"));
	renamePreview->AppendTextColumn(wxT("原始命名"));
	renamePreview->AppendTextColumn(wxT("新命名"));
	this->pRenamer = std::make_unique<AnimeRenamemer>();
}

AnimeRenamerFrame::~AnimeRenamerFrame()
{

}



void AnimeRenamerFrame::onFileOpen(wxCommandEvent& event)
{

	wxDirDialog dlg(nullptr, wxT("选择你的番剧文件夹"), "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		wpath = dlg.GetPath();
		this->pRenamer->setPath(wpath.ToStdString());
	}

}

void AnimeRenamerFrame::hintInputAnime(wxMouseEvent& event)
{
	if (event.Entering())
	{
		this->hint->SetLabelText(wxT("请输入动画名"));
		this->hint->Show();
	}

}

void AnimeRenamerFrame::hintInputSeason(wxMouseEvent& event)
{
	if (event.Entering())
	{
		this->hint->SetLabelText(wxT("请输入季数"));
		this->hint->Show();
	}
}

void AnimeRenamerFrame::onLeaveSeason(wxMouseEvent& event)
{
	if (event.Leaving())
	{
		this->hint->Hide();
	}
}

void AnimeRenamerFrame::onRenameClick(wxCommandEvent& event) {

	if (this->genBackup->IsChecked()) {
		this->pRenamer->backUpPathes();
	}
	this->pRenamer->doRename();
}

void AnimeRenamerFrame::hintRename(wxMouseEvent& event)
{
	if (event.Entering())
	{
		this->hint->SetLabelText(wxT("执行重名操作，需要先预览"));
		this->hint->Show();
	}
}

void AnimeRenamerFrame::onLeaveRename(wxMouseEvent& event)
{
	if (event.Leaving())
	{
		this->hint->Hide();
	}
}

void AnimeRenamerFrame::recoverBackup(wxCommandEvent& event)
{
	if (!pRenamer) {
		return;
	}
	if (this->wpath != "")
	{
		pRenamer->restoreBackup(fs::path(this->wpath.ToStdWstring()));
	}
}

void AnimeRenamerFrame::hintRecover(wxMouseEvent& event)
{
	if (event.Entering())
	{
		this->hint->SetLabelText(wxT("从备份文件中恢复原始命名，注意备份文件中的路径"));
		this->hint->Show();
	}
}

void AnimeRenamerFrame::onLeaveRecover(wxMouseEvent& event)
{
	if (event.Leaving())
	{
		this->hint->Hide();
	}
}

void AnimeRenamerFrame::onRegChoice(wxCommandEvent& event)
{
	const auto& choiceString = event.GetString();


	pRenamer->setRegex(choiceString.ToStdString());
}

void AnimeRenamerFrame::hintRegex(wxMouseEvent& event)
{
	if (event.Entering())
	{
		this->hint->SetLabelText(wxT("必须选择一个正则表达式"));
		this->hint->Show();
	}
}

void AnimeRenamerFrame::onLeaveRegex(wxMouseEvent& event)
{
	if (event.Leaving())
	{
		this->hint->Hide();
	}
}

void AnimeRenamerFrame::onResetClick(wxCommandEvent& event)
{
	animeNameInput->Clear();
	seasonInput->Clear();
	wpath.clear();
}

void AnimeRenamerFrame::onLeaveReset(wxMouseEvent& event)
{
	if (event.Leaving())
	{
		this->hint->Hide();
	}
}

void AnimeRenamerFrame::hintReset(wxMouseEvent& event)
{
	if (event.Entering())
	{
		this->hint->SetLabelText(wxT("重置全部状态"));
		this->hint->Show();
	}
}

void AnimeRenamerFrame::onLeaveAnimeInput(wxMouseEvent& event)
{
	if (event.Leaving())
	{
		this->hint->Hide();
	}
}

void AnimeRenamerFrame::onPreviewButton(wxCommandEvent& event)
{
	renamePreview->DeleteAllItems();

	const auto& season = seasonInput->GetLineText(0);
	const auto& animeName = animeNameInput->GetLineText(0);
	this->pRenamer->setSeason(season.ToStdString());
	this->pRenamer->setAnimeName(animeName.ToStdString());
	//this->pRenamer->setPath(fs::path(this->wpath.ToStdString()));
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
		const std::u8string oldu8 = path_old_new.first.filename().u8string();
		const std::u8string newu8 = path_old_new.second.filename().u8string();
		const std::string oldPath(oldu8.begin(), oldu8.end());
		const std::string newPath(newu8.begin(), newu8.end());
		
		data.push_back(wxString::FromUTF8(oldPath));
		data.push_back(wxString::FromUTF8(newPath));
		renamePreview->AppendItem(data);
	}



}

void AnimeRenamerFrame::hintPreview(wxMouseEvent& event)
{
	if (event.Entering())
	{
		this->hint->SetLabelText(wxT("执行重名的结果预览"));
		this->hint->Show();
	}
}

void AnimeRenamerFrame::onLeavePreview(wxMouseEvent& event)
{
	if (event.Leaving())
	{
		this->hint->Hide();
	}
}


bool AnimeRenamerApp::OnInit()
{
	frame = new AnimeRenamerFrame(nullptr);
	frame->Show();
	return true;
}

AnimeRenamerApp::AnimeRenamerApp():frame(nullptr)
{
	
}

AnimeRenamerApp::~AnimeRenamerApp()
{

}

