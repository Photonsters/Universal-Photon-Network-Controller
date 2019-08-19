#include "wx/process.h"
#include <wx/string.h>
#include "ping.h"

bool pingPrinter(wxString ipAddress,int Timeout)
{

#if defined(__WINDOWS__)
		wxString cmd = wxString::Format("ping -n 1 -w %d -l 32 ", Timeout) + ipAddress;
#else
		wxString cmd = wxString::Format("ping -c 1 -W %d -s 32 ", Timeout) + ipAddress;
#endif
    wxArrayString output, errors;
    int code = wxExecute(cmd, output, errors);
    if(code!=0)
        return false;
    for(unsigned int i=0;i<output.GetCount();i++)
    {
        wxString line = output.Item(i);
       // line =line;
		//printf("%s",(char*)( line.mb_str().data()));
		#if defined(__WINDOWS__)
            if(line.Contains(wxString("timed out")) || line.Contains(wxString("host unreachable")))
            {
                //printf("Ping Failed");
                return false;
            }
        #else
            if(line.Find(wxString("packet loss"))!=wxNOT_FOUND)
            {
                wxString loss = line.BeforeLast('%').AfterLast(',');
                double value;
                if(loss.ToDouble(&value))
                {
                    if(value==0)
                        return true;
                    else
                        return false;
                }
            }
        #endif
    }
    return true;
}
