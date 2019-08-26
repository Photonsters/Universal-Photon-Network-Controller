#include "SettingsDialog.h"

//(*InternalHeaders(SettingsDialog)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/textdlg.h>
#include <wx/tokenzr.h>
#include <wx/dynarray.h>
#include "settings.h"
#include "NewFrame.h"

bool IsIPAddress(wxString ipaddr)
{
    wxStringTokenizer quads(ipaddr.Trim(), ".");
    if (quads.CountTokens() != 4)
        return false;
    wxString quad;
    while (quads.HasMoreTokens())
    {
        quad = quads.GetNextToken();
        if(!quad.IsNumber())
            return false;
        long value;
        if(!quad.ToLong(&value))
            return false;
        else
        {
            if ((value < 0) || (value > 255))
                    return false;
        }
    }
    return true;
}


//(*IdInit(SettingsDialog)
const long SettingsDialog::ID_STATICTEXT1 = wxNewId();
const long SettingsDialog::ID_STATICTEXT2 = wxNewId();
const long SettingsDialog::ID_STATICTEXT3 = wxNewId();
const long SettingsDialog::ID_STATICTEXT4 = wxNewId();
const long SettingsDialog::ID_TEXTCTRL1 = wxNewId();
const long SettingsDialog::ID_TEXTCTRL2 = wxNewId();
const long SettingsDialog::ID_TEXTCTRL3 = wxNewId();
const long SettingsDialog::ID_TEXTCTRL4 = wxNewId();
const long SettingsDialog::ID_BUTTON1 = wxNewId();
const long SettingsDialog::ID_BUTTON2 = wxNewId();
const long SettingsDialog::ID_STATICTEXT5 = wxNewId();
const long SettingsDialog::ID_LISTBOX1 = wxNewId();
const long SettingsDialog::ID_BITMAPBUTTON1 = wxNewId();
const long SettingsDialog::ID_BITMAPBUTTON2 = wxNewId();
const long SettingsDialog::ID_CHECKBOX1 = wxNewId();
const long SettingsDialog::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(SettingsDialog,wxDialog)
	//(*EventTable(SettingsDialog)
	//*)
END_EVENT_TABLE()

