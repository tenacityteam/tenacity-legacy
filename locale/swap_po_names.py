import polib
import os

localedir = './'
count_seen = 0
count_modified = 0

changes = set()
files = set()

for filename in os.listdir(localedir):

    msgids = set()
    if filename.endswith('.po') or filename.endswith('.pot'):
        po = polib.pofile(localedir + filename,
                          check_for_duplicates=True, wrapwidth=0)
# ".*[\n]*Audacity[\n]*.*"
        modified_po_file = False
        for entry in po:
            count_seen += 1
            newmsgid = entry.msgid.replace(
                "Audacity", "Tenacity").replace(
                "audacity", "tenacity").replace(
                "an Tenacity", "a Tenacity").replace(
                "an tenacity", "a tenacity")
            newmsgstr = entry.msgstr.replace(
                "Audacity", "Tenacity").replace(
                "audacity", "tenacity").replace(
                "an Tenacity", "a Tenacity").replace(
                "an tenacity", "a tenacity")

            if (newmsgid != entry.msgid or newmsgstr != entry.msgstr):

                if newmsgid in msgids:
                    print("Skipped \"Duplicate\": ",
                          "[", filename, "]",
                          " line ", entry.linenum,
                          " file ", entry.occurrences,)
                    continue

                if entry.msgid.rfind("audacityteam.org") >= 0 or entry.msgstr.rfind("audacityteam.org") >= 0:
                    print("Skipped \"URL\": ",
                          "[", filename, "]",
                          " line ", entry.linenum,
                          " file ", entry.occurrences,)
                    continue
                if entry.msgid == "The Audacity%s trademark is used within this software for descriptive and informational purposes only.":
                    print("Skipped \"trademark\": ",
                          "[", filename, "]",
                          " line ", entry.linenum,
                          " file ", entry.occurrences,)
                    continue
                if entry.msgid.rfind("LAME for ") >= 0 or entry.msgid.rfind("Lame for ") >= 0 or entry.msgid.rfind("lame for ") >= 0:
                    newmsgid = newmsgid.replace(
                        "LAME for Tenacity", "LAME for Audacity")
                    newmsgstr = entry.msgstr

                if filename.endswith('.pot'):
                    if (not entry.obsolete) and (not entry.msgid in changes):
                        print(
                            "[", filename, "]",
                            " line ", entry.linenum,
                            " file ", entry.occurrences,
                            # " (\"", entry.msgid.replace(
                            #    "\n", "\\n")[0:80], "\")"
                        )
                        changes.add(entry.msgid)
                        for occurence in entry.occurrences:
                            files.add(occurence)

                entry.msgid = newmsgid
                entry.msgstr = newmsgstr
                if filename.endswith('.po') and not entry.fuzzy:
                    entry.flags.append('fuzzy')
                modified_po_file = True
                msgids.add(entry.msgid)
                count_modified += 1

        if modified_po_file:
            po.save(newline='\n')

print("Total Strings: ", count_seen)
print("Modifications: ", count_modified)
print("Files: ", files)
