/**********************************************************************

  Tenacity

  AboutDialog.cpp

********************************************************************//**

\class AboutDialog
\brief The AboutDialog shows the program version and developer credits.

*//*****************************************************************//**

\class AboutDialogCreditItem
\brief AboutDialogCreditItem is a structure used by the AboutDialog to
hold information about a credit item in the Credits list

*//********************************************************************/


#include "AboutDialog.h"

#include <wx/dialog.h>
#include <wx/html/htmlwin.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/intl.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>

#include <BuildInfo.h>
#include "FileNames.h"
#include "HelpText.h"
#include "ShuttleGui.h"
#include "widgets/HelpSystem.h"

#include "AllThemeResources.h"
#include "Theme.h"

// DA: Logo for About box.
#ifdef EXPERIMENTAL_DA
#include "../images/DarkAudacityLogoWithName.xpm"
#else
#include "../images/AudacityLogoWithName.xpm"
#endif



// To substitute into many other translatable strings
static const auto ProgramName = Verbatim("Tenacity");
static const auto PreforkProgramName = Verbatim("Audacity");

// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(AboutDialog, wxDialogWrapper)
EVT_BUTTON(wxID_OK, AboutDialog::OnOK)
END_EVENT_TABLE()

IMPLEMENT_CLASS(AboutDialog, wxDialogWrapper)

