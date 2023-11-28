#include "Assignment.h"
#include <wx/wx.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "RoundedRectangle.h"
#include <wx/gbsizer.h>
#include <wx/datectrl.h>
#include "Assets.h"

wxDateTime dt = wxDateTime::Today();
wxDateTime dt2 = wxDateTime::Today() + wxDateSpan::Days(1);
int completed = 0;
int remaining = 0;

Assignment::Assignment(wxWindow* parent) : wxPanel(parent)
{
	const auto margin = FromDIP(30);
	auto mainsizer = new wxBoxSizer(wxVERTICAL);
	mainsizer->AddSpacer(10);
	wxFont* headLineFont = new wxFont(32, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	auto* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(800, 800)); // , wxDefaultPosition, wxDefaultSize, wxEXPAND | wxALL);
	panel->SetBackgroundColour(wxColor(84, 78, 111));

	this->SetBackgroundColour(panel->GetBackgroundColour());
	auto sizer = new wxGridBagSizer(margin, margin);

	auto MainTitle = new wxStaticText(panel, wxID_ANY, "Assignments");

	MainTitle->SetForegroundColour(TEXT_THEME_COLOUR);

	int count = 0;
	sizer->Add(MainTitle, { 0, 0 }, { 1, 3 });

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (count < 7)
			{
				auto BoxArray = new RoundedRectangle(panel, wxSize(400, 220), wxColor(44, 41, 59), wxColor(84, 78, 111), 30);
				sizer->Add(BoxArray, { i + 1, j }, { 1, 1 });

				auto MainSizer = new wxBoxSizer(wxVERTICAL);
				auto TitleAndButton = new wxBoxSizer(wxVERTICAL);
				auto Title = new wxBoxSizer(wxHORIZONTAL);
				auto Button = new wxBoxSizer(wxHORIZONTAL);
				auto CheckBox = new wxBoxSizer(wxHORIZONTAL);

				switch (count)
				{
				case 0:
					DisplayTitle = "Digital Logic";
					break;
				case 1:
					DisplayTitle = "Object Oriented Programming";
					break;
				case 2:
					DisplayTitle = "Electronics Devices and Circuits";
					break;
				case 3:
					DisplayTitle = "Electrical Circuits Theory";
					break;
				case 4:
					DisplayTitle = "Theory of Computation";
					break;
				case 5:
					DisplayTitle = "Mathematics III";
					break;
				case 6:
					DisplayTitle = "Electromagnetics";
					break;
				}

				wxFont TitleFont(wxFontInfo(wxSize(0, 20)).Bold());
				auto TitleDisplay = new wxStaticText(BoxArray, wxID_ANY, DisplayTitle, wxPoint(20, 10), wxSize(200, 30));
				TitleDisplay->SetFont(TitleFont);
				TitleDisplay->SetBackgroundColour(wxColor(44, 41, 59));
				TitleDisplay->SetForegroundColour(*wxWHITE);
				Title->Add(TitleDisplay);

				AddButton[count] = new wxButton(BoxArray, wxID_ANY, "+", wxPoint(355, 10), wxSize(25, 25));
				AddButton[count]->SetBackgroundColour(wxColor(84, 78, 111));
				Button->Add(AddButton[count]);

				CheckListBox[count] = new wxCheckListBox(BoxArray, wxID_ANY, wxPoint(20, 50), wxSize(360, 150), 0, NULL, wxNO_BORDER); //, wxALIGN_BOTTOM);// | wxALIGN_CENTER);
				CheckListBox[count]->SetBackgroundColour(SIDEBAR_COLOUR);
				CheckListBox[count]->SetForegroundColour(*wxWHITE);
				TitleAndButton->Add(Title);
				TitleAndButton->Add(Button);

				CheckBox->Add(CheckListBox[count]);

				MainSizer->Add(TitleAndButton);
				MainSizer->Add(CheckBox);

				BindEventHandlers(count);
				count++;
			}
		}
	}
	MainTitle->SetFont(*headLineFont);

	AddSavedTasks();

	panel->SetSizer(sizer);
	mainsizer->Add(panel, 0, wxEXPAND | wxALL, margin);
	this->SetSizerAndFit(mainsizer);
	Hide();
}

void Assignment::BindEventHandlers(int a)
{
	int j = a;
	int pra = j;

	AddButton[j]->Bind(wxEVT_BUTTON, [this, pra](wxCommandEvent& evt)
		{ OnAddButtonClicked(evt, pra); });

	CheckListBox[j]->Bind(wxEVT_KEY_DOWN, [this, j](wxKeyEvent& evt)
		{ OnListKeyDown(evt, j); });
	RemoveInputFields();
};

void Assignment::OnAddButtonClicked(wxCommandEvent& evt, int pra) { CallInputFields(pra); }

