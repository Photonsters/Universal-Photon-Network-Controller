#pragma once
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/dir.h>
#include <wx/utils.h>
#include <wx/datetime.h>
#include <wx/time.h>
extern double pingTimeOut;
extern double replyTimeout;
extern wxString ipAddress;
extern double port;
extern double pollingInterval;
extern wxArrayString StaticIPList;
extern bool enableLogging;

enum LOG_TYPE { LOG_ERROR, LOG_WARNING, LOG_INFORMATION,LOG_EXCEPTION };

void saveLog(LOG_TYPE,  wxString);