AboutDialog::AboutDialog(wxWindow* parent)
/* i18n-hint: information about the program */
    : wxDialogWrapper(parent, -1, XO("About %s").Format(ProgramName),
                      wxDefaultPosition, wxDefaultSize,
                      wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {

    SetName();
    this->SetBackgroundColour(theTheme.Colour(clrAboutBoxBackground));
    ShuttleGui AboutDialogGUI(this, eIsCreating);
    AboutDialogGUI.StartNotebook();
    {
        CreateTenacityTab(AboutDialogGUI);
        CreateInformationTab(AboutDialogGUI);
        CreateLicenseTab(AboutDialogGUI);
    }
    AboutDialogGUI.EndNotebook();

    AboutDialogGUI.Id(wxID_OK).Prop(GROWING_PROPORTION).AddButton(XXO("OK"), wxALIGN_CENTER, true);

    Fit();
    Centre();
}

AboutDialog::~AboutDialog() {
    if(this->IsShown()){
        EndModal(wxID_CLOSE);
    }
    Destroy();
}

void AboutDialog::OnOK(wxCommandEvent& WXUNUSED(event)) {
    EndModal(wxID_OK);
}

void AboutDialog::CreateTenacityTab(ShuttleGui& AboutDialogGUI) {
    PopulateCreditsList();

    wxStringOutputStream tenacityPageGeneratedContent;
    wxTextOutputStream tenacityPageContent(tenacityPageGeneratedContent);   // string to build up list of information in

    tenacityPageContent << wxT("<center>");
        // Tenacity specific credits
        GenerateTenacityPageDescription(tenacityPageContent);
        GenerateTenacityTeamMembersInfo(tenacityPageContent);
        GenerateTenacitySpecialThanksInfo(tenacityPageContent);
        GenerateTenacityLibsInfo(tenacityPageContent);

        // Pre-fork (Audacity) credits
        GeneratePreforkSubheader(tenacityPageContent);
        GeneratePreforkTeamMembersInfo(tenacityPageContent);
        GeneratePreforkEmeritusInfo(tenacityPageContent);
        GeneratePreforkContributorInfo(tenacityPageContent);
        GeneratePreforkTranslatorsInfo(tenacityPageContent);
        GeneratePreforkGraphicsInfo(tenacityPageContent);
        GeneratePreforkSpecialThanksInfo(tenacityPageContent);
        GeneratePreforkWebsiteInfo(tenacityPageContent);
        GeneratePreforkTrademarkDisclaimer(tenacityPageContent);
    tenacityPageContent << wxT("</center>");

    auto pPage = AboutDialogGUI.StartNotebookPage(ProgramName);
    AboutDialogGUI.StartVerticalLay(GROWING_PROPORTION);
    {
        const float fScale = 0.5f;// smaller size.
        const wxImage RescaledImage = GenerateTenacityLogoRescaledImage(fScale);
        const wxColour MainColour(RescaledImage.GetRed(1, 1), RescaledImage.GetGreen(1, 1), RescaledImage.GetBlue(1, 1));
        pPage->SetBackgroundColour(MainColour);

        const wxBitmap RescaledBitmap(RescaledImage);
        const wxSize RescaleBitmapSize((int)(LOGOWITHNAME_WIDTH * fScale), (int)(LOGOWITHNAME_HEIGHT * fScale));
        AboutDialog::icon = safenew wxStaticBitmap(AboutDialogGUI.GetParent(), -1, RescaledBitmap, wxDefaultPosition, RescaleBitmapSize);
        AboutDialogGUI.Prop(MINIMUM_PROPORTION).AddWindow(AboutDialog::icon);
    }

    HtmlWindow* html = safenew LinkingHtmlWindow(AboutDialogGUI.GetParent(), wxID_ANY,
                                                 wxDefaultPosition,
                                                 ABOUT_DIALOG_DEFAULT_SIZE,
                                                 wxHW_SCROLLBAR_AUTO | wxSUNKEN_BORDER);
    html->SetPage(FormatHtmlText(tenacityPageGeneratedContent.GetString()));

    AboutDialogGUI.Prop(GROWING_PROPORTION).Position(wxEXPAND).Focus().AddWindow(html);
    AboutDialogGUI.EndVerticalLay();
    AboutDialogGUI.EndNotebookPage();
}

/** \brief: Fills out the "Information" tab of the preferences dialogue
 *
 * Provides as much information as possible about build-time options and
 * the libraries used, to try and make Linux support easier. Basically anything
 * about the build we might wish to know should be visible here */
void AboutDialog::CreateInformationTab(ShuttleGui& AboutDialogGUI) {
    wxStringOutputStream o;
    wxTextOutputStream informationStr(o);

    AboutDialogGUI.StartNotebookPage(XO("Build Information"));
    AboutDialogGUI.StartVerticalLay(GROWING_PROPORTION);

    HtmlWindow* html = safenew LinkingHtmlWindow(AboutDialogGUI.GetParent(), -1, wxDefaultPosition,
                                                 wxSize(ABOUT_DIALOG_WIDTH, 264),
                                                 wxHW_SCROLLBAR_AUTO | wxSUNKEN_BORDER);
    // create a html pane in it to put the content in.
    auto enabled = XO("Enabled");
    auto disabled = XO("Disabled");
    wxString blank;

    /* this builds up the list of information to go in the window in the string
     * informationStr */
    informationStr
        << wxT("<h2><center>")
        << XO("Build Information")
        << wxT("</center></h2>\n");

    informationStr
        << wxT("<h3>")
        /* i18n-hint: Information about when audacity was compiled follows */
        << XO("The Build")
        << wxT("</h3>\n<table>"); // start build info table

    AddBuildInfoRow(&informationStr, XO("Version:"), BuildInfo::getRevisionIdentifier());
    AddBuildInfoRow(&informationStr, XO("Build type:"), BuildInfo::getBuildType());
    AddBuildInfoRow(&informationStr, XO("Compiler:"), BuildInfo::getCompilerVersionString());
    AddBuildInfoRow(&informationStr, XO("wxWidgets:"), BuildInfo::getWxWidgetsVersion());

    // Install prefix
#ifdef __WXGTK__
    /* i18n-hint: The directory audacity is installed into (on *nix systems) */
    AddBuildInfoRow(&informationStr, XO("Installation Prefix:"), wxT(INSTALL_PREFIX));
#endif

    // Location of settings
    AddBuildInfoRow(&informationStr, XO("Config folder:"), \
        FileNames::ConfigDir());
    // Location of data
    AddBuildInfoRow(&informationStr, XO("Data folder:"), \
        FileNames::DataDir());

    informationStr
        << wxT("<h3>")
        << XO("File Format Support")
        << wxT("</h3>\n<p>");

    informationStr
        << wxT("<table>");   // start table of file formats supported

    AddBuildInfoRow(&informationStr, wxT("PortAudio"), XO("Audio playback and recording"), Verbatim(wxT("v19")));
    AddBuildInfoRow(&informationStr, wxT("libsoxr"), XO("Sample rate conversion"), enabled);

    // This huge set of preprocessor statements can be eliminated if the CMake setup is
    // changed to set OFF options to something falsey instead of nothing
    #ifndef USE_LIBMAD
    #define USE_LIBMAD 0
    #endif
    #ifndef USE_LIBVORBIS
    #define USE_LIBVORBIS 0
    #endif
    #ifndef USE_LIBID3TAG
    #define USE_LIBID3TAG 0
    #endif
    #ifndef USE_LIBFLAC
    #define USE_LIBFLAC 0
    #endif
    #ifndef USE_LIBTWOLAME
    #define USE_LIBTWOLAME 0
    #endif
    #ifndef USE_QUICKTIME
    #define USE_QUICKTIME 0
    #endif
    #ifndef USE_FFMPEG
    #define USE_FFMPEG 0
    #endif
    #ifndef USE_NYQUIST
    #define USE_NYQUIST 0
    #endif
    #ifndef USE_LADSPA
    #define USE_LADPSA 0
    #endif
    #ifndef USE_VAMP
    #define USE_VAMP 0
    #endif
    #ifndef USE_AUDIO_UNITS
    #define USE_AUDIO_UNITS 0
    #endif
    #ifndef USE_VST
    #define USE_VST 0
    #endif
    #ifndef USE_LV2
    #define USE_LV2 0
    #endif
    #ifndef USE_SOUNDTOUCH
    #define USE_SOUNDTOUCH 0
    #endif
    #ifndef USE_SBSMS
    #define USE_SBSMS 0
    #endif

    const auto buildInfo_libmad = XO("MP3 Importing");
    const auto buildInfo_libvorbis = XO("Ogg Vorbis Import and Export");
    const auto buildInfo_libid3tag = XO("ID3 tag support");
    const auto buildInfo_libflac = XO("FLAC import and export");
    const auto buildInfo_libtwolame = XO("MP2 export");
    const auto buildInfo_libquicktime = XO("Import via QuickTime");
    const auto buildInfo_libffmpeg = XO("FFmpeg Import/Export");
    const auto buildInfo_pluginSupport = XO("Plug-in support");
    const auto buildInfo_soundCardMixerSupport = XO("Sound card mixer support");
    const auto buildInfo_pitchTempoSupport = XO("Pitch and Tempo Change support");
    const auto buildInfo_extremePitchTempoSupport = XO("Extreme Pitch and Tempo Change support");

    #ifndef USE_LIBMAD
        AddBuildInfoRow(&informationStr, wxT("libmad"), buildInfo_libmad, USE_LIBMAD ? enabled : disabled);
    #endif
    #ifdef USE_LIBVORBIS
        /* i18n-hint: Ogg is the container format. Vorbis is the compression codec. Both are proper nouns and shouldn't be translated */
        AddBuildInfoRow(&informationStr, wxT("libvorbis"), buildInfo_libvorbis, USE_LIBVORBIS ? enabled : disabled);
    #endif
    #ifdef USE_LIBID3TAG
        AddBuildInfoRow(&informationStr, wxT("libid3tag"), buildInfo_libid3tag, USE_LIBID3TAG ? enabled : disabled);
    #endif
    #ifdef USE_LIBFLAC
        /* i18n-hint: FLAC stands for Free Lossless Audio Codec, but is effectively a proper noun and so shouldn't be translated */
        AddBuildInfoRow(&informationStr, wxT("libflac"), buildInfo_libflac, USE_LIBFLAC ? enabled : disabled);
    #endif
    #ifdef USE_LIBTWOLAME
        AddBuildInfoRow(&informationStr, wxT("libtwolame"), buildInfo_libtwolame, USE_LIBTWOLAME ? enabled : disabled);
    #endif
    #ifdef USE_QUICKTIME
        AddBuildInfoRow(&informationStr, wxT("QuickTime"), buildInfo_libquicktime, USE_QUICKTIME ? enabled : disabled);
    #endif
    #ifdef USE_FFMPEG
        AddBuildInfoRow(&informationStr, wxT("ffmpeg"), buildInfo_libffmpeg, USE_FFMPEG ? enabled : disabled);
    #endif
    informationStr << wxT("</table>\n");  //end table of file formats supported

    informationStr
        << wxT("<h3>")
        << XO("Features")
        << wxT("</h3>\n<table>");  // start table of features

    //AddBuildInfoRow(&informationStr, wxT("Theme"), XO("Dark Theme Extras"),  DA_EXPERIMENTAL ? enabled : disabled);

    #ifdef USE_NYQUIST
        AddBuildInfoRow(&informationStr, wxT("Nyquist"), buildInfo_pluginSupport, USE_NYQUIST ? enabled : disabled);
    #endif
    #ifdef USE_LADSPA
        AddBuildInfoRow(&informationStr, wxT("LADSPA"), buildInfo_pluginSupport, USE_LADSPA ? enabled : disabled);
    #endif
    #ifdef USE_VAMP
        AddBuildInfoRow(&informationStr, wxT("Vamp"), buildInfo_pluginSupport, USE_VAMP ? enabled : disabled);
    #endif
    #ifdef USE_AUDIO_UNITS
        AddBuildInfoRow(&informationStr, wxT("Audio Units"), buildInfo_pluginSupport, USE_AUDIO_UNITS ? enabled : disabled);
    #endif
    #ifdef USE_VST
        AddBuildInfoRow(&informationStr, wxT("VST"), buildInfo_pluginSupport, USE_VST ? enabled : disabled);
    #endif
    #ifdef USE_LV2
        AddBuildInfoRow(&informationStr, wxT("LV2"), buildInfo_pluginSupport, USE_LV2 ? enabled : disabled);
    #endif
    #ifdef USE_SOUNDTOUCH
        AddBuildInfoRow(&informationStr, wxT("SoundTouch"), buildInfo_pitchTempoSupport, USE_SOUNDTOUCH ? enabled : disabled);
    #endif
    #ifdef USE_SBSMS
        AddBuildInfoRow(&informationStr, wxT("SBSMS"), buildInfo_extremePitchTempoSupport, USE_SBSMS ? enabled : disabled);
    #endif
    informationStr << wxT("</table>\n");   // end of table of features

    html->SetPage(FormatHtmlText(o.GetString()));
    AboutDialogGUI.Prop(GROWING_PROPORTION).Position(wxEXPAND).AddWindow(html);
    AboutDialogGUI.EndVerticalLay();
    AboutDialogGUI.EndNotebookPage();
}


void AboutDialog::CreateLicenseTab(ShuttleGui& AboutDialogGUI) {
    AboutDialogGUI.StartNotebookPage(XO("GPL License"));
    AboutDialogGUI.StartVerticalLay(GROWING_PROPORTION);
    HtmlWindow* html = safenew LinkingHtmlWindow(AboutDialogGUI.GetParent(), -1,
                                                 wxDefaultPosition,
                                                 wxSize(ABOUT_DIALOG_WIDTH, 264),
                                                 wxHW_SCROLLBAR_AUTO | wxSUNKEN_BORDER);

    // I tried using <pre> here to get a monospaced font,
    // as is normally used for the GPL.
    // However can't reduce the font size in that case.  It looks
    // better proportionally spaced.
    //
    // The GPL is not to be translated....
    const wxString PageText = FormatHtmlText(
        wxT("<center>GNU GENERAL PUBLIC LICENSE\n</center>")
        wxT("<center>Version 2, June 1991\n</center>")
        wxT("<p><p>")
        wxT(" Copyright (C) 1989, 1991 Free Software Foundation, Inc.\n")
        wxT(" 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA\n")
        wxT(" Everyone is permitted to copy and distribute verbatim copies\n")
        wxT(" of this license document, but changing it is not allowed.\n")
        wxT("\n")
        wxT("<center>Preamble\n</center>")
        wxT("<p><p>\n")
        wxT("  The licenses for most software are designed to take away your\n")
        wxT("freedom to share and change it.  By contrast, the GNU General Public\n")
        wxT("License is intended to guarantee your freedom to share and change free\n")
        wxT("software--to make sure the software is free for all its users.  This\n")
        wxT("General Public License applies to most of the Free Software\n")
        wxT("Foundation's software and to any other program whose authors commit to\n")
        wxT("using it.  (Some other Free Software Foundation software is covered by\n")
        wxT("the GNU Library General Public License instead.)  You can apply it to\n")
        wxT("your programs, too.\n")
        wxT("<p><p>\n")
        wxT("  When we speak of free software, we are referring to freedom, not\n")
        wxT("price.  Our General Public Licenses are designed to make sure that you\n")
        wxT("have the freedom to distribute copies of free software (and charge for\n")
        wxT("this service if you wish), that you receive source code or can get it\n")
        wxT("if you want it, that you can change the software or use pieces of it\n")
        wxT("in new free programs; and that you know you can do these things.\n")
        wxT("<p><p>\n")
        wxT("  To protect your rights, we need to make restrictions that forbid\n")
        wxT("anyone to deny you these rights or to ask you to surrender the rights.\n")
        wxT("These restrictions translate to certain responsibilities for you if you\n")
        wxT("distribute copies of the software, or if you modify it.\n")
        wxT("<p><p>\n")
        wxT("  For example, if you distribute copies of such a program, whether\n")
        wxT("gratis or for a fee, you must give the recipients all the rights that\n")
        wxT("you have.  You must make sure that they, too, receive or can get the\n")
        wxT("source code.  And you must show them these terms so they know their\n")
        wxT("rights.\n")
        wxT("<p><p>\n")
        wxT("  We protect your rights with two steps: (1) copyright the software, and\n")
        wxT("(2) offer you this license which gives you legal permission to copy,\n")
        wxT("distribute and/or modify the software.\n")
        wxT("<p><p>\n")
        wxT("  Also, for each author's protection and ours, we want to make certain\n")
        wxT("that everyone understands that there is no warranty for this free\n")
        wxT("software.  If the software is modified by someone else and passed on, we\n")
        wxT("want its recipients to know that what they have is not the original, so\n")
        wxT("that any problems introduced by others will not reflect on the original\n")
        wxT("authors' reputations.\n")
        wxT("<p><p>\n")
        wxT("  Finally, any free program is threatened constantly by software\n")
        wxT("patents.  We wish to avoid the danger that redistributors of a free\n")
        wxT("program will individually obtain patent licenses, in effect making the\n")
        wxT("program proprietary.  To prevent this, we have made it clear that any\n")
        wxT("patent must be licensed for everyone's free use or not licensed at all.\n")
        wxT("<p><p>\n")
        wxT("  The precise terms and conditions for copying, distribution and\n")
        wxT("modification follow.\n")
        wxT("<p><p>\n")
        wxT("<center>GNU GENERAL PUBLIC LICENSE\n</center>")
        wxT("<center>TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n</center>")
        wxT("<p><p>\n")
        wxT("  0. This License applies to any program or other work which contains\n")
        wxT("a notice placed by the copyright holder saying it may be distributed\n")
        wxT("under the terms of this General Public License.  The \"Program\", below,\n")
        wxT("refers to any such program or work, and a \"work based on the Program\"\n")
        wxT("means either the Program or any derivative work under copyright law:\n")
        wxT("that is to say, a work containing the Program or a portion of it,\n")
        wxT("either verbatim or with modifications and/or translated into another\n")
        wxT("language.  (Hereinafter, translation is included without limitation in\n")
        wxT("the term \"modification\".)  Each licensee is addressed as \"you\".\n")
        wxT("<p><p>\n")
        wxT("Activities other than copying, distribution and modification are not\n")
        wxT("covered by this License; they are outside its scope.  The act of\n")
        wxT("running the Program is not restricted, and the output from the Program\n")
        wxT("is covered only if its contents constitute a work based on the\n")
        wxT("Program (independent of having been made by running the Program).\n")
        wxT("Whether that is true depends on what the Program does.\n")
        wxT("<p><p>\n")
        wxT("  1. You may copy and distribute verbatim copies of the Program's\n")
        wxT("source code as you receive it, in any medium, provided that you\n")
        wxT("conspicuously and appropriately publish on each copy an appropriate\n")
        wxT("copyright notice and disclaimer of warranty; keep intact all the\n")
        wxT("notices that refer to this License and to the absence of any warranty;\n")
        wxT("and give any other recipients of the Program a copy of this License\n")
        wxT("along with the Program.\n")
        wxT("<p><p>\n")
        wxT("You may charge a fee for the physical act of transferring a copy, and\n")
        wxT("you may at your option offer warranty protection in exchange for a fee.\n")
        wxT("<p><p>\n")
        wxT("  2. You may modify your copy or copies of the Program or any portion\n")
        wxT("of it, thus forming a work based on the Program, and copy and\n")
        wxT("distribute such modifications or work under the terms of Section 1\n")
        wxT("above, provided that you also meet all of these conditions:\n")
        wxT("<p><p>\n")
        wxT("<blockquote>")
        wxT("    a) You must cause the modified files to carry prominent notices\n")
        wxT("    stating that you changed the files and the date of any change.\n")
        wxT("<p><p>\n")
        wxT("    b) You must cause any work that you distribute or publish, that in\n")
        wxT("    whole or in part contains or is derived from the Program or any\n")
        wxT("    part thereof, to be licensed as a whole at no charge to all third\n")
        wxT("    parties under the terms of this License.\n")
        wxT("<p><p>\n")
        wxT("    c) If the modified program normally reads commands interactively\n")
        wxT("    when run, you must cause it, when started running for such\n")
        wxT("    interactive use in the most ordinary way, to print or display an\n")
        wxT("    announcement including an appropriate copyright notice and a\n")
        wxT("    notice that there is no warranty (or else, saying that you provide\n")
        wxT("    a warranty) and that users may redistribute the program under\n")
        wxT("    these conditions, and telling the user how to view a copy of this\n")
        wxT("    License.  (Exception: if the Program itself is interactive but\n")
        wxT("    does not normally print such an announcement, your work based on\n")
        wxT("    the Program is not required to print an announcement.)\n")
        wxT("</blockquote>")
        wxT("<p><p>\n")
        wxT("These requirements apply to the modified work as a whole.  If\n")
        wxT("identifiable sections of that work are not derived from the Program,\n")
        wxT("and can be reasonably considered independent and separate works in\n")
        wxT("themselves, then this License, and its terms, do not apply to those\n")
        wxT("sections when you distribute them as separate works.  But when you\n")
        wxT("distribute the same sections as part of a whole which is a work based\n")
        wxT("on the Program, the distribution of the whole must be on the terms of\n")
        wxT("this License, whose permissions for other licensees extend to the\n")
        wxT("entire whole, and thus to each and every part regardless of who wrote it.\n")
        wxT("<p><p>\n")
        wxT("Thus, it is not the intent of this section to claim rights or contest\n")
        wxT("your rights to work written entirely by you; rather, the intent is to\n")
        wxT("exercise the right to control the distribution of derivative or\n")
        wxT("collective works based on the Program.\n")
        wxT("<p><p>\n")
        wxT("In addition, mere aggregation of another work not based on the Program\n")
        wxT("with the Program (or with a work based on the Program) on a volume of\n")
        wxT("a storage or distribution medium does not bring the other work under\n")
        wxT("the scope of this License.\n")
        wxT("<p><p>\n")
        wxT("  3. You may copy and distribute the Program (or a work based on it,\n")
        wxT("under Section 2) in object code or executable form under the terms of\n")
        wxT("Sections 1 and 2 above provided that you also do one of the following:\n")
        wxT("<p><p>\n")
        wxT("<blockquote>")
        wxT("    a) Accompany it with the complete corresponding machine-readable\n")
        wxT("    source code, which must be distributed under the terms of Sections\n")
        wxT("    1 and 2 above on a medium customarily used for software interchange; or,\n")
        wxT("<p><p>\n")
        wxT("    b) Accompany it with a written offer, valid for at least three\n")
        wxT("    years, to give any third party, for a charge no more than your\n")
        wxT("    cost of physically performing source distribution, a complete\n")
        wxT("    machine-readable copy of the corresponding source code, to be\n")
        wxT("    distributed under the terms of Sections 1 and 2 above on a medium\n")
        wxT("    customarily used for software interchange; or,\n")
        wxT("<p><p>\n")
        wxT("    c) Accompany it with the information you received as to the offer\n")
        wxT("    to distribute corresponding source code.  (This alternative is\n")
        wxT("    allowed only for noncommercial distribution and only if you\n")
        wxT("    received the program in object code or executable form with such\n")
        wxT("    an offer, in accord with Subsection b above.)\n")
        wxT("</blockquote>")
        wxT("<p><p>\n")
        wxT("The source code for a work means the preferred form of the work for\n")
        wxT("making modifications to it.  For an executable work, complete source\n")
        wxT("code means all the source code for all modules it contains, plus any\n")
        wxT("associated interface definition files, plus the scripts used to\n")
        wxT("control compilation and installation of the executable.  However, as a\n")
        wxT("special exception, the source code distributed need not include\n")
        wxT("anything that is normally distributed (in either source or binary\n")
        wxT("form) with the major components (compiler, kernel, and so on) of the\n")
        wxT("operating system on which the executable runs, unless that component\n")
        wxT("itself accompanies the executable.\n")
        wxT("<p><p>\n")
        wxT("If distribution of executable or object code is made by offering\n")
        wxT("access to copy from a designated place, then offering equivalent\n")
        wxT("access to copy the source code from the same place counts as\n")
        wxT("distribution of the source code, even though third parties are not\n")
        wxT("compelled to copy the source along with the object code.\n")
        wxT("<p><p>\n")
        wxT("  4. You may not copy, modify, sublicense, or distribute the Program\n")
        wxT("except as expressly provided under this License.  Any attempt\n")
        wxT("otherwise to copy, modify, sublicense or distribute the Program is\n")
        wxT("void, and will automatically terminate your rights under this License.\n")
        wxT("However, parties who have received copies, or rights, from you under\n")
        wxT("this License will not have their licenses terminated so long as such\n")
        wxT("parties remain in full compliance.\n")
        wxT("<p><p>\n")
        wxT("  5. You are not required to accept this License, since you have not\n")
        wxT("signed it.  However, nothing else grants you permission to modify or\n")
        wxT("distribute the Program or its derivative works.  These actions are\n")
        wxT("prohibited by law if you do not accept this License.  Therefore, by\n")
        wxT("modifying or distributing the Program (or any work based on the\n")
        wxT("Program), you indicate your acceptance of this License to do so, and\n")
        wxT("all its terms and conditions for copying, distributing or modifying\n")
        wxT("the Program or works based on it.\n")
        wxT("<p><p>\n")
        wxT("  6. Each time you redistribute the Program (or any work based on the\n")
        wxT("Program), the recipient automatically receives a license from the\n")
        wxT("original licensor to copy, distribute or modify the Program subject to\n")
        wxT("these terms and conditions.  You may not impose any further\n")
        wxT("restrictions on the recipients' exercise of the rights granted herein.\n")
        wxT("You are not responsible for enforcing compliance by third parties to\n")
        wxT("this License.\n")
        wxT("<p><p>\n")
        wxT("  7. If, as a consequence of a court judgment or allegation of patent\n")
        wxT("infringement or for any other reason (not limited to patent issues),\n")
        wxT("conditions are imposed on you (whether by court order, agreement or\n")
        wxT("otherwise) that contradict the conditions of this License, they do not\n")
        wxT("excuse you from the conditions of this License.  If you cannot\n")
        wxT("distribute so as to satisfy simultaneously your obligations under this\n")
        wxT("License and any other pertinent obligations, then as a consequence you\n")
        wxT("may not distribute the Program at all.  For example, if a patent\n")
        wxT("license would not permit royalty-free redistribution of the Program by\n")
        wxT("all those who receive copies directly or indirectly through you, then\n")
        wxT("the only way you could satisfy both it and this License would be to\n")
        wxT("refrain entirely from distribution of the Program.\n")
        wxT("<p><p>\n")
        wxT("If any portion of this section is held invalid or unenforceable under\n")
        wxT("any particular circumstance, the balance of the section is intended to\n")
        wxT("apply and the section as a whole is intended to apply in other\n")
        wxT("circumstances.\n")
        wxT("<p><p>\n")
        wxT("It is not the purpose of this section to induce you to infringe any\n")
        wxT("patents or other property right claims or to contest validity of any\n")
        wxT("such claims; this section has the sole purpose of protecting the\n")
        wxT("integrity of the free software distribution system, which is\n")
        wxT("implemented by public license practices.  Many people have made\n")
        wxT("generous contributions to the wide range of software distributed\n")
        wxT("through that system in reliance on consistent application of that\n")
        wxT("system; it is up to the author/donor to decide if he or she is willing\n")
        wxT("to distribute software through any other system and a licensee cannot\n")
        wxT("impose that choice.\n")
        wxT("<p><p>\n")
        wxT("This section is intended to make thoroughly clear what is believed to\n")
        wxT("be a consequence of the rest of this License.\n")
        wxT("<p><p>\n")
        wxT("  8. If the distribution and/or use of the Program is restricted in\n")
        wxT("certain countries either by patents or by copyrighted interfaces, the\n")
        wxT("original copyright holder who places the Program under this License\n")
        wxT("may add an explicit geographical distribution limitation excluding\n")
        wxT("those countries, so that distribution is permitted only in or among\n")
        wxT("countries not thus excluded.  In such case, this License incorporates\n")
        wxT("the limitation as if written in the body of this License.\n")
        wxT("<p><p>\n")
        wxT("  9. The Free Software Foundation may publish revised and/or new versions\n")
        wxT("of the General Public License from time to time.  Such new versions will\n")
        wxT("be similar in spirit to the present version, but may differ in detail to\n")
        wxT("address new problems or concerns.\n")
        wxT("<p><p>\n")
        wxT("Each version is given a distinguishing version number.  If the Program\n")
        wxT("specifies a version number of this License which applies to it and \"any\n")
        wxT("later version\", you have the option of following the terms and conditions\n")
        wxT("either of that version or of any later version published by the Free\n")
        wxT("Software Foundation.  If the Program does not specify a version number of\n")
        wxT("this License, you may choose any version ever published by the Free Software\n")
        wxT("Foundation.\n")
        wxT("<p><p>\n")
        wxT("  10. If you wish to incorporate parts of the Program into other free\n")
        wxT("programs whose distribution conditions are different, write to the author\n")
        wxT("to ask for permission.  For software which is copyrighted by the Free\n")
        wxT("Software Foundation, write to the Free Software Foundation; we sometimes\n")
        wxT("make exceptions for this.  Our decision will be guided by the two goals\n")
        wxT("of preserving the free status of all derivatives of our free software and\n")
        wxT("of promoting the sharing and reuse of software generally.\n")
        wxT("<p><p>\n")
        wxT("<center>NO WARRANTY\n</center>")
        wxT("<p><p>\n")
        wxT("  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY\n")
        wxT("FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN\n")
        wxT("OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES\n")
        wxT("PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED\n")
        wxT("OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\n")
        wxT("MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS\n")
        wxT("TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE\n")
        wxT("PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,\n")
        wxT("REPAIR OR CORRECTION.\n")
        wxT("<p><p>\n")
        wxT("  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING\n")
        wxT("WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR\n")
        wxT("REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,\n")
        wxT("INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING\n")
        wxT("OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED\n")
        wxT("TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY\n")
        wxT("YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER\n")
        wxT("PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE\n")
        wxT("POSSIBILITY OF SUCH DAMAGES.\n"));

    html->SetPage(PageText);

    AboutDialogGUI.Prop(GROWING_PROPORTION).Position(wxEXPAND).AddWindow(html);
    AboutDialogGUI.EndVerticalLay();
    AboutDialogGUI.EndNotebookPage();
}


void AboutDialog::PopulateCreditsList() {

    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto tenacity_leadDeveloperFormat = XO("%s, lead Tenacity developer");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto tenacity_developerFormat = XO("%s, Tenacity developer");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto tenacity_contributorFormat = XO("%s, Tenacity contributor");

    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_sysAdminFormat = XO("%s, system administration");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_coFounderFormat = XO("%s, co-founder and developer");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_developerFormat = XO("%s, developer");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_developerAndSupportFormat = XO("%s, developer and support");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_documentationAndSupportFormat = XO("%s, documentation and support");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_qaDocumentationAndSupportFormat = XO("%s, QA tester, documentation and support");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_documentationAndSupportFrenchFormat = XO("%s, documentation and support, French");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_qualityAssuranceFormat = XO("%s, quality assurance");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_accessibilityAdvisorFormat = XO("%s, accessibility advisor");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_graphicArtistFormat = XO("%s, graphic artist");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_composerFormat = XO("%s, composer");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_testerFormat = XO("%s, tester");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_NyquistPluginsFormat = XO("%s, Nyquist plug-ins");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_webDeveloperFormat = XO("%s, web developer");
    /* i18n-hint: For "About Tenacity..." credits, substituting a person's proper name */
    const auto prefork_graphicsFormat = XO("%s, graphics");

    // Contributors

    AddCredit(wxT("abb128 ([[https://github.com/abb128|GitHub]])"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("AnotherFoxGuy ([[https://github.com/AnotherFoxGuy|GitHub]])"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("Ajay Ramachandran ([[https://ajay.app|Website]])"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("Be ([[https://github.com/Be-ing/|GitHub]])"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("caughtquick ([[https://caughtquick.tech|Website]]"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("Emily \"emabrey\" Mabrey ([[https://github.com/emabrey|GitHub]])"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("fossdd ([[https://github.com/fossdd|GitHub]])"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("nyanpasu64 ([[https://github.com/nyanpasu64|GitHub]])"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("Panagiotis \"AlwaysLivid\" Vasilopoulos ([[https://alwayslivid.com|Website]])"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("Rikard \"akleja\" Jansson ([[https://github.com/akleja|GitHub]])"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("Semisol ([[https://github.com/Semisol|GitHub]])"), tenacity_contributorFormat, roleTenacityTeamMember);
    AddCredit(wxT("Sol Fisher Romanoff ([[https://solfisher.com|Website]])"), tenacity_contributorFormat, roleTenacityTeamMember);

    // Thanks

    AddCredit(wxT("Drew \"SirCmpwn\" DeVault"), roleTenacityThanks);
    AddCredit(wxT("Filipe \"falkTX\" Coelho"), roleTenacityThanks);

    // Libraries section

    AddCredit(wxT("[[https://libexpat.github.io/|expat]]"), roleLibrary);
    AddCredit(wxT("[[https://xiph.org/flac/|FLAC]]"), roleLibrary);
    AddCredit(wxT("[[http://lame.sourceforge.net/|LAME]]"), roleLibrary);
    AddCredit(wxT("[[https://www.underbit.com/products/mad/|libmad]]"), roleLibrary);
    AddCredit(wxT("[[http://www.mega-nerd.com/libsndfile/|libsndfile]]"), roleLibrary);
    AddCredit(wxT("[[https://sourceforge.net/p/soxr/wiki/Home/|libsoxr]]"), roleLibrary);
    AddCredit(XO("%s (incorporating %s, %s, %s, %s and %s)").Format("[[http://lv2plug.in/|lv2]]", "lilv", "msinttypes", "serd", "sord", "sratom").Translation(), roleLibrary);
    AddCredit(wxT("[[https://www.cs.cmu.edu/~music/nyquist/|Nyquist]]"), roleLibrary);
    AddCredit(wxT("[[https://xiph.org/vorbis/|Ogg Vorbis]]"), roleLibrary);
    AddCredit(wxT("[[http://www.portaudio.com/|PortAudio]]"), roleLibrary);
    AddCredit(wxT("[[http://www.portmedia.sourceforge.net/portmidi/|PortMidi]]"), roleLibrary);
    AddCredit(wxT("[[https://sourceforge.net/p/portmedia/wiki/portsmf/|portsmf]]"), roleLibrary);
    AddCredit(wxT("[[http://sbsms.sourceforge.net/|sbsms]]"), roleLibrary);
    AddCredit(wxT("[[https://www.surina.net/soundtouch/|SoundTouch]]"), roleLibrary);
    AddCredit(wxT("[[http://www.twolame.org/|TwoLAME]]"), roleLibrary);
    AddCredit(wxT("[[http://www.vamp-plugins.org/|Vamp]]"), roleLibrary);
    AddCredit(wxT("[[https://wxwidgets.org/|wxWidgets]]"), roleLibrary);

    // The Prefork Team section
    AddCredit(wxT("James Crook"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Roger Dannenberg"), prefork_coFounderFormat, rolePreforkTeamMember);
    AddCredit(wxT("Steve Daulton"), rolePreforkTeamMember);
    AddCredit(wxT("Anton Gerasimov"), prefork_developerFormat, rolePreforkTeamMember);
    AddCredit(wxT("Greg Kozikowski"), prefork_documentationAndSupportFormat, rolePreforkTeamMember);
    AddCredit(wxT("Paul Licameli"), prefork_developerFormat, rolePreforkTeamMember);
    AddCredit(wxT("Vitaly Sverchinsky"), prefork_developerFormat, rolePreforkTeamMember);
    AddCredit(wxT("Dmitry Vedenko"), prefork_developerFormat, rolePreforkTeamMember);


    // The Prefork Emertitus section
    AddCredit(wxT("Gale Andrews"), prefork_qualityAssuranceFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Richard Ash"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Christian Brochec"), prefork_documentationAndSupportFrenchFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Matt Brubeck"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Arturo \"Buanzo\" Busleiman"), prefork_sysAdminFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Michael Chinen"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Al Dimond"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Benjamin Drung"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Joshua Haberman"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Ruslan Ijbulatov"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Vaughan Johnson"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Leland Lucius"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Dominic Mazzoni"), prefork_coFounderFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Markus Meyer"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Monty Montgomery"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Shane Mueller"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Tony Oetzmann"), prefork_documentationAndSupportFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Alexandre Prokoudine"), prefork_documentationAndSupportFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Peter Sampson"), prefork_qaDocumentationAndSupportFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Martyn Shaw"), prefork_developerFormat, rolePreforkEmeritusTeam);
    AddCredit(wxT("Bill Wharrie"), prefork_documentationAndSupportFormat, rolePreforkEmeritusTeam);

    // Prefork Contributors Section
    AddCredit(wxT("Lynn Allan"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Brian Armstrong"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("David Avery"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("David Bailes"), prefork_accessibilityAdvisorFormat, rolePreforkContributor);
    AddCredit(wxT("William Bland"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Sami Boukortt"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Jeremy R. Brown"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Alex S. Brown"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Chris Cannam"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Cory Cook"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Craig DeForest"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Edgar Franke"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Mitch Golden"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Brian Gunlogson"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Andrew Hallendorff"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Robert H\u00E4nggi"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Daniel Horgan"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("David Hostetler"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Steve Jolly"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Steven Jones"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Henric Jungheim"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Myungchul Keum"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Arun Kishore"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Paul Livesey"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Harvey Lubin"), prefork_graphicArtistFormat, rolePreforkContributor);
    AddCredit(wxT("Max Maisel"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Greg Mekkes"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Abe Milde"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Paul Nasca"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Clayton Otey"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Mark Phillips"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("André Pinto"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Jean Claude Risset"), prefork_composerFormat, rolePreforkContributor);
    AddCredit(wxT("Augustus Saunders"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Benjamin Schwartz"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Cliff Scott"), prefork_testerFormat, rolePreforkContributor);
    AddCredit(wxT("David R. Sky"), prefork_NyquistPluginsFormat, rolePreforkContributor);
    AddCredit(wxT("Rob Sykes"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Mike Underwood"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Philip Van Baren"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Salvo Ventura"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Darrell Walisser"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Jun Wan"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Daniel Winzen"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Tom Woodhams"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Mark Young"), prefork_developerFormat, rolePreforkContributor);
    AddCredit(wxT("Wing Yu"), prefork_developerFormat, rolePreforkContributor);

    // Prefork Website and Graphics
    AddCredit(wxT("Shinta Carolinasari"), prefork_webDeveloperFormat, rolePreforkGraphics);
    AddCredit(wxT("Bayu Rizaldhan Rayes"), prefork_graphicsFormat, rolePreforkGraphics);

    // Prefork Thanks
    AddCredit(wxT("Dave Beydler"), rolePreforkThanks);
    AddCredit(wxT("Brian Cameron"), rolePreforkThanks);
    AddCredit(wxT("Jason Cohen"), rolePreforkThanks);
    AddCredit(wxT("Dave Fancella"), rolePreforkThanks);
    AddCredit(wxT("Steve Harris"), rolePreforkThanks);
    AddCredit(wxT("Daniel James"), rolePreforkThanks);
    AddCredit(wxT("Daniil Kolpakov"), rolePreforkThanks);
    AddCredit(wxT("Robert Leidle"), rolePreforkThanks);
    AddCredit(wxT("Logan Lewis"), rolePreforkThanks);
    AddCredit(wxT("David Luff"), rolePreforkThanks);
    AddCredit(wxT("Jason Pepas"), rolePreforkThanks);
    AddCredit(wxT("Jonathan Ryshpan"), rolePreforkThanks);
    AddCredit(wxT("Michael Schwendt"), rolePreforkThanks);
    AddCredit(wxT("Patrick Shirkey"), rolePreforkThanks);
    AddCredit(wxT("Tuomas Suutari"), rolePreforkThanks);
    AddCredit(wxT("Mark Tomlinson"), rolePreforkThanks);
    AddCredit(wxT("David Topper"), rolePreforkThanks);
    AddCredit(wxT("Rudy Trubitt"), rolePreforkThanks);
    AddCredit(wxT("StreetIQ.com"), rolePreforkThanks);
    AddCredit(wxT("UmixIt Technologies, LLC"), rolePreforkThanks);
    AddCredit(wxT("Verilogix, Inc."), rolePreforkThanks);
}

wxImage AboutDialog::GenerateTenacityLogoRescaledImage(const float fScale) {
    // v For now, change to AudacityLogoWithName via old-fashioned way, not Theme.
    wxBitmap logo(AudacityLogoWithName_xpm); //v

    // JKC: Resize to 50% of size.  Later we may use a smaller xpm as
    // our source, but this allows us to tweak the size - if we want to.
    // It also makes it easier to revert to full size if we decide to.
    wxImage RescaledImage(logo.ConvertToImage());

    // wxIMAGE_QUALITY_HIGH not supported by wxWidgets 2.6.1, or we would use it here.
    RescaledImage.Rescale((int)(LOGOWITHNAME_WIDTH * fScale), (int)(LOGOWITHNAME_HEIGHT * fScale));

    return RescaledImage;
}

void AboutDialog::GenerateTenacityPageDescription(wxTextOutputStream& tos) {
    tos
        // DA: Description and provenance in About box
    #ifdef EXPERIMENTAL_DA
    #undef _
    #define _(s) wxGetTranslation((s))
        << wxT("<h3>DarkTenacity ")
        << wxString(GIT_DESCRIBE)
        << wxT("</h3>")
        << wxT("Customised version of the free, open source, cross-platform audio recorder and editor Tenacity.")

    #else
        << XO("<h3>")
        << ProgramName
        << wxT(" ")
        << wxString(GIT_DESCRIBE)
        << wxT("</h3>")
        /* i18n-hint: The program's name substitutes for %s */
        << XO("Free, open source, cross-platform audio recorder and editor.")
    #endif

        // << wxT("<p><br>")
        // << par1Str
        // << wxT("<p>")
        // << par2Str
        << wxT("<h3>")
        << XO("Credits")
        << wxT("</h3>")
        << wxT("<p>")
        << XO("Please note that names are sorted in alphabetical order, not in order of importance.")
        << wxT("</p>")
        << wxT("<p>");

    // DA: Customisation credit
#ifdef EXPERIMENTAL_DA
    informationStr
        << wxT("<p><b>")
        << XO("DarkTenacity Customisation")
        << wxT("</b><br>")
        << wxT("James Crook, art, coding &amp; design<br>");
#endif

}

void AboutDialog::GenerateTenacityTeamMembersInfo(wxTextOutputStream& tos) {
    tos
        << wxT("<p><b>")
        // To be replaced. Later.
        /* i18n-hint: The program's name substitutes for %s */
        << XO("%s Contributors").Format(ProgramName)
        << wxT("</b><br>")
        << GetCreditsByRole(roleTenacityTeamMember);
}

void AboutDialog::GenerateTenacitySpecialThanksInfo(wxTextOutputStream& tos) {

    tos
        << wxT("<p><b>")
        << XO("Tenacity Special thanks:")
        << wxT("</b><br>")
        << GetCreditsByRole(roleTenacityThanks);
}

void AboutDialog::GenerateTenacityLibsInfo(wxTextOutputStream& tos) {
    tos
        << wxT("<p><b>")
        << XO("Libraries")
        << wxT("</b><br>")

        /* i18n-hint: The program's name substitutes for %s */
        << XO("%s includes code from the following projects:").Format(ProgramName)
        << wxT("<br><br>")
        << GetCreditsByRole(roleLibrary);
}

void AboutDialog::GeneratePreforkSubheader(wxTextOutputStream &tos) {
    tos
        << wxT("<center><h3>")
        << PreforkProgramName
        << wxT("</h3></center>")
        << wxT("<center>");
}

void AboutDialog::GeneratePreforkTeamMembersInfo(wxTextOutputStream &tos) {
    tos
        << wxT("<p><b>")
        /* i18n-hint: The program's name substitutes for %s */
        << XO("%s Team Members").Format(PreforkProgramName)
        << wxT("</b><br>")
        << GetCreditsByRole(rolePreforkTeamMember);
}

void AboutDialog::GeneratePreforkEmeritusInfo(wxTextOutputStream& tos) {
    tos
        << wxT("<p><b> ")
        << XO("Emeritus:")
        << wxT("</b><br>")
        << GetCreditsByRole(rolePreforkEmeritusTeam);
}

void AboutDialog::GeneratePreforkContributorInfo(wxTextOutputStream& tos) {
    tos
        << wxT("<p><b>")
        << XO("Contributors")
        << wxT("</b><br>")
        << GetCreditsByRole(rolePreforkContributor);
}

void AboutDialog::GeneratePreforkGraphicsInfo(wxTextOutputStream& tos) {
    tos
        << wxT("<p><b>")
        << XO("Website and Graphics")
        << wxT("</b><br>")
        << GetCreditsByRole(rolePreforkGraphics);
}

void AboutDialog::GeneratePreforkTranslatorsInfo(wxTextOutputStream& tos) {

    /* i18n-hint: The translation of "translator_credits" will appear
    *  in the credits in the About Audacity window.  Use this to add
    *  your own name(s) to the credits.
    *
    *  For example:  "English translation by Dominic Mazzoni."
    */
    auto translatorCreditsMsgid = XO("translator_credits");
    auto translatorCredits = translatorCreditsMsgid.Translation();
    if (translatorCredits == translatorCreditsMsgid.MSGID().GET())
        // We're in an English locale
        translatorCredits.clear();
    else
        translatorCredits += wxT("<br>");
    if (!translatorCredits.empty()) {
        tos
            << wxT("<p><b>")
            << XO("Translators")
            << wxT("</b><br>")
            << translatorCredits;
    }
}

void AboutDialog::GeneratePreforkSpecialThanksInfo(wxTextOutputStream& tos) {

    tos
        << wxT("<p><b>")
        << XO("Special thanks:")
        << wxT("</b><br>")
        << GetCreditsByRole(rolePreforkThanks);
}

void AboutDialog::GeneratePreforkWebsiteInfo(wxTextOutputStream& tos) {

    tos
        << wxT("<p><br>")
        /* i18n-hint: The program's name substitutes for %s */
        << XO("%s website: ").Format(ProgramName)
        << wxT("[[https://tenacityaudio.org/|https://tenacityaudio.org/]]");

    // DA: Link for DA url too
#ifdef EXPERIMENTAL_DA
    tos << wxT("<br>DarkTenacity website: [[https://tenacityaudio.org/|https://tenacityaudio.org/]]");
#else
    tos << wxT("<p><br>&nbsp; &nbsp; ")
        /* i18n-hint Tenacity's name substitutes for first and fourth %s, and the second and third %s are the copyright symbol and final year of copyright*/
        << XO("%s %s 1999-%s %s contributors")
        .Format(
            Verbatim("<b>%s</b>").Format(ProgramName),
            wxT("&copy;"),
            wxT("2022"),
            ProgramName)
        << wxT("<br>");
#endif
}

void AboutDialog::GeneratePreforkTrademarkDisclaimer(wxTextOutputStream& tos) {

    tos
        /* i18n-hint The registered trademark symbol (r) is substituted at %s*/
        << XO("The Audacity%s trademark is used within this software for descriptive and informational purposes only.").Format("<sup>&reg;</sup>")
        << wxT("\n")
        << XO("Tenacity is not produced or endorsed by MuseCY SM Ltd. or Dominic M Mazzoni.");
}

void AboutDialog::AddCredit(const wxString& name, const Role role) {
    AddCredit(name, {}, role);
}

void AboutDialog::AddCredit(const wxString& name, TranslatableString format, const Role role) {
    auto str = format.empty()
        ? Verbatim(name)
        : TranslatableString{format}.Format(name);

    creditItems.emplace_back(std::move(str), role);
}

wxString AboutDialog::GetCreditsByRole(const Role role) {
    wxString s;

    for (const auto& item : creditItems) {
        if (item.role == role) {
            s += item.description.Translation();
            s += wxT("<br>");
        }
    }

    // Strip last <br>, if any
    if (s.Right(4) == wxT("<br>"))
        s = s.Left(s.length() - 4);

    return s;
}

/** \brief Add a table row saying if a library is used or not
 *
 * Used when creating the build information tab to show if each optional
 * library is enabled or not, and what it does */
void AboutDialog::AddBuildInfoRow(
    wxTextOutputStream* str, const wxChar* libname,
    const TranslatableString& libdesc, const TranslatableString& status) {
    *str
        << wxT("<tr><td>")
        << libname
        << wxT("</td><td>(")
        << libdesc
        << wxT(")</td><td>")
        << status
        << wxT("</td></tr>");
}

/** \brief Add a table row saying if a library is used or not
 *
 * Used when creating the build information tab to show build dates and
 * file paths */
void AboutDialog::AddBuildInfoRow(
    wxTextOutputStream* str,
    const TranslatableString& description, const wxChar* spec) {
    *str
        << wxT("<tr><td>")
        << description
        << wxT("</td><td>")
        << spec
        << wxT("</td></tr>");
}
