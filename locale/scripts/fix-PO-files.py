import polib
import os

localedir = '../'
count_seen = 0
count_modified = 0

for filename in os.listdir(localedir):
    #if filename.endswith('.po') and not filename.startswith('en'):
    #    print('Starting non-english locale')
    #    po = polib.pofile(localedir + filename)  # specify path to .po file
#
#        for entry in po:
#
    if filename.endswith('.po'):
        po = polib.pofile(localedir + filename, check_for_duplicates=True, wrapwidth=0)  # specify path to en.po file
        for entry in po:
            count_seen += 1

            if count_seen % 500 == 0:
                print(count_seen)

            if entry.msgid.endswith("\n") and not entry.msgstr.endswith("\n") and entry.msgstr != "":
                entry.msgstr = entry.msgstr + '\n'
                count_modified += 1
                print("msgid ends with")

            if entry.msgid.startswith("\n") and not entry.msgstr.startswith("\n") and entry.msgstr != "":
                entry.msgstr = '\n' + entry.msgstr
                count_modified += 1
                print("msgid starts with")

            if entry.msgstr.endswith("\n") and not entry.msgid.endswith("\n"):
                entry.msgstr = entry.msgstr[:2]
                count_modified += 1
                print("msgstr ends with")

            if entry.msgstr.startswith("\n") and not entry.msgid.startswith("\n"):
                entry.msgstr = entry.msgstr[2:]
                count_modified += 1
                print("msgstr starts with")

            if entry.msgstr == "\n":
                entry.msgstr = ''
                count_modified += 1
                print("newline")

            if filename != 'en.po' and entry.msgstr == entry.msgid:
                entry.msgstr = ''
                count_modified += 1
                print("Remove empty")

        #po.merge(localedir + "tenacity.pot")
        po.save(newline='\n')

print("Total Strings: ", count_seen)
print("Modifications: ", count_modified)

