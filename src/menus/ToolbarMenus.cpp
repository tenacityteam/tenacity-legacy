

#include "../Menus.h"
#include "../ProjectSettings.h"
#include "../commands/CommandContext.h"
#include "../commands/CommandManager.h"
#include "../toolbars/ToolManager.h"
#include "../CommonCommandFlags.h"

/// Namespace for functions for View Toolbar menu
namespace ToolbarActions {

// exported helper functions
// none

// Menu handler functions

struct Handler : CommandHandlerObject {

void OnResetToolBars(const CommandContext &context)
{
   ToolManager::OnResetToolBars(context);
}

void OnEditMode(const CommandContext &context)
{
   auto &project = context.project;
   auto &commandManager = CommandManager::Get( project );

   bool checked = !gPrefs->Read(wxT("/GUI/Toolbars/EditMode"), true);
   gPrefs->Write(wxT("/GUI/Toolbars/EditMode"), checked);
   gPrefs->Flush();
   commandManager.Check(wxT("EditMode"), checked);

   wxTheApp->AddPendingEvent(wxCommandEvent{ EVT_PREFS_UPDATE });
}

}; // struct Handler


} // namespace

static CommandHandlerObject &findCommandHandler(AudacityProject &) {
   // Handler is not stateful.  Doesn't need a factory registered with
   // AudacityProject.
   static ToolbarActions::Handler instance;
   return instance;
};

// Menu definitions

#define FN(X) (& ToolbarActions::Handler :: X)

namespace{
using namespace MenuTable;

BaseItemSharedPtr ToolbarsMenu()
{
   using Options = CommandManager::Options;

   static BaseItemSharedPtr menu{
   ( FinderScope{ findCommandHandler },
   Section( wxT("Toolbars"),
      Menu( wxT("Toolbars"), XXO("&Toolbars"),
         Section( "Manage",
            /* i18n-hint: (verb)*/
            Command( wxT("EditMode"), XXO("Edit &Mode (on/off)"),
                FN(OnEditMode), AudioIONotBusyFlag(),
                Options{}.CheckTest( wxT("/GUI/Toolbars/EditMode"), true ) ),
            Command( wxT("ResetToolbars"), XXO("Reset Toolb&ars"),
               FN(OnResetToolBars), AlwaysEnabledFlag )
         ),

         Section( "Other" )
      )
   ) ) };
   return menu;
}

AttachedItem sAttachment1{
   Placement{ wxT("View/Other"), { OrderingHint::Begin } },
   Shared( ToolbarsMenu() )
};
}

#undef FN
