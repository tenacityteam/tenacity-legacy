/**********************************************************************

Tenacity

@file TenacityFileConfig.h
@brief Extend FileConfig with application-specific behavior

Paul Licameli split from Prefs.h

**********************************************************************/

#ifndef __TENACITY_FILE_CONFIG__
#define __TENACITY_FILE_CONFIG__

#include "widgets/FileConfig.h" // to inherit
#include <memory>

/// \brief Our own specialisation of FileConfig.
class TENACITY_DLL_API TenacityFileConfig final : public FileConfig {
public:
    //! Require a call to this factory, to guarantee proper two-phase initialization
    static std::unique_ptr<TenacityFileConfig> Create(
        const wxString &appName = {},
        const wxString &vendorName = {},
        const wxString &localFilename = {},
        const wxString &globalFilename = {},
        long style = wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_GLOBAL_FILE,
        const wxMBConv &conv = wxConvAuto());

    ~TenacityFileConfig() override;

protected:
    void Warn() override;

private:
    //! Disallow direct constructor call, because a two-phase initialization is required
    TenacityFileConfig(
        const wxString &appName,
        const wxString &vendorName,
        const wxString &localFilename,
        const wxString &globalFilename,
        long style,
        const wxMBConv &conv);
};
#endif
