/**********************************************************************

  Tenacity

  AboutDialog.h

**********************************************************************/

#ifndef TENACITY_ABOUT_DLG
#define TENACITY_ABOUT_DLG

#include <vector>
#include "widgets/wxPanelWrapper.h" // to inherit

class wxStaticBitmap;
class wxTextOutputStream;

class ShuttleGui;

struct AboutDialogCreditItem
{
    AboutDialogCreditItem(TranslatableString str, int r) : description{std::move(str)},
        role{r}{ }
    TranslatableString description;
    int role;
};

using AboutDialogCreditItemsList = std::vector<AboutDialogCreditItem>;

class AUDACITY_DLL_API AboutDialog final : public wxDialogWrapper{
    DECLARE_DYNAMIC_CLASS(AboutDialog)

    public:

    AboutDialog() : AboutDialog(nullptr){};
    AboutDialog(wxWindow * parent);
    virtual ~AboutDialog();
    void OnOK(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()

    private:

    wxStaticBitmap* icon;

    enum Role
    {
        roleTenacityTeamMember,
        rolePreforkTeamMember,
        rolePreforkEmeritusTeam,
        rolePreforkDeceased,
        rolePreforkContributor,
        rolePreforkGraphics,
        roleLibrary,
        rolePreforkThanks
    };

    AboutDialogCreditItemsList creditItems;
    void CreateTenacityTab(ShuttleGui & S);
    void CreateLicenseTab(ShuttleGui & S);
    void CreateInformationTab(ShuttleGui & S);

    static wxImage GenerateTenacityLogoRescaledImage(const float fScale);
    void GenerateTenacityPageDescription(wxTextOutputStream & tos);
    void GenerateTenacityTeamMembersInfo(wxTextOutputStream & tos);
    void GenerateTenacityLibsInfo(wxTextOutputStream & tos);

    void GeneratePreforkTeamMembersInfo(wxTextOutputStream & tos);
    void GeneratePreforkEmeritusInfo(wxTextOutputStream & tos);
    void GeneratePreforkContributorInfo(wxTextOutputStream & tos);
    void GeneratePreforkGraphicsInfo(wxTextOutputStream & tos);
    void GeneratePreforkTranslatorsInfo(wxTextOutputStream & tos);
    void GeneratePreforkSpecialThanksInfo(wxTextOutputStream & tos);
    void GeneratePreforkWebsiteInfo(wxTextOutputStream & tos);

    void PopulateCreditsList();
    void AddCredit(const wxString & name, Role role);
    void AddCredit(const wxString & name, TranslatableString format, Role role);
    wxString GetCreditsByRole(Role role);

    static void AddBuildInfoRow(wxTextOutputStream * str, const wxChar * libname,
                                const TranslatableString & libdesc, const TranslatableString & status);
    static void AddBuildInfoRow(wxTextOutputStream * str,
                                const TranslatableString & description, const wxChar * spec);
};

#endif
