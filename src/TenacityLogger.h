/**********************************************************************

  Tenacity

  TenacityLogger.h

  Dominic Mazzoni

  This is the main source file for Tenacity which handles
  initialization and termination by subclassing wxApp.

**********************************************************************/

#ifndef __TENACITY_LOGGER__
#define __TENACITY_LOGGER__

#include "MemoryX.h"
#include "Prefs.h"
#include <wx/event.h> // to inherit wxEvtHandler
#include <wx/log.h>   // to inherit

class wxFrame;
class wxTextCtrl;

class TENACITY_DLL_API TenacityLogger final : public wxEvtHandler,
                                              public wxLog,
                                              public PrefsListener {
public:
    ~TenacityLogger() override;

    // Get the singleton instance or null
    static TenacityLogger *Get();

    void Show(bool show = true);

    bool SaveLog(const wxString &fileName) const;
    bool ClearLog();

    wxString GetLog(int count = 0);

protected:
    void Flush() override;
    void DoLogText(const wxString &msg) override;

private:
    TenacityLogger();

    void OnCloseWindow(wxCloseEvent &e);
    void OnClose(wxCommandEvent &e);
    void OnClear(wxCommandEvent &e);
    void OnSave(wxCommandEvent &e);

    // PrefsListener implementation
    void UpdatePrefs() override;

    Destroy_ptr<wxFrame> mFrame;
    wxTextCtrl *mText;
    wxString mBuffer;
    bool mUpdated;
};

#endif
