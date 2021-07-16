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

struct AboutDialogCreditItem {
   AboutDialogCreditItem( TranslatableString str, int r )
      : description{ std::move( str ) }, role{ r }
   {}
   TranslatableString description;
   int role;
};

using AboutDialogCreditItemsList = std::vector<AboutDialogCreditItem>;

class AUDACITY_DLL_API AboutDialog final : public wxDialogWrapper {
   DECLARE_DYNAMIC_CLASS(AboutDialog)

 public:
   AboutDialog(wxWindow * parent);
   virtual ~ AboutDialog();

   static AboutDialog *ActiveIntance();

   void OnOK(wxCommandEvent & event);

   wxStaticBitmap *icon;

   DECLARE_EVENT_TABLE()

 private:
   enum Role {
      roleTeamMember,
      roleEmeritusTeam,
      roleDeceased,
      roleContributor,
      roleGraphics,
      roleLibrary,
      roleThanks
   };

   AboutDialogCreditItemsList creditItems;
   void PopulateTenacityPage( ShuttleGui & S );
   void PopulateLicensePage( ShuttleGui & S );
   void PopulateInformationPage (ShuttleGui & S );

   void CreateCreditsList();
   void AddCredit( const wxString &name, Role role );
   void AddCredit( const wxString &name, TranslatableString format, Role role );
   wxString GetCreditsByRole(Role role);

   static void AddBuildInfoRow( wxTextOutputStream *str, const wxChar * libname,
                                const TranslatableString &libdesc, const TranslatableString &status);
   static void AddBuildInfoRow( wxTextOutputStream *str,
      const TranslatableString &description, const wxChar *spec);
};

#endif