void Assignment::OnListKeyDown(wxKeyEvent& evt, int j)
{
	switch (evt.GetKeyCode())
	{
	case WXK_DELETE:
		DeleteSelectedTask(j);
		break;
	case WXK_UP:
		MoveSelectedTasks(j, -1);
		break;
	case WXK_DOWN:
		MoveSelectedTasks(j, 1);
		break;
	};
};

void Assignment::DeleteSelectedTask(int m)
{
	int selectedIndex = CheckListBox[m]->GetSelection();
	if (selectedIndex == wxNOT_FOUND)
	{
		return;
	};
	CheckListBox[m]->Delete(selectedIndex);
};

void Assignment::AddItemFromInput(int p, wxString Input, wxString Time)
{

	wxString date;
	wxString item = Input;
	wxString time = Time;

	wxDateTime dt3;

	if (dt3.ParseISODate(time))
	{
		if (dt3.IsEqualTo(dt))
		{
			date = "Due Today";
		}
		else if (dt3.IsEqualTo(dt2))
		{
			date = "Due Tomorrow";
		}
		else if (dt3.IsEarlierThan(dt))
		{
			date = "Now Overdue";
		}
		else
		{
			date = time;
		}
	}

	item = date + wxString(30 - date.Length(), ' ') + item;
	if (!item.IsEmpty() && !time.IsEmpty() && time.Length() <= 10 && dt3.ParseISODate(time))
	{
		int index = CheckListBox[p]->GetCount();
		CheckListBox[p]->Insert(item, CheckListBox[p]->GetCount());
		CheckListBox[p]->GetItem(index)->SetTextColour(wxColor(255, 255, 255));

	}
}

void Assignment::CallInputFields(int f)
{

	wxTextEntryDialog* InputBoxNew = new wxTextEntryDialog(this, "Enter your assignment here:", "Assignment");
	InputBoxNew->SetBackgroundColour(wxColor(84, 78, 111));

	if (InputBoxNew->ShowModal() == wxID_OK)
	{
		Input = InputBoxNew->GetValue();
	}
	if (!Input.IsEmpty())
	{

		wxTextEntryDialog* InputBoxNewTime = new wxTextEntryDialog(this, "Enter submission date in YYYY-MM-DD format here:", "Date of Submission");
		InputBoxNewTime->SetBackgroundColour(wxColor(84, 78, 111));

		if (InputBoxNewTime->ShowModal() == wxID_OK)
		{
			InputTime = InputBoxNewTime->GetValue();
		}
	}

	if (!Input.IsEmpty() && !InputTime.IsEmpty())
	{
		AddItemFromInput(f, Input, InputTime);
	}
	Input = "";
}

void Assignment::RemoveInputFields()
{

	if (InputFields != nullptr && InputFields->IsShown())
	{
		InputFields->Hide();
		for (int i = 0; i < 4; i++)
		{
			CheckListBox[i]->Show();
			AddButton[i]->Show();
			ClearButton[i]->Show();
			TitleText_[i]->Show();
		}
	}
}

void Assignment::MoveSelectedTasks(int f, int offset)
{
	int selectedIndex = CheckListBox[f]->GetSelection();
	if (selectedIndex == wxNOT_FOUND)
	{
		return;
	}

	int newIndex = selectedIndex + offset;
	if (newIndex >= 0 && newIndex < CheckListBox[f]->GetCount())
	{
		SwapTasks(f, selectedIndex, newIndex);
		CheckListBox[f]->SetSelection(newIndex, true);
	}
}

void Assignment::SwapTasks(int i, int j, int k)
{
	Assignment_a TaskJ{ CheckListBox[i]->GetString(j).ToStdString(), CheckListBox[i]->IsChecked(j) };
	Assignment_a TaskK{ CheckListBox[i]->GetString(k).ToStdString(), CheckListBox[i]->IsChecked(k) };

	CheckListBox[i]->SetString(j, TaskK.item);
	CheckListBox[i]->Check(j, TaskK.done);

	CheckListBox[i]->SetString(k, TaskJ.item);
	CheckListBox[i]->Check(k, TaskJ.done);
}

