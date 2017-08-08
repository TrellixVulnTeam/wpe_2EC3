# Copyright (C) 2010 Chris Jerdonek (cjerdonek@webkit.org)
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# This module is required for Python to treat this directory as a package.

"""Autoinstalls third-party code required by WebKit."""


import codecs
import os
import sys

from webkitpy.common.system.autoinstall import AutoInstaller
from webkitpy.common.system.filesystem import FileSystem

_THIRDPARTY_DIR = os.path.dirname(__file__)
_AUTOINSTALLED_DIR = os.path.join(_THIRDPARTY_DIR, "autoinstalled")

# Putting the autoinstall code into webkitpy/thirdparty/__init__.py
# ensures that no autoinstalling occurs until a caller imports from
# webkitpy.thirdparty.  This is useful if the caller wants to configure
# logging prior to executing autoinstall code.

# FIXME: If any of these servers is offline, webkit-patch breaks (and maybe
# other scripts do, too). See <http://webkit.org/b/42080>.

# We put auto-installed third-party modules in this directory--
#
#     webkitpy/thirdparty/autoinstalled
fs = FileSystem()
fs.maybe_make_directory(_AUTOINSTALLED_DIR)

init_path = fs.join(_AUTOINSTALLED_DIR, "__init__.py")
if not fs.exists(init_path):
    fs.write_text_file(init_path, "")

readme_path = fs.join(_AUTOINSTALLED_DIR, "README")
if not fs.exists(readme_path):
    fs.write_text_file(readme_path,
        "This directory is auto-generated by WebKit and is "
        "safe to delete.\nIt contains needed third-party Python "
        "packages automatically downloaded from the web.")


