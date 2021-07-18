/**********************************************************************

  Audacity: A Digital Audio Editor

  FileConfig.h

  Leland Lucius

**********************************************************************/

#ifndef __AUDACITY_WIDGETS_FILECONFIG__
#define __AUDACITY_WIDGETS_FILECONFIG__

#include <memory>

#include <wx/defs.h>
#include <wx/fileconf.h>

#include "Identifier.h"

class FileConfig : public wxConfigBase
{
public:
   FileConfig(const wxString& appName = wxEmptyString,
              const wxString& vendorName = wxEmptyString,
              const wxString& localFilename = wxEmptyString,
              const wxString& globalFilename = wxEmptyString,
              long style = wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_GLOBAL_FILE,
              const wxMBConv& conv = wxConvAuto());
   void Init();
   virtual ~FileConfig();

   virtual void SetPath(const wxString& strPath) override;
   virtual const wxString& GetPath() const override;
   virtual bool GetFirstGroup(wxString& str, long& lIndex) const override;
   virtual bool GetNextGroup(wxString& str, long& lIndex) const override;
   virtual bool GetFirstEntry(wxString& str, long& lIndex) const override;
   virtual bool GetNextEntry(wxString& str, long& lIndex) const override;
   virtual size_t GetNumberOfEntries(bool bRecursive = false) const override;
   virtual size_t GetNumberOfGroups(bool bRecursive = false) const override;
   virtual bool HasGroup(const wxString& strName) const override;
   virtual bool HasEntry(const wxString& strName) const override;
   virtual bool Flush(bool bCurrentOnly = false) override;
   virtual bool RenameEntry(const wxString& oldName, const wxString& newName) override;
   virtual bool RenameGroup(const wxString& oldName, const wxString& newName) override;
   virtual bool DeleteEntry(const wxString& key, bool bDeleteGroupIfEmpty = true) override;
   virtual bool DeleteGroup(const wxString& key) override;
   virtual bool DeleteAll() override;

   // Set and Get values of the version major/minor/micro keys in tenacity.cfg when Tenacity first opens
   void SetVersionKeysInit( int major, int minor, int micro)
   {
      mVersionMajorKeyInit = major;
      mVersionMinorKeyInit = minor;
      mVersionMicroKeyInit = micro;
   }
   void GetVersionKeysInit( int& major, int& minor, int& micro) const
   {
      major = mVersionMajorKeyInit;
      minor = mVersionMinorKeyInit;
      micro = mVersionMicroKeyInit;
   }

protected:
   virtual bool DoReadString(const wxString& key, wxString *pStr) const override;
   virtual bool DoReadLong(const wxString& key, long *pl) const override;
#if wxUSE_BASE64
   virtual bool DoReadBinary(const wxString& key, wxMemoryBuffer* buf) const override;
#endif // wxUSE_BASE64

   virtual bool DoWriteString(const wxString& key, const wxString& szValue) override;
   virtual bool DoWriteLong(const wxString& key, long lValue) override;
#if wxUSE_BASE64
   virtual bool DoWriteBinary(const wxString& key, const wxMemoryBuffer& buf) override;
#endif // wxUSE_BASE64

protected:
   //! Override to notify the user of error conditions involving writability of config files
   virtual void Warn() = 0;

   const FilePath &GetFilePath() const { return mLocalFilename; }

private:
   const wxString mAppName;
   const wxString mVendorName;
   const wxString mLocalFilename;
   const wxString mGlobalFilename;
   const long mStyle;
   const wxMBConv & mConv;

   std::unique_ptr<wxFileConfig> mConfig;

   // values of the version major/minor/micro keys in tenacity.cfg
   // when Tenacity first opens
   int mVersionMajorKeyInit{};
   int mVersionMinorKeyInit{};
   int mVersionMicroKeyInit{};

   bool mDirty;
};

#endif

