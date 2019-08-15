#include "SettingsDialog.h"

//(*InternalHeaders(SettingsDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "settings.h"
#include "NewFrame.h"

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
	SetClientSize(wxSize(320,168));
	Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(320,176), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Ping Timeout in millisecond :"), wxPoint(16,16), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Reply Timeout in millisecond :"), wxPoint(16,41), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("Polling Interval in millisecond :"), wxPoint(16,66), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, _("Connection Port :"), wxPoint(16,89), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	txtPingTimeout = new wxTextCtrl(Panel1, ID_TEXTCTRL1, _("100"), wxPoint(208,16), wxSize(100,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	txtReplyTimeout = new wxTextCtrl(Panel1, ID_TEXTCTRL2, _("2000"), wxPoint(208,40), wxSize(100,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	txtPollingInterval = new wxTextCtrl(Panel1, ID_TEXTCTRL3, _("1000"), wxPoint(208,64), wxSize(100,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	txtConnectionPort = new wxTextCtrl(Panel1, ID_TEXTCTRL4, _("3000"), wxPoint(208,88), wxSize(100,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	btnSettingsCancel = new wxButton(Panel1, ID_BUTTON1, _("Cancel"), wxPoint(224,136), wxSize(83,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	btnSettingsOK = new wxButton(Panel1, ID_BUTTON2, _("OK"), wxPoint(128,136), wxSize(83,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Center();

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsDialog::OnbtnSettingsCancelClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsDialog::OnbtnSettingsOKClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&SettingsDialog::OnClose);
	//*)
	parentFrame =(NewFrame*) parent;
	parentFrame->readSettings();
	txtPingTimeout->SetValue(wxString::Format(wxT("%d"), (int)pingTimeOut));
	txtReplyTimeout->SetValue(wxString::Format(wxT("%d"), (int)replyTimeout));
	txtPollingInterval->SetValue(wxString::Format(wxT("%d"), (int)pollingInterval));
	txtConnectionPort->SetValue(wxString::Format(wxT("%d"), (int)port));
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
    parentFrame->saveSettings();
    parentFrame->btnSettings->Enable();
    Destroy();
}

void SettingsDialog::OnbtnSettingsCancelClick(wxCommandEvent& event)
{
    parentFrame->btnSettings->Enable();
    Destroy();
}

void SettingsDialog::OnClose(wxCloseEvent& event)
{
    parentFrame->btnSettings->Enable();
    Destroy();

}
