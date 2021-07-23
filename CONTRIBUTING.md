# Contributing to Tenacity

Thanks for showing your interest to contribute, your contribution is very
valuable to us as people like **you** help us build Tenacity. Some guidelines
have been put in place in an effort to keep the codebase clean.

## Reporting bugs and feature requests

A great place to ask questions about using Tenacity, report bugs and discuss
feature requests is the [tenacity-discuss mailing list](https://lists.sr.ht/~tenacity/tenacity-discuss).
Please search through the archives, as your question may have been asked before.
We're also available on IRC at #tenacity on Libera.Chat [(web)](https://web.libera.chat/gamja/?channels=tenacity)
if you'd like a more real-time discussion.

The [Issues tab](https://github.com/tenacityteam/tenacity/issues)
on GitHub is restricted to **bugs and feature requests**
only. Please only submit a bug report if you are sure it is valid.

## Contributing

Contributing code to Tenacity is done either via sourcehut or GitHub. Tenacity
requires you to Sign-off your commits, which indicates you agree to the
[Developer Certificate of Origin](#developer-certificate-of-origin). Details below.

_Note: you do not need to open a GitHub issue for every matching contribution,
only for those which need further looking into, and only when asked to._

When contributing, please adhere to the following guidelines:

1. Follow proper code formatting guidelines e.g. If the file uses spaces, do not
   change them to tabs.
2. Do not change any variable names unless necessary.
3. Try to keep changes compatible with upstream (Audacity)
   whenever possible.
4. Make concise and accurate commit messages. A commit message should be
   limited to 50 characters and its description limited to 72 characters
   per line, and the message should be able to complete this sentence:

    > This commit will...

    If you need to add any additional context, do so in the commit description.

### Contributing through sourcehut

SourceHut operates on an email-driven workflow, and uses [`git send-email`](https://git-send-email.io) for patch submission. Please send your
patches to
[`~tenacity/tenacity-dev@lists.sr.ht`](mailto:~tenacity/tenacity-dev@lists.sr.ht).
When revising a patch, please use `git commit --amend` and add the `-v2` (increment every revision)
flag.

### Contributing through GitHub

To contribute code using GitHub, first
[fork this repository](https://github.com/tenacityteam/tenacity)
and make your changes. Please use `git commit --amend` and
`git push -f` for minor changes (only **your** commits).
See [git-rebase.io](https://git-rebase.io) for more details.

## Developer Certificate of Origin

Tenacity is an open source project licensed under the GNU General Public
license, version 2 or later (see [`LICENSE`](LICENSE.txt)).

We respect intellectual property rights, and we'd like to make sure that all
contributions are properly attributed. As such, we use the simple and clear
Developer Certificate of Origin (DCO).

The DCO is a declaration attached to every contribution made by every
contributor. All the developer has to do is include a `Signed-off-by` statement,
thereby agreeing to the DCO, provided below or on
[developercertificate.org](https://developercertificate.org):

```
Developer Certificate of Origin Version 1.1

Copyright (C) 2004, 2006 The Linux Foundation and its contributors.  1
Letterman Drive Suite D4700 San Francisco, CA, 94129

Everyone is permitted to copy and distribute verbatim copies of this license
document, but changing it is not allowed.


Developer's Certificate of Origin 1.1

By making a contribution to this project, I certify that:

(a) The contribution was created in whole or in part by me and I have the right
to submit it under the open source license indicated in the file; or

(b) The contribution is based upon previous work that, to the best of my
knowledge, is covered under an appropriate open source license and I have the
right under that license to submit that work with modifications, whether
created in whole or in part by me, under the same open source license (unless I
am permitted to submit under a different license), as indicated in the file; or

(c) The contribution was provided directly to me by some other person who
certified (a), (b) or (c) and I have not modified it.

(d) I understand and agree that this project and the contribution are public
and that a record of the contribution (including all personal information I
submit with it, including my sign-off) is maintained indefinitely and may be
redistributed consistent with this project or the open source license(s)
involved.
```

Each commit must include a DCO in its description, which looks like this:

```
Signed-off-by: Jane Doe <j.doe@email.email>
```

You may type this line manually or, if using the command line, simply append
`-s` to your commit.

We ask that all contributors use their real email address, which can be replied
to.
