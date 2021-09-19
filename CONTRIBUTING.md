# Contributing to Tenacity

Thanks for showing your interest to contribute, your contribution is very
valuable to us as people like **you** help us build Tenacity. Some guidelines
have been put in place in an effort to keep the codebase clean.

## Reporting bugs and feature requests

Our IRC channel, [`#tenacity` on Libera](https://web.libera.chat/#tenacity)
([IRC](ircs://irc.libera.chat:6697/#libera)/
[Matrix](https://matrix.to/#/#tenacity:libera.chat)), is the best place to ask
general questions about Tenacity and talk about feature requests in real time.

You can use [GitHub Issues](https://github.com/tenacityteam/tenacity/issues)
to report bugs and propose feature requests *only*. Support questions will be
ignored.

The
[`tenacity-discuss` mailing list](https://lists.sr.ht/~tenacity/tenacity-discuss)
can also be used for bug reports, feature requests and discussion.
Using a mailing list is as simple as sending an ordinary email to
[`~tenacity/tenacity-discuss@lists.sr.ht`](mailto:~tenacity/tenacity-discuss@lists.sr.ht).
Keep in mind that all emails posted in our mailing list will be archived publicly.
It may be a good idea to look through our archives, because it is possible that
someone else has asked the same question as you have before.

* Please only submit a bug report if you are sure it is valid.
* On GitHub, we use "issue templates" to help you help us. Please use them
  and answer as many questions as you can. That way, we will not have to go
  back and forth to understand what you want to say to us.

## Contributing

Contributing code to Tenacity is done either via sourcehut or GitHub. Tenacity
requires you to Sign-off your commits, which indicates you agree to the
[Developer Certificate of Origin](#developer-certificate-of-origin). Details below.

_Note: you do not need to open a GitHub issue for every matching contribution,
only for those which need further looking into, and only when asked to._

### Submitting code

#### Sending patches through SourceHut

SourceHut operates on an email-driven workflow, and uses
[`git send-email`](https://git-send-email.io) for patch submission. Please
configure the repository like so:

```
git config format.subjectprefix "PATCH tenacity"
git config sendemail.to "~tenacity/tenacity-dev@lists.sr.ht"
```

When revising a patch, please use `git commit --amend` and add the `-v2`
(increment every revision) flag.

#### Making pull requests on GitHub

To contribute code using GitHub, first
[fork this repository](https://github.com/tenacityteam/tenacity)
and make your changes. Please use `git commit --amend` and
`git push -f` for minor changes (only **your** commits).

See [git-rebase.io](https://git-rebase.io) for more details.

### Guidelines for code

Please adhere to the following guidelines when authoring code that you plan to submit to Tenacity:

1. Follow proper code formatting guidelines e.g. If the file uses spaces, do not
   change them to tabs.
2. Do not change any variable names unless necessary.
3. Follow the [commit message guidelines](#commit-messages).

### Guidelines for commits

#### Developer Certificate of Origin

Tenacity is an open source project licensed under the GNU General Public
license, version 2 or later (see [`LICENSE`](LICENSE.txt)).

We respect intellectual property rights, and we would like to make sure that
all contributions are properly attributed. As such, we use the simple and clear
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
Signed-off-by: Con Tributor <con.tributor@example.com>
```

You may type this line manually or, if using the command line, simply append
`-s` to your commit.

We ask that all contributors use their real email address, which can be replied
to.

#### Commit messages

The following rules continue to support hosting our code on multiple platforms,
such as both GitHub and SourceHut, without being unnecessarily locked in to
GitHub. Moreover, they are also necessary for complying to the GPL license,
ensuring our independence and just giving credit where it is due.

Our stance on these rules is not very strict. Worst case scenario, we may
correct the commit messages for you and inform you about what we had to correct
for future reference. However, if you would like to get seriously involved
with our project and take on responsibilities such as as reviewing and merging
patches, your abidance to the following rules will also be one of the factors
that will be considered.

Apart from including a DCO, as mentioned earlier, the following are also very
important:

* Make concise and accurate commit messages. A commit message should be
   limited to 50 characters and its description limited to 72 characters
   per line, and the message should be able to complete this sentence:

    > This commit will...

    If you need to add any additional context, do so in the commit description.

* Avoid using full stops (e.g. `.`) or past tense in your commit messages.

   - Correct: `Add support for the Commodore 64`
   - Incorrect: `Added support for the Commodore 64.`

* The first character of the commit message should be capitalized.

   Example:

   - `GH Actions: Buy celebratory Margaretha Pizza`
   - `Have Tenacity eat more veggies`

* If you are using changes that were made by another person, make sure to
  properly credit them by using the `Co-authored-by: ` tag(s) in the end of
  the commit message before the `Signed-off-by:` tag(s), followed by the name
  or alias that they have used in Git in the past, as well as their e-mail.

   Example: `Co-authored-by: Jane Doe <jane.doe@example.com>`

* If your commit is complicated and involves multiple changes, use asterisks
  and explain of the changes you made in a few words.

   Example:

   ```
   Prepare Teriyaki Sauce

   * Added Soy Sauce
   * Added Cooking Sake and Sugar
   * Added 1 teaspoon of Mirin
   * Added Dashi Stock
   * Mixed ingredients together
   ```

* If you are using changes that were made by another person, the original
  changes by that said person should generally be signed off and available
  publicly in places such as another pull request on GitHub. Exceptions can
  be made, but do ***not*** sign off a commit for another person without
  their explicit permission.

* If your patch resolves an issue that was previously mentioned in the Issues
  tab on GitHub or in our mailing list, please use the `Reference-to:` tag,
  followed by the URL where the issue in question was mentioned.

   Example:

   ```
   Reference-to: https://github.com/tenacityteam/tenacity/issues/2046
   ```

* Leave an empty line between tags such as `Signed-off-by:` and the rest of
  the commit description.

   Example:

   ```
   Remove references to pumpkin pies

   I get that pumpkin pie is tasty, but this does not have anything to do
   with Tenacity whatsoever.

   Signed-off-by: Pumpkin Hater <pumpkin.hater99@example.com>
   ```

##### GitHub

* Avoid using emojis or GitHub-specific references (e.g. `:tada:`) to emojis
  in your commits. They may look just fine on GitHub, but they do not anywhere
  else.

##### Maintainers

* When merging pull requests from GitHub, make sure to remove references to
  issues or pull requests that have a numeric format (e.g. `(#1234)` or
  `Resolves #1234`). Please use the `Reference-to:` tag instead.

  Including a hyperlink to the said issue or pull request is preferred, because
  these links will not break outside of GitHub and will also reduce confusion
  between patches that refer to issues in the Audacity repository and patches
  that are meant to be used in Tenacity. If you use a hyperlink instead of just
  the #nnn format, GitHub will still show the #nnn format on the website, but
  other websites and/or the command line will show the full hyperlink.
  This is good, because it reduces our dependency on GitHub.

###### Merging branches

* If a proposed change is running behind a certain amount of commits that affect
  the same "parts" of the project that the patch also affects, make sure to
  rebase the patch on top of the `master` branch just to be sure that the most
  recent changes do not cause the proposed patch to break.

* In order to accommodate other reviewers that live in different timezones,
  the rule of thumb is to wait for up to a day before merging a change that
  has been approved by a reviewer, or wait around 12 hours before merging a
  change that has been approved by multiple people. If possible, make sure to
  check the change for yourself if possible, especially when a reviewer
  approves a change reluctantly.

* Before merging any change, make sure that all (or, at the very least, *most
  of*) the tests have passed. If a change concerns a particular platform (e.g.
  macOS), then wait for the tests for that said platform to complete.

* If a change affects the user interface or the audio engine, you're generally
  expected to use Tenacity with the included change on your machine and
  evaluate it. Since it's very hard to answer whether a specific change
  affecting the experience of the user is worth including or if the contributor
  should adjust their change, you may want to ask for the help of other
  contributors.

* If there are multiple proposed changes that affect the same parts of the
  project, ***please wait*** for a while after initially merging a single
  proposed change just to be sure that this will not break the build. This does
  not apply to changes that do not affect the functionality of the program
  (such as changes to a Markdown file).

The most basic way of evaluating whether two separate changes affect the same
part of the project is checking whether the changes concern the same source
or header files. For example, if both changes affect `src/Theme.cpp`, then
they affect the same part of the project. However, this sort of evaluation
can get trickier, as in large applications, different source and header files
depend on each other.

Rebasing patches on top of the `master` branch and making sure that they work
as intended is the safest and fastest way to make sure that everything will be
okay.

###### Reverting

Mistakes can happen, and that's okay. After all, we're here to learn and
help others. However, reverting can impose a large amount of work on yourself
or other maintainers later down the line, as well as frustrate
contributors -- particularly those who are contributing for the first-time
or are thinking about contributing to the project.

A change that breaks Tenacity should be reverted under at least some of the
following conditions:

* There is no obvious or fast way (up to a couple of hours) to fix the mistake
  that caused Tenacity to break. **Fixing is better than reverting most of the
  time.**
* There is a high amount of activity on the project and the change that got
  merged is killing off that said activity.
* There are maintainers and contributors that are aware of this and agree that
  reverting is the best cause of action.
* The community appears to heavily disagree with the change.
* Another person, regardless of whether they are a well-established developer
  or a community member, provides an additional perspective that the
  contributors or maintainers were not previously aware of, which calls the
  validity of the change into question.

When reverting a change, you should be *at least* **just as careful** as when
committing a change. Make sure to use your own judgement, communicate
transparently and coordinate with other contributors -- especially the ones
that worked on the change itself.