NewFrame *parentFrame=NULL;
SettingsDialog::SettingsDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SettingsDialog)
	Create(parent, wxID_ANY, _("Settings"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("wxID_ANY"));
	SetClientSize(wxSize(321,281));
	SetMaxSize(wxSize(25,-1));
	btnAddIP = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(320,280), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	StaticText1 = new wxStaticText(btnAddIP, ID_STATICTEXT1, _("Ping Timeout in ms :"), wxPoint(16,16), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticText2 = new wxStaticText(btnAddIP, ID_STATICTEXT2, _("Reply Timeout in ms :"), wxPoint(16,41), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText3 = new wxStaticText(btnAddIP, ID_STATICTEXT3, _("Polling Interval in ms :"), wxPoint(16,66), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(btnAddIP, ID_STATICTEXT4, _("Connection Port :"), wxPoint(16,89), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	txtPingTimeout = new wxTextCtrl(btnAddIP, ID_TEXTCTRL1, _("100"), wxPoint(160,16), wxSize(148,21), 0, wxTextValidator(wxFILTER_DIGITS), _T("ID_TEXTCTRL1"));
	txtReplyTimeout = new wxTextCtrl(btnAddIP, ID_TEXTCTRL2, _("2000"), wxPoint(160,40), wxSize(148,21), 0, wxTextValidator(wxFILTER_DIGITS), _T("ID_TEXTCTRL2"));
	txtPollingInterval = new wxTextCtrl(btnAddIP, ID_TEXTCTRL3, _("1000"), wxPoint(160,64), wxSize(148,21), 0, wxTextValidator(wxFILTER_DIGITS), _T("ID_TEXTCTRL3"));
	txtConnectionPort = new wxTextCtrl(btnAddIP, ID_TEXTCTRL4, _("3000"), wxPoint(160,88), wxSize(148,21), 0, wxTextValidator(wxFILTER_DIGITS), _T("ID_TEXTCTRL4"));
	btnSettingsCancel = new wxButton(btnAddIP, ID_BUTTON1, _("Cancel"), wxPoint(224,248), wxSize(83,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	btnSettingsOK = new wxButton(btnAddIP, ID_BUTTON2, _("OK"), wxPoint(128,248), wxSize(83,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	StaticText5 = new wxStaticText(btnAddIP, ID_STATICTEXT5, _("Static IP List : "), wxPoint(16,113), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	listIPList = new wxListBox(btnAddIP, ID_LISTBOX1, wxPoint(160,112), wxSize(148,120), 0, 0, wxLB_MULTIPLE, wxDefaultValidator, _T("ID_LISTBOX1"));
	BitmapButtonAddIP = new wxBitmapButton(btnAddIP, ID_BITMAPBUTTON1, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_NEW")),wxART_BUTTON), wxPoint(128,208), wxSize(25,25), wxBU_AUTODRAW|wxSIMPLE_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
	BitmapButtonAddIP->SetMinSize(wxSize(25,25));
	BitmapButtonAddIP->SetMaxSize(wxSize(25,25));
	BitmapButtonAddIP->SetToolTip(_("Add IP to list"));
	BitmapButtonDeleteIP = new wxBitmapButton(btnAddIP, ID_BITMAPBUTTON2, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_DELETE")),wxART_BUTTON), wxPoint(128,180), wxSize(25,25), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
	BitmapButtonDeleteIP->SetMinSize(wxSize(25,25));
	BitmapButtonDeleteIP->SetMaxSize(wxSize(25,25));
	BitmapButtonDeleteIP->SetToolTip(_("Delete IP From list"));
	chkEnableLogging = new wxCheckBox(btnAddIP, ID_CHECKBOX1, _("Log Enable"), wxPoint(16,253), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	chkEnableLogging->SetValue(true);
	Center();

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsDialog::OnbtnSettingsCancelClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsDialog::OnbtnSettingsOKClick);
	Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsDialog::OnBitmapButtonAddIPClick);
	Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsDialog::OnBitmapButtonDeleteIPClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&SettingsDialog::OnClose);
	//*)
	parentFrame =(NewFrame*) parent;
	parentFrame->readSettings();
	txtPingTimeout->SetValue(wxString::Format(wxT("%d"), (int)pingTimeOut));
	txtReplyTimeout->SetValue(wxString::Format(wxT("%d"), (int)replyTimeout));
	txtPollingInterval->SetValue(wxString::Format(wxT("%d"), (int)pollingInterval));
	txtConnectionPort->SetValue(wxString::Format(wxT("%d"), (int)port));
	if(StaticIPList.GetCount()>0)
    {
        listIPList->Clear();
        for(unsigned int i=0;i<StaticIPList.GetCount();i++)
        {
            listIPList->Append(StaticIPList[i].Trim());
        }
    }
    chkEnableLogging->SetValue(enableLogging);
}

SettingsDialog::~SettingsDialog()
{
	//(*Destroy(SettingsDialog)
	//*)
}


void SettingsDialog::OnbtnSettingsOKClick(wxCommandEvent& event)
{
    double value;
    if(txtPingTimeout->GetValue().ToDouble(&value)){ pingTimeOut = value; } else {pingTimeOut = 250;}
    if(txtReplyTimeout->GetValue().ToDouble(&value)){ replyTimeout = value; } else {replyTimeout = 2000;}
    if(txtPollingInterval->GetValue().ToDouble(&value)){ pollingInterval = value; } else {pollingInterval = 1000;}
    if(txtConnectionPort->GetValue().ToDouble(&value)){ port = value; } else {port = 3000;}
    if(chkEnableLogging->IsChecked()){enableLogging=true;} else {enableLogging=false;}
    StaticIPList.clear();
    for(unsigned int i=0;i<listIPList->GetCount();i++)
    {
        wxString data = listIPList->GetString(i);
        if(IsIPAddress(data))
        {
            StaticIPList.Add(data);
        }
    }
    parentFrame->saveSettings();
    parentFrame->btnSettings->Enable();
    AcceptAndClose();
}

void SettingsDialog::OnbtnSettingsCancelClick(wxCommandEvent& event)
{
    parentFrame->btnSettings->Enable();
    AcceptAndClose();
    //Destroy();
}

void SettingsDialog::OnClose(wxCloseEvent& event)
{
    parentFrame->btnSettings->Enable();
    AcceptAndClose();

}

void SettingsDialog::OnBitmapButtonAddIPClick(wxCommandEvent& event)
{
    wxTextEntryDialog *dlg = new wxTextEntryDialog(this, "Enter the IP address that you wist to add","IP input", "");
    if (dlg->ShowModal() == wxID_OK)
    {
        if(IsIPAddress(dlg->GetValue()))
            listIPList->Append(dlg->GetValue());
    }
        //return dlg.GetValue()
    dlg->Destroy();
}


void SettingsDialog::OnBitmapButtonDeleteIPClick(wxCommandEvent& event)
{
    wxArrayInt selection;
    listIPList->GetSelections(selection);
    if(selection.GetCount()>0)
    {
        StaticIPList.clear();
        for(unsigned int i=0;i<listIPList->GetCount();i++)
        {
            if(selection.Index(i)== wxNOT_FOUND)                         //the current index is not one of the indices choosen to be deleted
            {
				StaticIPList.Add(listIPList->GetString(i));
            }
        }
		listIPList->Clear();
		for (unsigned int i = 0;i<StaticIPList.GetCount();i++)
		{
			listIPList->Append(StaticIPList[i].Trim());
		}
    }
}