void Assignment::AddSavedTasks() // This function Adds the saved tasks to the vector to display in the CheckListBox when the user opens the app
{
	int count = 0;
	while (count < 7)
	{
		switch (count)
		{
		case 0:
			FileName = "Assignment_DL.txt";
			break;
		case 1:
			FileName = "Assignment_OOP.txt";
			break;
		case 2:
			FileName = "Assignment_EDC.txt";
			break;
		case 3:
			FileName = "Assignment_ECT.txt";
			break;
		case 4:
			FileName = "Assignment_TOC.txt";
			break;
		case 5:
			FileName = "Assignment_Math.txt";
			break;
		case 6:
			FileName = "Assignment_EM.txt";
			break;
		default:
			break;
		}
		std::vector<Assignment_a> assignments = LoadItem(FileName);
		for (const Assignment_a& assignment : assignments)
		{
			bool check = false;
			int index = CheckListBox[count]->GetCount();
			std::string Data;
			wxDateTime dt3;
			if (assignment.time == dt.Format("%Y-%m-%d"))
			{
				Data = "Due Today " + assignment.item;
			}
			else if (assignment.time == dt2.Format("%Y-%m-%d"))
			{
				Data = "Due Tomorrow " + assignment.item;
			}
			else if (dt3.ParseISODate(assignment.time) && dt3.IsEarlierThan(dt))
			{
				Data = "Now Overdue" + assignment.item;
				if (assignment.done)
				{
					check = true;
				}
			}
			else if (
				((dt3.ParseISODate(assignment.time) && dt3.IsEarlierThan(dt)) || assignment.time == "Now Overdue") && assignment.done)
			{
				check = true;
			}
			else
			{
				Data = assignment.time + assignment.item;
			}
			if (!check)
			{
				CheckListBox[count]->Insert(Data, index);
				CheckListBox[count]->Check(index, assignment.done);
				CheckListBox[count]->GetItem(index)->SetTextColour(wxColor(255, 255, 255));
			}
		}
		count++;
	}
}

void Assignment::OnWindowClosedAssignment(wxCloseEvent& evt) // , int i)
{
	for (int i = 0; i < 7; i++)
	{
		std::vector<Assignment_a> assignments;

		int k = CheckListBox[i]->GetCount();
		for (int j = 0; j < k; j++)
		{
			Assignment_a assignment;
			assignment.item = CheckListBox[i]->GetString(j);
			std::string data = assignment.item;
			int k = 0;
			bool Continue = true;
			int count = 0;
			std::string word;
			int index = 0;
			while (Continue)
			{
				word = data.substr(index, data.find(" "));
				if (count == 0)
				{
					if (word.length() == 10 || word.length() == 9 || word.length() == 8)
					{
						assignment.time = word;
						assignment.item = data.substr(word.length(), data.length());
						Continue = false;
					}
					else if (word.length() == 3)
					{
						assignment.time = word + ' ';
						data = data.substr(4, data.length());
					}
				}
				if (count == 1)
				{
					assignment.time = assignment.time + word;
					if (assignment.time == "Now Overdue")
					{

						assignment.item = ' ' + data.substr(word.length() + 1, data.length());
					}
					else
					{
						assignment.item = data.substr(word.length() + 1, data.length());
					}
					Continue = false;
				}

				count++;
			}

			if (assignment.time == "Due Today")
			{
				assignment.time = dt.Format("%Y-%m-%d");
			}
			else if (assignment.time == "Due Tomorrow")
			{
				assignment.time = dt2.Format("%Y-%m-%d");
			}

			assignment.done = CheckListBox[i]->IsChecked(j);

			assignments.push_back(Assignment_a{ assignment.item, assignment.done, assignment.time });
		}

		switch (i)
		{
		case 0:
			SaveItem(assignments, "Assignment_DL.txt");
			break;
		case 1:
			SaveItem(assignments, "Assignment_OOP.txt");
			break;
		case 2:
			SaveItem(assignments, "Assignment_EDC.txt");
			break;
		case 3:
			SaveItem(assignments, "Assignment_ECT.txt");
			break;
		case 4:
			SaveItem(assignments, "Assignment_TOC.txt");
			break;
		case 5:
			SaveItem(assignments, "Assignment_Math.txt");
			break;
		case 6:
			SaveItem(assignments, "Assignment_EM.txt");
			break;
		}
		evt.Skip();
	}
}

void SaveItem(const std::vector<Assignment::Assignment_a>& Assignments, const std::string& filename) // Saves item in the file
{
	std::ofstream ostream(filename);
	ostream << Assignments.size();
	for (const Assignment::Assignment_a& _assignment : Assignments)
	{
		wxString item = _assignment.item;
		wxString date = _assignment.time;
		std::replace(item.begin(), item.end(), ' ', '_');
		std::replace(date.begin(), date.end(), ' ', '_');
		ostream << '\n'
			<< date << ' ' << item << ' ' << _assignment.done;
	}
}

std::vector<Assignment::Assignment_a> LoadItem(const std::string& filename) // Loads the items in the vector and returns the vector
{
	if (!std::filesystem::exists(filename))
	{
		return std::vector<Assignment::Assignment_a>();
	}

	std::vector<Assignment::Assignment_a> Assignments;
	std::ifstream istream(filename);
	int n;
	istream >> n;
	for (int i = 0; i < n; i++)
	{
		std::string date;
		std::string item;
		bool done;
		istream >> date >> item >> done;
		std::replace(date.begin(), date.end(), '_', ' ');
		std::replace(item.begin(), item.end(), '_', ' ');
		Assignments.push_back(Assignment::Assignment_a{item, done, date});
		if (!done)
		{
			remaining++;
		}
		else
		{
			completed++;
		}
	}
	return Assignments;
}