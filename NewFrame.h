// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#  pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif
#include "wx/url.h"
#include "wx/sstream.h"
#include <wx/process.h>
//(*Headers(NewFrame)
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/frame.h>
#include <wx/gauge.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/timer.h>
//*)
#include <wx/file.h>

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};
// Define a new frame type: this is going to be our main frame
class NewFrame : public wxFrame
{
public:

    NewFrame(wxFrame* parent, wxWindowID id = wxID_ANY, const  wxString Title = "", const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
    virtual ~NewFrame();
    void saveSettings();
    void readSettings();
    void processInput(wxString);
    void getPrintStatus();
    //(*Declarations(NewFrame)
    wxBitmapButton* btnSearchPrinter;
    wxButton* btnConnect;
    wxButton* btnDelete;
    wxButton* btnDownload;
    wxButton* btnPause;
    wxButton* btnRefresh;
    wxButton* btnSettings;
    wxButton* btnStart;
    wxButton* btnStop;
    wxButton* btnUpload;
    wxComboBox* comboIP;
    wxGauge* PrintProgress;
    wxGauge* progressFile;
    wxListCtrl* ListCtrl1;
    wxPanel* Panel1;
    wxStaticBox* StaticBox1;
    wxStaticBox* StaticBox2;
    wxStaticBox* StaticBox3;
    wxStaticText* StaticText1;
    wxStaticText* lblPercentDone;
    wxStaticText* lblStatus;
    wxStatusBar* StatusBar1;
    wxTimer PollTimer;
    wxTimer ProcessPollTimer;
    wxTimer WatchDogTimer;
    //*)
    //static wxString[5] SizeSuffixes =  { "bytes", "KB", "MB", "GB", "TB"};
    bool ispaused = false;
    bool isPrinting = false;
    double progressBefore = -9999;
    int framenumber = 0;
    bool isConnected = false;
    int tickCount = 0;
    wxString msges[3];
    int exitstatus;
    bool isPingRunning=false;
protected:

    //(*Identifiers(NewFrame)
    static const long ID_STATICBOX1;
    static const long ID_STATICTEXT1;
    static const long ID_BUTTON1;
    static const long ID_STATICTEXT2;
    static const long ID_STATICBOX2;
    static const long ID_BUTTON2;
    static const long ID_BUTTON3;
    static const long ID_BUTTON4;
    static const long ID_GAUGE1;
    static const long ID_STATICTEXT3;
    static const long ID_STATICBOX3;
    static const long ID_BUTTON5;
    static const long ID_BUTTON6;
    static const long ID_BUTTON7;
    static const long ID_BUTTON8;
    static const long ID_GAUGE2;
    static const long ID_LISTCTRL1;
    static const long ID_BUTTON9;
    static const long ID_BITMAPBUTTON1;
    static const long ID_COMBOBOX1;
    static const long ID_PANEL1;
    static const long ID_TIMER1;
    static const long ID_STATUSBAR1;
    static const long ID_TIMER2;
    static const long ID_TIMER3;
    //*)
private:

    //(*Handlers(NewFrame)
    void OnbtnConnectClick(wxCommandEvent& event);
    void OnbtnStartClick(wxCommandEvent& event);
    void OnbtnPauseClick(wxCommandEvent& event);
    void OnbtnStopClick(wxCommandEvent& event);
    void OnbtnDeleteClick(wxCommandEvent& event);
    void OnbtnRefreshClick(wxCommandEvent& event);
    void OnbtnUploadClick(wxCommandEvent& event);
    void OnbtnDownloadClick(wxCommandEvent& event);
    void OnPollTimerTrigger(wxTimerEvent& event);
    void OnWatchDogTimerTrigger(wxTimerEvent& event);
    void OnbtnSettingsClick(wxCommandEvent& event);
    void OnProcessPollTimerTrigger(wxTimerEvent& event);
    void OnbtnSearchPrinterClick(wxCommandEvent& event);
    //*)
    void getAsyncReply();
    void OnMyThread(wxCommandEvent& event);
    void OnProcessTerm( wxProcessEvent& event );
    void getVersion();
    void getBlockingReply();
    void handleResponse();
    void sendCmdToPrinter(wxString);
	void broadcastOverUDP(wxString,uint8_t*, unsigned int);
	wxArrayString getBlockingBroadcastReply(wxString);
    void sendCmdToPrinter(uint8_t*, unsigned int);
    void updatefileList();
    bool connectToPrinter(wxString);
    void disconnectFromPrinter();
    void setStatusMessages(wxString, wxString, wxString);
    wxString isError(wxString);
    void clearListControl();
    void AsyncPingPrinter(wxString,int);
    void OnProcessTimer(wxTimerEvent&);
    bool isconnected = false;
    bool startList = false;
    bool endList = false;
    int fileCount = 0;
    wxFile* photonFile;
    wxString photonFileName;
    wxString photonFilePath;
    unsigned int beforeByte = 0;
    unsigned int frameNum = 0;
    unsigned int numDownloadRetries = 0;
    bool downloadStarted = false;
    ssize_t downloadFileLength = -1;
    ssize_t downloadFileCurrentLength = -1;
    bool pingFailed=false;
	wxIPV4address *m_BroadCastAddress; // For broadcast sending
	wxIPV4address m_LocalAddress;     // For listening
	wxDatagramSocket* m_Listen_Socket;
    DECLARE_EVENT_TABLE()
};

class DisplayProcess : public wxProcess         // Executes a command, displaying output in a textctrl.  Adapted from the exec sample
{
    // Very similar to MyPipedProcess in Tools.cpp, but enough differences to make derivation difficult
public:
    DisplayProcess(NewFrame* parentFrame):  wxProcess(wxPROCESS_REDIRECT)
{
    m_parent = parentFrame;
};
    bool HasInput();
    void SetPid(long pid)
    {
        m_pid = pid;
    }

protected:
    void OnTerminate(int pid, int status);
    long m_pid;                                     // Stores the process's pid, in case we want to kill it
    bool m_WasCancelled;
    NewFrame* m_parent;
};