class AutoinstallImportHook(object):
    def __init__(self, filesystem=None):
        self._fs = filesystem or FileSystem()

    def _ensure_autoinstalled_dir_is_in_sys_path(self):
        # Some packages require that the are being put somewhere under a directory in sys.path.
        if not _AUTOINSTALLED_DIR in sys.path:
            sys.path.insert(0, _AUTOINSTALLED_DIR)

    def find_module(self, fullname, _):
        # This method will run before each import. See http://www.python.org/dev/peps/pep-0302/
        if '.autoinstalled' not in fullname:
            return

        # Note: all of the methods must follow the "_install_XXX" convention in
        # order for autoinstall_everything(), below, to work properly.
        if '.mechanize' in fullname:
            self._install_mechanize()
        elif '.pep8' in fullname:
            self._install_pep8()
        elif '.pylint' in fullname:
            self._install_pylint()
        elif '.coverage' in fullname:
            self._install_coverage()
        elif '.buildbot' in fullname:
            self._install_buildbot()
        elif '.keyring' in fullname:
            self._install_keyring()
        elif '.twisted_15_5_0' in fullname:
            self._install_twisted_15_5_0()

    def _install_mechanize(self):
        self._install("http://pypi.python.org/packages/source/m/mechanize/mechanize-0.2.5.tar.gz",
                             "mechanize-0.2.5/mechanize")

    def _install_keyring(self):
        self._install("https://pypi.python.org/packages/7d/a9/8c6bf60710781ce13a9987c0debda8adab35eb79c6b5525f7fe5240b7a8a/keyring-7.3.1.tar.gz#md5=99dd793e1233964eb87cf56406ee66f6",
                             "keyring-7.3.1/keyring")

    def _install_pep8(self):
        self._install("http://pypi.python.org/packages/source/p/pep8/pep8-0.5.0.tar.gz#md5=512a818af9979290cd619cce8e9c2e2b",
                             "pep8-0.5.0/pep8.py")

    def _install_pylint(self):
        self._ensure_autoinstalled_dir_is_in_sys_path()
        if (not self._fs.exists(self._fs.join(_AUTOINSTALLED_DIR, "pylint")) or
            not self._fs.exists(self._fs.join(_AUTOINSTALLED_DIR, "logilab/astng")) or
            not self._fs.exists(self._fs.join(_AUTOINSTALLED_DIR, "logilab/common"))):
            installer = AutoInstaller(target_dir=_AUTOINSTALLED_DIR)
            files_to_remove = []
            if sys.platform == 'win32':
                files_to_remove = ['test/data/write_protected_file.txt']
            installer.install("http://pypi.python.org/packages/source/l/logilab-common/logilab-common-0.58.1.tar.gz#md5=77298ab2d8bb8b4af9219791e7cee8ce", url_subpath="logilab-common-0.58.1", target_name="logilab/common", files_to_remove=files_to_remove)
            installer.install("http://pypi.python.org/packages/source/l/logilab-astng/logilab-astng-0.24.1.tar.gz#md5=ddaf66e4d85714d9c47a46d4bed406de", url_subpath="logilab-astng-0.24.1", target_name="logilab/astng")
            installer.install('http://pypi.python.org/packages/source/p/pylint/pylint-0.25.1.tar.gz#md5=728bbc2b339bc3749af013709a7f87a5', url_subpath="pylint-0.25.1", target_name="pylint")

    # autoinstalled.buildbot is used by BuildSlaveSupport/build.webkit.org-config/mastercfg_unittest.py
    # and should ideally match the version of BuildBot used at build.webkit.org.
    def _install_buildbot(self):
        # The buildbot package uses jinja2, for example, in buildbot/status/web/base.py.
        # buildbot imports jinja2 directly (as though it were installed on the system),
        # so the search path needs to include jinja2.  We put jinja2 in
        # its own directory so that we can include it in the search path
        # without including other modules as a side effect.
        jinja_dir = self._fs.join(_AUTOINSTALLED_DIR, "jinja2")
        installer = AutoInstaller(append_to_search_path=True, target_dir=jinja_dir)
        installer.install(url="http://pypi.python.org/packages/source/J/Jinja2/Jinja2-2.6.tar.gz#md5=1c49a8825c993bfdcf55bb36897d28a2",
                                                url_subpath="Jinja2-2.6/jinja2")

        SQLAlchemy_dir = self._fs.join(_AUTOINSTALLED_DIR, "sqlalchemy")
        installer = AutoInstaller(append_to_search_path=True, target_dir=SQLAlchemy_dir)
        installer.install(url="http://pypi.python.org/packages/source/S/SQLAlchemy/SQLAlchemy-0.7.7.tar.gz#md5=ddf6df7e014cea318fa981364f3f93b9",
                                                 url_subpath="SQLAlchemy-0.7.7/lib/sqlalchemy")

        twisted_dir = self._fs.join(_AUTOINSTALLED_DIR, "twisted")
        installer = AutoInstaller(prepend_to_search_path=True, target_dir=twisted_dir)
        installer.install(url="https://pypi.python.org/packages/source/T/Twisted/Twisted-12.1.0.tar.bz2#md5=f396f1d6f5321e869c2f89b2196a9eb5", url_subpath="Twisted-12.1.0/twisted")

        self._install("http://pypi.python.org/packages/source/b/buildbot/buildbot-0.8.6p1.tar.gz#md5=b6727d2810c692062c657492bcbeac6a", "buildbot-0.8.6p1/buildbot")

    def _install_coverage(self):
        self._ensure_autoinstalled_dir_is_in_sys_path()
        self._install(url="http://pypi.python.org/packages/source/c/coverage/coverage-3.5.1.tar.gz#md5=410d4c8155a4dab222f2bc51212d4a24", url_subpath="coverage-3.5.1/coverage")

    def _install_twisted_15_5_0(self):
        twisted_dir = self._fs.join(_AUTOINSTALLED_DIR, "twisted_15_5_0")
        installer = AutoInstaller(prepend_to_search_path=True, target_dir=twisted_dir)
        installer.install(url="https://pypi.python.org/packages/source/T/Twisted/Twisted-15.5.0.tar.bz2#md5=0831d7c90d0020062de0f7287530a285", url_subpath="Twisted-15.5.0/twisted")
        installer.install(url="https://pypi.python.org/packages/source/z/zope.interface/zope.interface-4.1.3.tar.gz#md5=9ae3d24c0c7415deb249dd1a132f0f79", url_subpath="zope.interface-4.1.3/src/zope")

    def _install(self, url, url_subpath=None, target_name=None):
        installer = AutoInstaller(target_dir=_AUTOINSTALLED_DIR)
        installer.install(url=url, url_subpath=url_subpath, target_name=target_name)


_hook = AutoinstallImportHook()
sys.meta_path.append(_hook)


def autoinstall_everything():
    install_methods = [method for method in dir(_hook.__class__) if method.startswith('_install_')]
    for method in install_methods:
        getattr(_hook, method)()
