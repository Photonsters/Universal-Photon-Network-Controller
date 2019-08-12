#include "NewFrame.h"
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <wx/file.h>
#include <wx/process.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <stdlib.h>
#include "MyThread.h"
#include "ping.h"

#define RECV_BUFFER_LENGTH 1290
wxDatagramSocket *sock;
wxIPV4address *addrPeer;
uint8_t receivedBuf[RECV_BUFFER_LENGTH];
wxUint32 numRead = -1;
bool isRunning = false;


DEFINE_EVENT_TYPE(wxEVT_READTHREAD)
MyThread::MyThread(wxEvtHandler* pParent) : wxThread(wxTHREAD_DETACHED), m_pParent(pParent)
{
    //pass parameters into the thread
    //addrLocal1
    //m_param = param;
    //sock.SetRefData(s);
}
void* MyThread::Entry()
{
    try
    {
        wxCommandEvent evt(wxEVT_READTHREAD, GetId());
        //can be used to set some identifier for the data
        memset(&receivedBuf[0], 0, sizeof(receivedBuf));
        isRunning = true;
        //if (sock->IsConnected())
        //{
        if (sock->IsOk())
        {
            sock->RecvFrom(*addrPeer, receivedBuf, RECV_BUFFER_LENGTH);
            numRead = sock->LastCount();
        }
        //}
        evt.SetInt(0);
        isRunning = false;
        //whatever data your thread calculated, to be returned to GUI
        //evt.SetClientData((void*)temp);
        wxPostEvent(m_pParent, evt);
        return 0;
    }
    catch (int e)
    {
        isRunning = true;
        return 0;
    }
}

MyThread *sockThread;

class GcodeCommandClass
{
private:
    wxString cmd;
    wxString params;
public:
    GcodeCommandClass(wxString Command = "", wxString Parameters = "")
    {
        cmd = Command;
        params = Parameters;
    }
    wxString getCommand()
    {
        return cmd;
    }
    wxString getParameter()
    {
        return params;
    }
    void setCommand(wxString Command)
    {
        cmd = Command;
    }
    void setParameter(wxString Parameters)
    {
        params = Parameters;
    }
    wxString getGcodeCmd(wxString Delimater = " ")
    {
        return cmd + Delimater + params;
    }
    bool isCmd(wxString Command)
    {
        if (cmd.CmpNoCase(Command) == 0)
            return true;
        return false;
    }
};

GcodeCommandClass gcodeCmd;

// --------------------------------------------------------------------------
// resources
// --------------------------------------------------------------------------

// the application icon
//#ifndef wxHAS_IMAGES_IN_RESOURCES
//#include "../sample.xpm"
//#endif

//(*IdInit(NewFrame)
const long NewFrame::ID_STATICBOX1 = wxNewId();
const long NewFrame::ID_TEXTCTRL1 = wxNewId();
const long NewFrame::ID_STATICTEXT1 = wxNewId();
const long NewFrame::ID_BUTTON1 = wxNewId();
const long NewFrame::ID_STATICTEXT2 = wxNewId();
const long NewFrame::ID_STATICBOX2 = wxNewId();
const long NewFrame::ID_BUTTON2 = wxNewId();
const long NewFrame::ID_BUTTON3 = wxNewId();
const long NewFrame::ID_BUTTON4 = wxNewId();
const long NewFrame::ID_GAUGE1 = wxNewId();
const long NewFrame::ID_STATICTEXT3 = wxNewId();
const long NewFrame::ID_STATICBOX3 = wxNewId();
const long NewFrame::ID_BUTTON5 = wxNewId();
const long NewFrame::ID_BUTTON6 = wxNewId();
const long NewFrame::ID_BUTTON7 = wxNewId();
const long NewFrame::ID_BUTTON8 = wxNewId();
const long NewFrame::ID_GAUGE2 = wxNewId();
const long NewFrame::ID_LISTCTRL1 = wxNewId();
const long NewFrame::ID_PANEL1 = wxNewId();
const long NewFrame::ID_TIMER1 = wxNewId();
const long NewFrame::ID_STATUSBAR1 = wxNewId();
const long NewFrame::ID_TIMER2 = wxNewId();
//*)


// --------------------------------------------------------------------------
// constants
// --------------------------------------------------------------------------

// IDs for the controls and the menu commands


// --------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// --------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(NewFrame, wxFrame)
//(*EventTable(NewFrame)
//*)
    EVT_COMMAND(wxID_ANY, wxEVT_READTHREAD, NewFrame::OnMyThread)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

// ==========================================================================
// implementation
// ==========================================================================

// --------------------------------------------------------------------------
// the application class
// --------------------------------------------------------------------------


bool MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    // Create the main application window
    NewFrame *frame = new NewFrame((wxFrame *)NULL, wxID_ANY, _("Photon Network Controller"), wxDefaultPosition, wxSize(300, 200));

#if defined(__WINDOWS__)
    frame->SetIcon(wxICON(aaaa));
#else
    char path[ PATH_MAX ];
    char dest[PATH_MAX];
    memset(dest,0,sizeof(dest));
    pid_t pid = getpid();
    sprintf(path, "/proc/%d/exe", pid);
    ssize_t PathLen = readlink( path, dest, PATH_MAX );
    if(PathLen!=-1)
        frame->SetIcon(wxIcon(wxString::Format(wxT("%s.xpm"), dest)));
#endif // defined

    // Show it
    frame->Show(true);

    // success

    return true;
}

// --------------------------------------------------------------------------
// main frame
// --------------------------------------------------------------------------

