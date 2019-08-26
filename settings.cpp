#include "settings.h"

double pingTimeOut = 100;
double replyTimeout = 2000;
wxString ipAddress = wxString("192.168.1.222");
double port = 3000;
double pollingInterval = 1000;
wxArrayString StaticIPList;
bool enableLogging=true;

void saveLog(LOG_TYPE Type,wxString Message)
{
    if(!enableLogging)
        return;
    wxString ini_Directory = wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + wxString("PhotonTool");
    if(!wxDir::Exists(ini_Directory))           //create directory if it doesn't exist
        wxDir::Make(ini_Directory);
    wxString log_filename = wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + wxString("PhotonTool")+ wxFileName::GetPathSeparator() + wxString(_("run.LOG"));
    wxTextFile logFile(log_filename);
    bool fileOpened=false;
    if(wxFileExists(log_filename))
        fileOpened = logFile.Open();
    else
        fileOpened = logFile.Create();

    if(fileOpened)
    {
        wxString logType=wxString("\t[Information]\t");
        if(Type==LOG_ERROR)
            logType=wxString("\t[Error]\t");
        else if(Type==LOG_WARNING)
            logType=wxString("\t[Warning]\t");
        else if(Type==LOG_EXCEPTION)
            logType=wxString("\t[Exception]\t");
        else if(Type==LOG_INFORMATION)
            logType=wxString("\t[Information]\t");
        wxLongLong  utc_time = wxGetUTCTimeMillis();
        wxDateTime  utc_dt(utc_time);
        unsigned short milisec = utc_dt.GetMillisecond();
        wxString ms = wxString::Format("%u", milisec);      //Convert the milisecond to string
        if(ms.Length()==1)
            ms=wxString("00")+ms;
        else if(ms.Length()==2)
            ms=wxString("0")+ms;
        logFile.AddLine(utc_dt.Format("%a %d:%B:%Y  %I:%M:%S:")+ ms +utc_dt.Format(" %p") + logType + Message);
    }
	logFile.Write();
    logFile.Close();
}
