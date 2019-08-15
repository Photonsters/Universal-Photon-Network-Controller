#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

//(*Headers(SettingsDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class SettingsDialog: public wxDialog
{
	public:

		SettingsDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SettingsDialog();

		//(*Declarations(SettingsDialog)
		wxButton* btnSettingsCancel;
		wxButton* btnSettingsOK;
		wxPanel* Panel1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxTextCtrl* txtConnectionPort;
		wxTextCtrl* txtPingTimeout;
		wxTextCtrl* txtPollingInterval;
		wxTextCtrl* txtReplyTimeout;
		//*)

	protected:

		//(*Identifiers(SettingsDialog)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL1;
		static const long ID_TEXTCTRL2;
		static const long ID_TEXTCTRL3;
		static const long ID_TEXTCTRL4;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(SettingsDialog)
		void OnbtnSettingsOKClick(wxCommandEvent& event);
		void OnbtnSettingsCancelClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