// frame constructor
NewFrame::NewFrame(wxFrame* parent, wxWindowID id, wxString title, const wxPoint& pos, const wxSize& size)
{
    //    HICON hIcon;
    //    hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(2));
    //    SetClassLong(hWnd, GCL_HICON, (LONG) hIcon); // The new icon
    //    PostMessage (hWnd, WM_SETICON, ICON_BIG, (LPARAM) hIcon);

    //(*Initialize(NewFrame)
    Create(parent, wxID_ANY, _("Photon Network Controller"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX, _T("wxID_ANY"));
    SetClientSize(wxSize(334,491));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(224,320), wxSize(334,488), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, _("Connection Settings"), wxPoint(8,8), wxSize(320,80), 0, _T("ID_STATICBOX1"));
    txtIP = new wxTextCtrl(Panel1, ID_TEXTCTRL1, _("192.168.1.222"), wxPoint(208,24), wxSize(112,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("IP Address"), wxPoint(16,28), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    btnConnect = new wxButton(Panel1, ID_BUTTON1, _("Connect"), wxPoint(235,54), wxSize(85,26), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    lblStatus = new wxStaticText(Panel1, ID_STATICTEXT2, _("Not Connected"), wxPoint(16,60), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    StaticBox2 = new wxStaticBox(Panel1, ID_STATICBOX2, _("Print Status"), wxPoint(8,96), wxSize(320,112), 0, _T("ID_STATICBOX2"));
    btnStart = new wxButton(Panel1, ID_BUTTON2, _("Start"), wxPoint(16,118), wxSize(85,26), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    btnPause = new wxButton(Panel1, ID_BUTTON3, _("Pause"), wxPoint(127,118), wxSize(85,26), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    btnStop = new wxButton(Panel1, ID_BUTTON4, _("Stop"), wxPoint(235,118), wxSize(85,26), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    PrintProgress = new wxGauge(Panel1, ID_GAUGE1, 1000, wxPoint(16,170), wxSize(304,24), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    lblPercentDone = new wxStaticText(Panel1, ID_STATICTEXT3, _("Percent Done"), wxPoint(16,152), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticBox3 = new wxStaticBox(Panel1, ID_STATICBOX3, _("Files"), wxPoint(8,216), wxSize(320,251), 0, _T("ID_STATICBOX3"));
    btnDelete = new wxButton(Panel1, ID_BUTTON5, _("Delete"), wxPoint(16,392), wxSize(85,26), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    btnRefresh = new wxButton(Panel1, ID_BUTTON6, _("Refresh"), wxPoint(125,392), wxSize(85,26), 0, wxDefaultValidator, _T("ID_BUTTON6"));
    btnUpload = new wxButton(Panel1, ID_BUTTON7, _("Upload"), wxPoint(235,392), wxSize(85,26), 0, wxDefaultValidator, _T("ID_BUTTON7"));
    btnDownload = new wxButton(Panel1, ID_BUTTON8, _("Download"), wxPoint(235,432), wxSize(85,26), 0, wxDefaultValidator, _T("ID_BUTTON8"));
    progressFile = new wxGauge(Panel1, ID_GAUGE2, 100, wxPoint(16,433), wxSize(212,24), 0, wxDefaultValidator, _T("ID_GAUGE2"));
    ListCtrl1 = new wxListCtrl(Panel1, ID_LISTCTRL1, wxPoint(16,232), wxSize(304,152), wxLC_REPORT|wxLC_SINGLE_SEL|wxSUNKEN_BORDER, wxDefaultValidator, _T("ID_LISTCTRL1"));
    PollTimer.SetOwner(this, ID_TIMER1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -10 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    WatchDogTimer.SetOwner(this, ID_TIMER2);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewFrame::OnbtnConnectClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewFrame::OnbtnStartClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewFrame::OnbtnPauseClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewFrame::OnbtnStopClick);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewFrame::OnbtnDeleteClick);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewFrame::OnbtnRefreshClick);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewFrame::OnbtnUploadClick);
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewFrame::OnbtnDownloadClick);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&NewFrame::OnPollTimerTrigger);
    Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&NewFrame::OnWatchDogTimerTrigger);
    //*)
    setStatusMessages(_("Not Connected"), "", "");
    readSettings();
}

wxString convertSize(size_t size)
{
    const char *SIZES[] = { _("Bytes"), _("KB"), _("MB"), _("GB") };
    unsigned int div = 0;
    size_t rem = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES))
    {
        rem = (size % 1024);
        div++;
        size /= 1024;
    }

    double size_d = (float)size + (float)rem / 1024.0;
    wxString result;
    unsigned int FileSize = round(size_d);
    result.Printf("%d %s", FileSize, SIZES[div]);
    return result;
}


double getValue(wxString str, wxString prefix, double default_val, wxString ahead)
{
    try
    {
        int startIndex = 0;
        if (ahead != "")
        {
            startIndex = str.Find(ahead);
            if (startIndex != -1)
            {
                startIndex += ahead.Length();
            }
        }
        int index = str.substr(startIndex).Find(prefix);
        if (index != wxNOT_FOUND)
        {
            index += prefix.Length();
            int length = 0;
            //char* chArray = str.ToCharArray();
            char* chArray = (char*)malloc(sizeof(char)*(str.Length()));
            strcpy(chArray, (const char*)str.mb_str(wxConvUTF8));
            for (unsigned int i = index; i < str.Length(); i++)
            {
                char ch = chArray[i];
                if (((ch < '0') || (ch > '9')) && ((ch != '.') && (ch != '-')))
                {
                    break;
                }
                length++;
            }
            if (length > 0)
            {
                double value;
                if (!str.substr(index, length).ToDouble(&value))
                    return -1;
                else
                    return value;
            }
            return default_val;
        }
        return default_val;
    }
    catch(int e)
    {
        return default_val;
    }
}

double getValue(wxString str, wxString prefix, double default_val)
{
    return getValue(str, prefix, default_val, "");
}

NewFrame::~NewFrame()
{
    //(*Destroy(NewFrame)
    //*)
}
bool NewFrame::connectToPrinter(wxString hostname)
{
    if(pingPrinter(hostname))
    {
        wxIPV4address addrLocal;
        addrLocal.Hostname();
        sock = new wxDatagramSocket(addrLocal);
        //wxDatagramSocket sock(addrLocal);
        if (!sock->IsOk())
        {
            wxMessageBox(_("Failed to create UDP socket"), _("Error"), wxICON_ERROR);
            return false;
        }


        //wxIPV4address addrPeer;
        free(addrPeer);
        addrPeer = NULL;
        addrPeer = new wxIPV4address;
        addrPeer->Hostname(hostname);
        addrPeer->Service(3000);
        isconnected = false;
        startList = false;
        endList = false;
        fileCount = 0;
        photonFile = NULL;
        photonFileName = wxEmptyString;
        photonFilePath = wxEmptyString;
        beforeByte = 0;
        frameNum = 0;
        numDownloadRetries = 0;
        downloadStarted = false;
        downloadFileLength = -1;
        downloadFileCurrentLength = -1;
        btnStart->Enable();
        PollTimer.Stop();
        WatchDogTimer.Stop();
        return true;
    }
    else
    {
        wxMessageBox(_("The address cannot be reached."), _("Error"), wxICON_ERROR);
        return false;
    }

}
void NewFrame::disconnectFromPrinter()
{
    try
    {
        if (isRunning)
        {
            sockThread->Kill();
            //free(sockThread);
            sockThread = NULL;
            isRunning = false;
        }
        sock->Close();
        free(addrPeer);
        addrPeer = NULL;
        isconnected = false;
        startList = false;
        endList = false;
        fileCount = 0;
        photonFile = NULL;
        photonFileName = wxEmptyString;
        photonFilePath = wxEmptyString;
        beforeByte = 0;
        frameNum = 0;
        numDownloadRetries = 0;
        downloadStarted = false;
        downloadFileLength = -1;
        downloadFileCurrentLength = -1;
        PollTimer.Stop();
        WatchDogTimer.Stop();
        setStatusMessages(_("Not Connected"), "", "");
        btnConnect->SetLabel(_("Connect"));
        lblStatus->SetLabel(_("Not Connected"));
        clearListControl();
        progressFile->SetValue(0);
        PrintProgress->SetValue(0);
        btnStart->Disable();
    }
    catch (int e) {}

}
void NewFrame::setStatusMessages(wxString message1, wxString message2, wxString message3)
{
    if (message1 != "prev")
        msges[0] = message1;
    if (message2 != "prev")
        msges[1] = message2;
    if (message3 != "prev")
        msges[2] = message3;
    SetStatusText(msges[0] + " " + msges[1] + " " + msges[2], 0);
}
void NewFrame::sendCmdToPrinter(wxString cmd)
{
    try
    {
        memset(&receivedBuf[0], 0, sizeof(receivedBuf));
        wxUint32 sendSize = cmd.Length();
        char* tempBuffer = (char*)malloc(sizeof(char)*(2 + sendSize));
        strcpy(tempBuffer, (const char*)cmd.mb_str(wxConvUTF8)); // buf will now contain the command
        tempBuffer[sendSize] = '\0';
        tempBuffer[sendSize + 1] = '\0';
        WatchDogTimer.Start(2000, true);
        if (sock->SendTo(*addrPeer, tempBuffer, sendSize).LastCount() != sendSize)
        {
            wxMessageBox(_("Failed to send data"), _("Error"), wxICON_ERROR);
            return;
        }
        free(tempBuffer);
    }
    catch(int e)
    {
        wxMessageBox(_("Failed to send data"), _("Error"), wxICON_ERROR);
    }

}

void NewFrame::sendCmdToPrinter(uint8_t* cmd, unsigned int sendSize)
{
    try
    {
        memset(&receivedBuf[0], 0, sizeof(receivedBuf));
        WatchDogTimer.Start(2000, true);
        if (sock->SendTo(*addrPeer, cmd, sendSize).LastCount() != sendSize)
        {
            wxMessageBox(_("failed to send data"), _("Error"), wxICON_ERROR);
            return;
        }
    }
    catch(int e)
    {
        wxMessageBox(_("failed to send data"), _("Error"), wxICON_ERROR);
    }
}

void NewFrame::getAsyncReply()
{
    try
    {
        sockThread = new MyThread(this);
        sockThread->Create();
        sockThread->Run();
    }
    catch (int e) {}
}

void NewFrame::getBlockingReply()
{
    try
    {
        memset(&receivedBuf[0], 0, sizeof(receivedBuf));
        numRead = sock->RecvFrom(*addrPeer, receivedBuf, sizeof(receivedBuf)).LastCount();
        WatchDogTimer.Stop();
    }
    catch (int e) {}
}

void NewFrame::clearListControl()
{
    ListCtrl1->ClearAll();
    ListCtrl1->DeleteAllColumns();
    //The two lines below is only needed for GCC under windows. It works fine without this in all other OSes
    ListCtrl1->~wxListCtrl();
    ListCtrl1 = new wxListCtrl(Panel1, ID_LISTCTRL1, wxPoint(16, 232), wxSize(304, 152), wxLC_REPORT | wxLC_SINGLE_SEL | wxSUNKEN_BORDER, wxDefaultValidator, _T("ID_LISTCTRL1"));
    //Under Windows GCC the list fails to redraw after deleting items in it. The above workaround is the easiest way to fix that issue
    //comment them out for better performance under other platforms
}

void NewFrame::handleResponse()
{
    WatchDogTimer.Stop();
    if (gcodeCmd.isCmd("M4002"))       //Gcode to read version info from printer
    {
        wxString tempStr = receivedBuf;
        lblStatus->SetLabel(_("Connected"));
        btnConnect->SetLabel(_("Disconnect"));
        isconnected = true;
        setStatusMessages(tempStr.substr(3).Trim(), "", "");
        isConnected = true;
        while (sock->IsData())               //get rid of extra messages from the sockets buffer as it will confuse the program
            getBlockingReply();
        updatefileList();
    }
    else if (gcodeCmd.isCmd("M20"))       //Gcode to read the filelist
    {
        wxString receivedText = receivedBuf;
        receivedText.Trim();
        if (!receivedText.Contains("Begin file list"))
        {
            if (!startList)  //if the file listing has not started then read again
            {
                getAsyncReply();
            }
            else
            {
                if (!receivedText.Contains("End file list"))       //File listing has not ended
                {
                    wxStringTokenizer temp(receivedText, " ");
                    wxString temp1 = "";
                    while (temp.HasMoreTokens())
                    {
                        wxString token = temp.GetNextToken();
                        temp1 = temp1 + token + " ";
                        if (temp.CountTokens() == 1)
                            break;
                    }
                    wxString token = temp.GetNextToken();

                    long value;
                    if (token.ToLong(&value))
                    {
                        if (value > 0)
                        {
                            ListCtrl1->InsertItem(fileCount, wxString((char)(fileCount + 49)));
                            ListCtrl1->SetItem(fileCount, 1, temp1);
                            ListCtrl1->SetItem(fileCount, 2, convertSize(value));
                            fileCount++;
                        }
                    }
                    else
                    {
                        ListCtrl1->InsertItem(fileCount, wxString((char)(fileCount + 49)));
                        ListCtrl1->SetItem(fileCount, 1, temp1);
                        ListCtrl1->SetItem(fileCount, 2, _("Unknown"));
                        fileCount++;
                    }

                    getAsyncReply();
                }
                else
                {
                    startList = false;                    //File list ended don't read any more lines
                    endList = true;
                }
            }
        }
        else
        {
            startList = true;       //File Listing just Started
            endList = false;
            clearListControl();
            ListCtrl1->AppendColumn(_("Num"), wxLIST_FORMAT_LEFT, 50);
            ListCtrl1->AppendColumn(_("File Name"), wxLIST_FORMAT_LEFT, 175);
            ListCtrl1->AppendColumn(_("Size"), wxLIST_FORMAT_LEFT, 75);
            fileCount = 0;
            getAsyncReply();
        }
        if (endList)     //Received end of file list in the last command so now clean up rest of the receive buffer
        {
            getBlockingReply();
        }
        //else if (endList>=1)
        //{
        //get rid of extra messages from the sockets buffer as it will confuse the program
        //}
    }
    else if (gcodeCmd.isCmd("M6030"))       //Gcode to Start Printing
    {
        ispaused = false;
        isPrinting = true;
        btnPause->Enable();
        btnStart->Disable();
        btnStop->Enable();
        gcodeCmd.setCommand("M27");
        gcodeCmd.setParameter("");
        beforeByte = 0;
        frameNum = 0;
        PollTimer.Start(1000);
        setStatusMessages("prev", _("Printing"), "");
        PrintProgress->SetValue(0);
        //wxMessageBox(wxString::Format(wxT("%s"), receivedText));
    }
    else if (gcodeCmd.isCmd("M6032"))       //Gcode to Download File
    {
        if (!downloadStarted)                //If the download has not started do the following check
        {
            wxString receivedText = receivedBuf;
            receivedText.Trim();
            wxString errorIfAny = isError(receivedText);
            if (errorIfAny.Length() <= 0)       //No error so we can proceed to the actual file download
            {
                downloadFileLength = getValue(receivedText, "L:", -1);
                if (downloadFileLength>0)        //it is a valid file with a positive file length
                {
                    downloadStarted = true;           //Se this flag that way you know the download has started and we need to process the incoming data.
                }
                else
                {
                    gcodeCmd.setCommand("M22");
                    sendCmdToPrinter(gcodeCmd.getCommand());
                    getAsyncReply();
                }
            }
            else
            {
                if (numDownloadRetries == 0)       //retry only once by sending a M22
                {
                    numDownloadRetries++;
                    gcodeCmd.setCommand("M22");
                    sendCmdToPrinter(gcodeCmd.getCommand());
                    getAsyncReply();
                }
                else        //It errored out after one retry just give up and show the error message
                {
                    wxMessageBox(errorIfAny, _("Error"), wxICON_ERROR);
                }
            }
        }
        if (downloadStarted)            //Download has started. Issue the M3000 command to start the download process
        {
            while (sock->IsData())               //get rid of extra messages from the sockets buffer as it will confuse the program
                getBlockingReply();
            if (downloadFileCurrentLength<downloadFileLength)        //Should be but still check if that's the case or not
            {
                progressFile->SetValue(0);
                downloadFileCurrentLength = 0;
                gcodeCmd.setCommand("M3000");
                sendCmdToPrinter(gcodeCmd.getCommand());
                getAsyncReply();
            }
            else
            {
                gcodeCmd.setCommand("M22");
                sendCmdToPrinter(gcodeCmd.getCommand());
                getAsyncReply();
            }
        }
        //wxMessageBox(wxString::Format(wxT("%s"), receivedText));
    }
    else if (gcodeCmd.isCmd("M3000"))       //Gcode to start downlading data from the printer or request resend of corrupted data
    {
        if (downloadFileCurrentLength < downloadFileLength)                 //there is still more data left to be read
        {
            if ((numRead >= 6) && ((uint8_t)(receivedBuf[numRead - 1]) == 0x83))
            {
                unsigned int maxValue = 0;
                maxValue = (maxValue << 8) + receivedBuf[numRead - 3];
                maxValue = (maxValue << 8) + receivedBuf[numRead - 4];
                maxValue = (maxValue << 8) + receivedBuf[numRead - 5];
                maxValue = (maxValue << 8) + receivedBuf[numRead - 6];
                if (maxValue == downloadFileCurrentLength)
                {
                    uint8_t checkSum = 0;              // This is the checksum
                    for (unsigned int i = 0; i < (numRead - 2); i++)
                        checkSum = (uint8_t)(checkSum ^ receivedBuf[i]);
                    if (checkSum == receivedBuf[numRead - 2])          // if the calculated checksum is the same as the one we received from the printer
                    {
                        //Write this data to file
                        photonFile->Write(receivedBuf, numRead - 6);
                        downloadFileCurrentLength += numRead - 6;
                        progressFile->SetValue((int)((100 * downloadFileCurrentLength) / downloadFileLength));
                        progressFile->Update();
                        gcodeCmd.setCommand("M3000");
                        sendCmdToPrinter(gcodeCmd.getCommand());
                        getAsyncReply();

                    }
                    else       //request resend
                    {
                        gcodeCmd.setCommand("M3001");
                        gcodeCmd.setParameter(wxString('I') + wxString::Format(wxT("%lld"), downloadFileCurrentLength));
                        sendCmdToPrinter(gcodeCmd.getGcodeCmd());
                        getAsyncReply();
                    }
                }
                else            //request resend
                {
                    gcodeCmd.setCommand("M3001");
                    gcodeCmd.setParameter(wxString('I') + wxString::Format(wxT("%lld"), downloadFileCurrentLength));
                    sendCmdToPrinter(gcodeCmd.getGcodeCmd());
                    getAsyncReply();
                }

            }
            else
            {
                downloadStarted = false;
                numDownloadRetries++;
                gcodeCmd.setCommand("M22");
                sendCmdToPrinter(gcodeCmd.getCommand());
                getAsyncReply();
            }
        }
        else
        {
            numDownloadRetries = 0;               //just to ensure that it closes the file and updates the flag;
            gcodeCmd.setCommand("M22");
            sendCmdToPrinter(gcodeCmd.getCommand());
            getAsyncReply();
        }

        //wxMessageBox(wxString::Format(wxT("%s"), receivedText));
    }
    else if (gcodeCmd.isCmd("M22"))       //Gcode to Stop downloading file
    {
        if (numDownloadRetries == 1)               //Request file re download if and only if it's retry number 1. In case of no retry i.e. 0 finish the process and in case of more than one i.e. >1 just give up and show an error message
        {
            gcodeCmd.setCommand("M6032");       // The gcode parameter should be same as before which is the file name
            sendCmdToPrinter(gcodeCmd.getGcodeCmd());
            getAsyncReply();
        }
        else
        {
            photonFile->Flush();
            photonFile->Close();
            if (downloadFileCurrentLength >= downloadFileLength)
                wxMessageBox(_("Download finished."), _("Information"), wxOK | wxICON_INFORMATION | wxCENTER);
            else
                wxMessageBox(_("Failed to download file. Please retry"), _("Warning"), wxOK | wxICON_EXCLAMATION | wxCENTER);
            downloadStarted = false;
            downloadFileLength = -1;
            downloadFileCurrentLength = -1;

        }
        //do Nothing
        //wxMessageBox(wxString::Format(wxT("%s"), receivedText));
    }
    else if (gcodeCmd.isCmd("M25"))       //Gcode to Pause Printing
    {
        ispaused = true;
        isPrinting = false;
        btnPause->SetLabel(_("Resume"));
        btnPause->Update();
        PollTimer.Stop();
        setStatusMessages("prev", _("Paused"), "");
        //wxMessageBox(wxString::Format(wxT("%s"), receivedText));
    }
    else if (gcodeCmd.isCmd("M27"))       //Gcode for getting print status
    {
        wxString receivedText = receivedBuf;
        while (sock->IsData())               //get rid of extra messages from the sockets buffer as it will confuse the program
            getBlockingReply();
        wxString errorIfAny = isError(receivedText);
        if (errorIfAny.Length() <= 0)
        {
            wxStringTokenizer temp(receivedText.Trim(), " ");
            wxString token;
            while (temp.HasMoreTokens())        //get to the last token that is the received message. it should have the format bytes_printed/total_bytes
                token = temp.GetNextToken();
            wxStringTokenizer temp1(token.Trim(), "/");
            unsigned long bytesPrinted = 0;
            unsigned long totalBytes = 0;
            temp1.GetNextToken().ToCULong(&bytesPrinted);
            temp1.GetNextToken().ToCULong(&totalBytes);
            if (beforeByte<bytesPrinted)
            {
                frameNum++;
                beforeByte = bytesPrinted;
            }
            //wxMessageBox(token + wxString::Format(wxT("  --  %ld/%ld"), bytesPrinted,totalBytes),"Information",wxOK|wxICON_INFORMATION|wxCENTER);
            double percentDone = ((bytesPrinted * 1000) / totalBytes);
            if (percentDone >= 1000)
            {
                PollTimer.Stop();
                btnStart->Enable();
                btnStop->Disable();
                btnPause->Disable();
                setStatusMessages("prev", _("Print"), _("finished."));
                lblPercentDone->SetLabel(wxString(_("Percent Done : 100")));
                PrintProgress->SetValue(1000);
                PrintProgress->Update();
            }
            else
            {
                lblPercentDone->SetLabel(wxString(_("Percent Done : ")) + wxString::Format(wxT("%d"), (int)round(percentDone / 10)));
                PrintProgress->SetValue((int)round(percentDone));
                PrintProgress->Update();
                setStatusMessages("prev", _("Printing"), wxString(_("Layer ")) + wxString::Format(wxT("%u"), frameNum));
            }
        }
        else
        {
            PollTimer.Stop();
            setStatusMessages("prev", _("Not Printing"), "");
        }

        //wxMessageBox(wxString::Format(wxT("%s"), receivedText),"Information",wxOK|wxICON_INFORMATION|wxCENTER);
    }
    else if (gcodeCmd.isCmd("M24"))       //Gcode to resume Printing
    {
        ispaused = false;
        isPrinting = true;
        btnPause->SetLabel("Pause");
        btnPause->Update();
        gcodeCmd.setCommand("M27");
        gcodeCmd.setParameter("");
        PollTimer.Start(1000);
        setStatusMessages("prev", _("Printing"), "");
        //wxMessageBox(wxString::Format(wxT("%s"), receivedText));
    }
    else if (gcodeCmd.isCmd("M33"))         //Stopping a print
    {
        wxString receivedText = receivedBuf;
        while (sock->IsData())               //get rid of extra messages from the sockets buffer as it will confuse the program
            getBlockingReply();
        wxString errorIfAny = isError(receivedText);
        if (errorIfAny.Length() <= 0)
        {
            gcodeCmd.setCommand("M29");
            gcodeCmd.setParameter("");
            sendCmdToPrinter(gcodeCmd.getCommand());
            getAsyncReply();
        }
        else
        {
            wxMessageBox(errorIfAny, _("Error"), wxICON_ERROR);
        }



    }
    else if (gcodeCmd.isCmd("M29"))         //Stopping a print or a file upload
    {
        //gcodeCmd.setCommand("G90, G0");
        //gcodeCmd.setParameter("Z 150 F300");
        //sendCmdToPrinter(gcodeCmd.getGcodeCmd());
        wxString receivedText = receivedBuf;
        if (receivedText.Length()>8)         //Don't show the OK N:6 message that you get on stopping of a print
            wxMessageBox(wxString::Format(wxT("%s"), receivedText), _("Information"), wxOK | wxICON_INFORMATION | wxCENTER);
        else
        {
            setStatusMessages("prev", _("Stopped"), "");
            btnPause->Disable();
            btnStart->Enable();
            btnStop->Enable();
            ispaused = false;
            isPrinting = false;
            PollTimer.Stop();
        }

        while (sock->IsData())               //get rid of extra messages from the sockets buffer as it will confuse the program
            getBlockingReply();
        updatefileList();

    }
    else if (gcodeCmd.isCmd("M30"))         //Delete a file
    {
        //gcodeCmd.setCommand("G90, G0");
        //gcodeCmd.setParameter("Z 150 F300");
        //sendCmdToPrinter(gcodeCmd.getGcodeCmd());
        wxString receivedText = receivedBuf;
        wxMessageBox(wxString::Format(wxT("%s"), receivedText), _("Information"), wxOK | wxICON_INFORMATION | wxCENTER);
        while (sock->IsData())               //get rid of extra messages from the sockets buffer as it will confuse the program
            getBlockingReply();
        updatefileList();
    }
    else if (gcodeCmd.isCmd("M28"))         //Uploading a File
    {
        wxString receivedText = receivedBuf;
        //wxMessageBox(wxString::Format(wxT("%s"), receivedText));
        int chunckSize = 0x500;
        uint8_t *buffer = (uint8_t*)malloc((chunckSize + 6) * sizeof(uint8_t));
        ssize_t index = 0;
        if (receivedText.Find("resend") != wxNOT_FOUND)
        {
            long resend_index = getValue(receivedText, "resend ", -1);
            photonFile->Seek(resend_index, wxFromStart);
        }
        if (photonFile->Tell() < photonFile->Length())
        {
            unsigned int position = photonFile->Tell();
            index = photonFile->Read(buffer, chunckSize);
            if (index != wxInvalidOffset)
            {
                uint8_t num8 = 0;
                buffer[index] = (uint8_t)position & 0xFF;
                buffer[index + 1] = (uint8_t)((position >> 8) & 0xFF);
                buffer[index + 2] = (uint8_t)((position >> 16) & 0xFF);
                buffer[index + 3] = (uint8_t)((position >> 24) & 0xFF);
                for (int i = 0; i < (index + 4); i++)
                {
                    num8 = (uint8_t)(num8 ^ buffer[i]);
                }
                buffer[index + 4] = num8;
                buffer[index + 5] = 0x83;
                sendCmdToPrinter(buffer, index + 6);
                free(buffer);
                progressFile->SetValue((int)((100 * photonFile->Tell()) / photonFile->Length()));
                getAsyncReply();
            }
        }
        else
        {
            photonFile->Close();
            gcodeCmd.setCommand("M29");
            gcodeCmd.setParameter("");
            sendCmdToPrinter(gcodeCmd.getCommand());
            getAsyncReply();
        }
    }
}
wxString NewFrame::isError(wxString response)
{
    if (response.Trim().substr(0, 6).Contains("Error:"))
    {
        wxStringTokenizer temp(response.Trim(), ":");
        wxString token;
        while (temp.HasMoreTokens())        //get to the last token that is the error message
            token = temp.GetNextToken();
        return token;
    }
    else
        return "";
}
void NewFrame::getVersion()
{
    gcodeCmd.setCommand("M4002");
    //receiver.Send(Encoding.ASCII.GetBytes(gcodeCmd), gcodeCmd.Length);
    sendCmdToPrinter(gcodeCmd.getCommand());
    getAsyncReply();
    //wxString s = getBlockingReply();
    //return s.Trim().substr(3);
}
void NewFrame::updatefileList()
{
    gcodeCmd.setCommand("M20");
    fileCount = 0;
    sendCmdToPrinter(gcodeCmd.getCommand());
    endList = false;
    startList = false;
    getAsyncReply();
}
void NewFrame:: saveSettings()
{
    wxString ini_Directory = wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + wxString("PhotonTool");
    if(!wxDir::Exists(ini_Directory))           //create directory if it doesn't exist
        wxDir::Make(ini_Directory);
    wxString ini_filename = wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + wxString("PhotonTool")+ wxFileName::GetPathSeparator() + wxString(_("Settings.INI"));
    wxFileConfig *config = new wxFileConfig( "", "", ini_filename);
    config->Write( wxT("IP"), addrPeer->IPAddress() );
    config->Flush();
    delete config;
}
void NewFrame:: readSettings()
{
    wxString ini_filename = wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + wxString("PhotonTool")+ wxFileName::GetPathSeparator() + wxString(_("Settings.INI"));
    if(wxFileExists(ini_filename))
    {
        wxFileConfig *config = new wxFileConfig( "", "", ini_filename);
        wxString savedIP=wxEmptyString;
        config->Read(wxT("IP"),&savedIP,"192.168.1.222");
        delete config;
        txtIP->SetValue(savedIP);
    }
    else
        txtIP->SetValue("192.168.1.222");

}
void NewFrame::OnbtnConnectClick(wxCommandEvent& event)
{
    //wxMessageBox(_("Connect to Printer\n"), _("About Connect"), wxOK | wxICON_INFORMATION, this);
    if (!isconnected)            //Connect to the printer if it not connected
    {
        if(connectToPrinter(txtIP->GetValue()))
        {
            getVersion();
            saveSettings();
        }

    }
    else
    {
        disconnectFromPrinter();
    }

    //wxLogMessage("received %s",s);
    //sendCmdToPrinter("M27");
    //getAsyncReply();

}

void NewFrame::OnbtnStartClick(wxCommandEvent& event)
{
    //wxMessageBox(_("Start a print\n"), _("About Start"), wxOK | wxICON_INFORMATION, this);
    long lSelectedItem = ListCtrl1->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    //wxMessageBox(wxString::Format(wxT("Sel = %d"), lSelectedItem));
    if (lSelectedItem != wxNOT_FOUND)
    {
        //wxMessageBox(ListCtrl1->GetItemText(lSelectedItem,1));
        //sprintf(gcodeCmd,"M6030 ':%s'",ListCtrl1->GetItemText(lSelectedItem,1).mb_str().data());
        gcodeCmd.setCommand("M6030");
        gcodeCmd.setParameter(wxString("\':") + ListCtrl1->GetItemText(lSelectedItem, 1).Trim() + wxString("\'"));
        //wxMessageBox(wxString::Format(wxT("%s"), gcodeCmd.getGcodeCmd()));
        sendCmdToPrinter(gcodeCmd.getGcodeCmd());
        getAsyncReply();
    }
    else
    {
        wxMessageBox(_("Please select a file before starting a print."), _("Warning"), wxOK | wxICON_EXCLAMATION | wxCENTER);
    }
}

void NewFrame::OnbtnPauseClick(wxCommandEvent& event)
{
    //wxMessageBox(_("Pause a print\n"), _("About Pause"), wxOK | wxICON_INFORMATION, this);
    PollTimer.Stop();
    if (ispaused)
    {
        gcodeCmd.setCommand("M24");
    }
    else
    {
        gcodeCmd.setCommand("M25");
    }
    sendCmdToPrinter(gcodeCmd.getCommand());
    getAsyncReply();
}

void NewFrame::OnbtnStopClick(wxCommandEvent& event)
{
    //wxMessageBox(_("Stop a print\n"), _("About Stop"), wxOK | wxICON_INFORMATION, this);
    int answer = wxMessageBox(_("Want to stop the current print?"), _("Confirm"), wxYES_NO | wxICON_QUESTION | wxCENTER, this);
    if (answer == wxYES)
    {
        PollTimer.Stop();
        gcodeCmd.setCommand("M33");
        gcodeCmd.setParameter("I5");
        sendCmdToPrinter(gcodeCmd.getGcodeCmd());
        getAsyncReply();
    }
}

void NewFrame::OnbtnDeleteClick(wxCommandEvent& event)
{
    //wxMessageBox(_("Delete a file\n"), _("About Delete"), wxOK | wxICON_INFORMATION, this);
    long lSelectedItem = ListCtrl1->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    //wxMessageBox(wxString::Format(wxT("Sel = %d"), lSelectedItem));
    if (lSelectedItem != wxNOT_FOUND)
    {
        int answer = wxMessageBox(wxString(_("Are you sure you want to permanently delete ")) + ListCtrl1->GetItemText(lSelectedItem, 1).Trim() + wxString(_('?')), _("Confirm"), wxYES_NO | wxICON_QUESTION | wxCENTER, this);
        if (answer == wxYES)
        {
            gcodeCmd.setCommand("M30");
            gcodeCmd.setParameter(ListCtrl1->GetItemText(lSelectedItem, 1).Trim());
            //wxMessageBox(wxString::Format(wxT("%s"), gcodeCmd.getGcodeCmd()));
            sendCmdToPrinter(gcodeCmd.getGcodeCmd());
            getAsyncReply();
        }
    }
    else
    {
        wxMessageBox(_("Please select a file to delete."), _("Warning"), wxOK | wxICON_EXCLAMATION | wxCENTER);
    }
}

void NewFrame::OnbtnRefreshClick(wxCommandEvent& event)
{
    //wxMessageBox(_("Refresh file list\n"), _("About Refresh"), wxOK | wxICON_INFORMATION, this);
	updatefileList();
//	gcodeCmd.setCommand("M27");
//	gcodeCmd.setParameter("");
//	sendCmdToPrinter(gcodeCmd.getCommand());
//	getAsyncReply();
//pingPrinter("192.168.1.223");
//    char buffer[128];
//    //std::string result = "";
//    FILE* pipe = popen("ls", "r");
//    if (!pipe) printf("popen() failed!");
//    else
//    {
//        try {
//            while (fgets(buffer, sizeof buffer, pipe) != NULL)
//            {
//                //result += buffer;
//                printf("%s",buffer);
//            }
//        } catch (...) {
//            pclose(pipe);
//            throw;
//        }
//        pclose(pipe);
//    }
//readSettings();
}

void NewFrame::OnbtnUploadClick(wxCommandEvent& event)
{
    wxFileDialog *OpenDialog = new wxFileDialog(this, _("Choose a file to upload"), wxEmptyString, wxEmptyString, _("All files (*.*)|*.*"), wxFD_OPEN, wxDefaultPosition);//change the filter to .photon files
    if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
    {
        photonFilePath = OpenDialog->GetPath();
        photonFileName = OpenDialog->GetFilename();
        photonFile = new wxFile(photonFilePath, wxFile::OpenMode::read);
        gcodeCmd.setCommand("M28");
        gcodeCmd.setParameter(photonFileName);
        progressFile->SetValue(0);
        sendCmdToPrinter(gcodeCmd.getGcodeCmd());
        getAsyncReply();

        //wxMessageBox(CurrentDocPath);
    }
}

void NewFrame::OnbtnDownloadClick(wxCommandEvent& event)
{
    long lSelectedItem = ListCtrl1->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (lSelectedItem != wxNOT_FOUND)
    {
        wxFileDialog *SaveDialog = new wxFileDialog(this, _("Save downloaded file as . . ."), wxEmptyString, ListCtrl1->GetItemText(lSelectedItem, 1).Trim(), _("All files (*.*)|*.*"), wxFD_SAVE, wxDefaultPosition);//change the filter to .photon files
        if (SaveDialog->ShowModal() == wxID_OK) // if the user click "Save" instead of "cancel"
        {
            downloadStarted = false;
            downloadFileLength = -1;
            downloadFileCurrentLength = -1;
            progressFile->SetValue(0);
            photonFilePath = SaveDialog->GetPath();
            photonFileName = SaveDialog->GetFilename();
            photonFile = new wxFile(photonFilePath, wxFile::OpenMode::write);
            if (photonFile->IsOpened())          //if opening of the file succeded
            {
                gcodeCmd.setCommand("M6032");
                gcodeCmd.setParameter(wxString('\'') + ListCtrl1->GetItemText(lSelectedItem, 1).Trim() + wxString('\''));
                progressFile->SetValue(0);
                sendCmdToPrinter(gcodeCmd.getGcodeCmd());
                getAsyncReply();
            }
            else
            {
                wxMessageBox(_("Failed to write file. Make sure you have write access."), _("Error"), wxOK | wxICON_ERROR | wxCENTER);
            }
        }
    }
    else
    {
        wxMessageBox(_("Please select a file to download."), _("Warning"), wxOK | wxICON_EXCLAMATION | wxCENTER);
    }


}

void NewFrame::OnMyThread(wxCommandEvent& event)
{
    //wxLogMessage("Received %s, %d", receivedBuf, numRead);
    handleResponse();
}

void NewFrame::OnPollTimerTrigger(wxTimerEvent& event)
{
    //wxMessageBox("Triggered");
    if(pingPrinter(addrPeer->IPAddress()))                  //Get status of the printer if it is still connected
    {
        gcodeCmd.setCommand("M27");
        gcodeCmd.setParameter("");
        sendCmdToPrinter(gcodeCmd.getCommand());
        getAsyncReply();
    }
    else
    {
        PollTimer.Stop();
        try
        {
          disconnectFromPrinter();
          wxMessageBox(_("Lost connection to the printer."), _("Error"), wxOK | wxICON_ERROR | wxCENTER);
        }
        catch (int e) {}
    }

}

void NewFrame::OnWatchDogTimerTrigger(wxTimerEvent& event)
{
    try
    {
        if (isRunning)
        {
            sockThread->Kill();
            //free(sockThread);
            sockThread = NULL;
            isRunning = false;
            sock->Close();
            free(addrPeer);
            addrPeer = NULL;
            isconnected = false;
            startList = false;
            endList = false;
            fileCount = 0;
            photonFile = NULL;
            photonFileName = wxEmptyString;
            photonFilePath = wxEmptyString;
            beforeByte = 0;
            frameNum = 0;
            numDownloadRetries = 0;
            downloadStarted = false;
            downloadFileLength = -1;
            downloadFileCurrentLength = -1;
            PollTimer.Stop();
            WatchDogTimer.Stop();
            setStatusMessages(_("Not Connected"), "", "");
            btnConnect->SetLabel(_("Connect"));
            lblStatus->SetLabel(_("Not Connected"));
            clearListControl();
            progressFile->SetValue(0);
            PrintProgress->SetValue(0);
            wxMessageBox(_("Failed to receive any response from the printer in 2 Seconds.\nPlease try re-connecting"), _("Error"), wxOK | wxICON_ERROR | wxCENTER);
        }
    }
    catch (int e) {}
}
