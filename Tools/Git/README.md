# Git

This project contains git from http://git-scm.com with some minor modifications

## Apple Specific Changes

  * Update .gitignore for case insensitive filesystems
  * Report Apple project version in --version output
  * Reword error message when requesting HTML documentation
  * Always list opendiff as an option for mergetool/difftool
  * Use symbolic links instead of hard links
  * Support for Xcode.app co-exestince and relocation
      * Locate perl and python modules using relative-paths
      * Locate git subcommand executables relative to runtime executable
      * Support Xcode.app-bundled gitattributes
      * Support Xcode.app-bundled gitconfig
  * Test suite updates
      * Regression test for <[rdar://problem/10238070](rdar://problem/10238070)>
      * Disable tests that are broken on OS X

## Updating from upstream

Ideally, you have a git remote setup for git://git.kernel.org/pub/scm/git/git.git
If you do not, then do this:

    git remote add upstream git://git.kernel.org/pub/scm/git/git.git
    git remote add upstream-htmldocs git://git.kernel.org/pub/scm/git/git-htmldocs.git
    git remote add upstream-manpages git://git.kernel.org/pub/scm/git/git-manpages.git

Once it is setup, then:

    git merge -X subtree=src/git $RELEASE_TAG

Unfortunately, the generated man pages and html documentation are not tagged.
Check `git log upstream-manpages/master` and `git log upstream-htmldocs/master`
for relevant commit hashes and merge them into the tree:

    git merge -X subtree=src/git-htmldocs <htmldocs hash>
    git merge -X subtree=src/git-manpages <man pages hash>

## Testing

The easiest way to test is to just use prove from the t directory:

    prove -j $(sysctl -n hw.activecpu) --timer --state=failed,save ./t[0-9]*.sh
