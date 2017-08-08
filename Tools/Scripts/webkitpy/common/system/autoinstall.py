# Copyright (c) 2009, Daniel Krech All rights reserved.
# Copyright (C) 2010 Chris Jerdonek (cjerdonek@webkit.org)
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#  * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
#  * Neither the name of the Daniel Krech nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""Support for automatically downloading Python packages from an URL."""


import codecs
import logging
import os
import shutil
import stat
import sys
import tarfile
import tempfile
import urllib2
import urlparse
import zipfile
import re
from distutils import dir_util
from glob import glob
import urlparse

_log = logging.getLogger(__name__)
_MIRROR_REGEXS = re.compile('.*sourceforge.*'), re.compile('.*pypi.*')
_PYPI_ENV_VAR = 'PYPI_MIRRORS'
_SOURCEFORGE_ENV_VAR = 'SOURCEFORGE_MIRRORS'
_CACHE_ENV_VAR = 'LOCAL_AUTOINSTALL_CACHE'


class AutoInstaller(object):

    """Supports automatically installing Python packages from an URL.

    Supports uncompressed files, .tar.gz, .tar.bz2 and .zip formats.

    Basic usage:

    installer = AutoInstaller()

    installer.install(url="http://pypi.python.org/packages/source/p/pep8/pep8-0.5.0.tar.gz#md5=512a818af9979290cd619cce8e9c2e2b",
                      url_subpath="pep8-0.5.0/pep8.py")
    installer.install(url="http://pypi.python.org/packages/source/m/mechanize/mechanize-0.2.4.zip",
                      url_subpath="mechanize")

    """

    def __init__(self, append_to_search_path=False, prepend_to_search_path=False, make_package=True,
                 target_dir=None, temp_dir=None):
        """Create an AutoInstaller instance, and set up the target directory.

        Args:
          append_to_search_path: A boolean value of whether to append the
                                 target directory to the sys.path search path.
          prepend_to_search_path: A boolean value of whether to prepend the
                                 target directory to the sys.path search path.
          make_package: A boolean value of whether to make the target
                        directory a package.  This adds an __init__.py file
                        to the target directory -- allowing packages and
                        modules within the target directory to be imported
                        explicitly using dotted module names.
          target_dir: The directory path to which packages should be installed.
                      Defaults to a subdirectory of the folder containing
                      this module called "autoinstalled".
          temp_dir: The directory path to use for any temporary files
                    generated while downloading, unzipping, and extracting
                    packages to install.  Defaults to a standard temporary
                    location generated by the tempfile module.  This
                    parameter should normally be used only for development
                    testing.

        """
        if target_dir is None:
            this_dir = os.path.dirname(__file__)
            target_dir = os.path.join(this_dir, "autoinstalled")

        # Ensure that the target directory exists.
        self._set_up_target_dir(target_dir, append_to_search_path, prepend_to_search_path, make_package)

        self._target_dir = target_dir
        self._temp_dir = temp_dir

    def _write_file(self, path, text, encoding):
        with codecs.open(path, "w", encoding) as filehandle:
            filehandle.write(text)

    def _set_up_target_dir(self, target_dir, append_to_search_path, prepend_to_search_path, make_package):
        """Set up a target directory.

        Args:
          target_dir: The path to the target directory to set up.
          append_to_search_path: A boolean value of whether to append the
                                 target directory to the sys.path search path.
          prepend_to_search_path: A boolean value of whether to prepend the
                                 target directory to the sys.path search path.
          make_package: A boolean value of whether to make the target
                        directory a package.  This adds an __init__.py file
                        to the target directory -- allowing packages and
                        modules within the target directory to be imported
                        explicitly using dotted module names.

        """
        if not os.path.exists(target_dir):
            os.makedirs(target_dir)

        if append_to_search_path:
            sys.path.append(target_dir)

        if prepend_to_search_path:
            sys.path.insert(0, target_dir)

        if make_package:
            self._make_package(target_dir)

    def _make_package(self, target_dir):
        init_path = os.path.join(target_dir, "__init__.py")
        if not os.path.exists(init_path):
            text = ("# This file is required for Python to search this "
                    "directory for modules.\n")
            self._write_file(init_path, text, "ascii")

    def _create_scratch_directory_inner(self, prefix):
        """Create a scratch directory without exception handling.

        Creates a scratch directory inside the AutoInstaller temp
        directory self._temp_dir, or inside a platform-dependent temp
        directory if self._temp_dir is None.  Returns the path to the
        created scratch directory.

        Raises:
          OSError: [Errno 2] if the containing temp directory self._temp_dir
                             is not None and does not exist.

        """
        # The tempfile.mkdtemp() method function requires that the
        # directory corresponding to the "dir" parameter already exist
        # if it is not None.
        scratch_dir = tempfile.mkdtemp(prefix=prefix.replace('/', '.'), dir=self._temp_dir)
        return scratch_dir

    def _create_scratch_directory(self, target_name):
        """Create a temporary scratch directory, and return its path.

        The scratch directory is generated inside the temp directory
        of this AutoInstaller instance.  This method also creates the
        temp directory if it does not already exist.

        """
        prefix = target_name.replace(os.sep, "_") + "_"
        try:
            scratch_dir = self._create_scratch_directory_inner(prefix)
        except OSError:
            # Handle case of containing temp directory not existing--
            # OSError: [Errno 2] No such file or directory:...
            temp_dir = self._temp_dir
            if temp_dir is None or os.path.exists(temp_dir):
                raise
            # Else try again after creating the temp directory.
            os.makedirs(temp_dir)
            scratch_dir = self._create_scratch_directory_inner(prefix)

        return scratch_dir

    def _url_downloaded_path(self, target_name):
        return os.path.join(self._target_dir, ".%s.url" % target_name.replace('/', '_'))

    def _is_downloaded(self, target_name, url):
        version_path = self._url_downloaded_path(target_name)

        if not os.path.exists(version_path):
            return False

        with codecs.open(version_path, "r", "utf-8") as filehandle:
            return filehandle.read().strip() == url.strip()

    def _record_url_downloaded(self, target_name, url):
        version_path = self._url_downloaded_path(target_name)
        self._write_file(version_path, url, "utf-8")

    def _extract_tar(self, path, scratch_dir):
        # tarfile.extractall() extracts to a path without the trailing ".tar.gz" or ".tar.bz2".

        if path.endswith(".tar.gz"):
            tarFileSuffix = ".tar.gz"
        elif path.endswith(".tar.bz2"):
            tarFileSuffix = ".tar.bz2"
        else:
            raise Exception("...")

        target_basename = os.path.basename(path[:-len(tarFileSuffix)])
        target_path = os.path.join(scratch_dir, target_basename)

        try:
            tar_file = tarfile.open(path)
        except tarfile.ReadError, err:
            # Append existing Error message to new Error.
            message = ("Could not open tar file: %s\n"
                       " The file probably does not have the correct format.\n"
                       " --> Inner message: %s"
                       % (path, err))
            raise Exception(message)

        try:
            tar_file.extractall(target_path)
        finally:
            tar_file.close()

        return target_path

    # This is a replacement for ZipFile.extractall(), which doesn't
    # seem to work with any shipped python 2.7.x versions.
    # See <https://bugs.webkit.org/show_bug.cgi?id=137519> for details.
    def _extract_all(self, zip_file, target_dir):
        for name in zip_file.namelist():
            path = os.path.join(target_dir, name)
            if not os.path.basename(path):
                # Then the path ends in a slash, so it is a directory.
                os.makedirs(path)
                continue

            try:
                # We open this file w/o encoding, as we're reading/writing
                # the raw byte-stream from the zip file.
                outfile = open(path, 'wb')
            except IOError:
                # Not all zip files seem to list the directories explicitly,
                # so try again after creating the containing directory.
                _log.debug("Got IOError: retrying after creating directory...")
                dirname = os.path.dirname(path)
                os.makedirs(dirname)
                outfile = open(path, 'wb')

            try:
                outfile.write(zip_file.read(name))
            finally:
                outfile.close()

    def _unzip(self, path, scratch_dir):
        # zipfile.extractall() extracts to a path without the trailing ".zip".
        target_basename = os.path.basename(path[:-len(".zip")])
        target_path = os.path.join(scratch_dir, target_basename)

        try:
            zip_file = zipfile.ZipFile(path, "r")
        except zipfile.BadZipfile, err:
            message = ("Could not open zip file: %s\n"
                       " --> Inner message: %s"
                       % (path, err))
            raise Exception(message)

        try:
            self._extract_all(zip_file, scratch_dir)
        finally:
            zip_file.close()

        return target_path

    def _copy_unpackaged_files_from_local_cache(self, path, scratch_dir):

        target_basename = os.path.basename(path)
        target_path = os.path.join(scratch_dir, target_basename)

        shutil.copy(path, target_path)
        return target_path

    def _prepare_package(self, path, scratch_dir):
        """Prepare a package for use, if necessary, and return the new path.

        For example, this method unzips zipped files and extracts
        tar files.

        Args:
          path: The path to the downloaded URL contents.
          scratch_dir: The scratch directory.  Note that the scratch
                       directory contains the file designated by the
                       path parameter.

        """
        # FIXME: Add other natural extensions.
        if path.endswith(".zip"):
            new_path = self._unzip(path, scratch_dir)
        elif path.endswith(".tar.gz") or path.endswith(".tar.bz2"):
            new_path = self._extract_tar(path, scratch_dir)
        elif _CACHE_ENV_VAR in os.environ:
            new_path = path
            if os.path.dirname(path) == os.path.normpath(os.environ[_CACHE_ENV_VAR]):
                new_path = self._copy_unpackaged_files_from_local_cache(path, scratch_dir)
        else:
            # No preparation is needed.
            new_path = path

        return new_path

    def _parse_colon_separated_mirrors_from_env(self):
        """
        Pypi mirror examle: PYPI_MIRRORS=pypi.hustunique.com...
        Sourceforge mirror example: SOURCEFORGE_MIRRORS=aarnet.dl.sourceforge.net:citylan.dl.sourceforge.net...
        Mirror sources: http://www.pypi-mirrors.org/, http://sourceforge.net/apps/trac/sourceforge/wiki/Mirrors
        """
        try:
            pypi_mirrors_list = os.environ[_PYPI_ENV_VAR].split(':')
        except(KeyError):
            pypi_mirrors_list = ()

        try:
            sourceforge_mirrors_list = os.environ[_SOURCEFORGE_ENV_VAR].split(':')
        except(KeyError):
            sourceforge_mirrors_list = ()

        mirroriterators = iter(sourceforge_mirrors_list), iter(pypi_mirrors_list)
        return zip(_MIRROR_REGEXS, mirroriterators)

    def _replace_domain_with_next_mirror(self, url, mirrors):
        parsed_url = list(urlparse.urlparse(url))
        new_url = None
        try:
            for regex, addresses in mirrors:
                if regex.match(parsed_url[1]):
                    parsed_url[1] = addresses.next()
                    new_url = urlparse.urlunparse(parsed_url)
        except StopIteration, e:
            _log.info('Ran out of mirrors.')

        return new_url

    def _download_to_stream(self, url, stream):
        mirrors = self._parse_colon_separated_mirrors_from_env()
        failures = 0
        while True:
            try:
                netstream = urllib2.urlopen(url, timeout=30)
                break
            except IOError, err:
                # Try multiple times
                if failures < 2:
                    _log.warning("Failed to download %s, %s retrying" % (
                        url, err))
                    failures += 1
                    continue

                url = self._replace_domain_with_next_mirror(url, mirrors)
                if url:
                    failures = 0
                    continue

                # Append existing Error message to new Error.
                message = ('Could not download Python modules from URL "%s".\n'
                           " Make sure you are connected to the internet.\n"
                           " You must be connected to the internet when "
                           "downloading needed modules for the first time.\n"
                           " --> Inner message: %s"
                           % (url, err))
                raise IOError(message)
        code = 200
        if not url.startswith("file") and hasattr(netstream, "getcode"):
            code = netstream.getcode()
        if not 200 <= code < 300:
            raise ValueError("HTTP Error code %s" % code)

        BUFSIZE = 2 ** 13  # 8KB
        while True:
            data = netstream.read(BUFSIZE)
            if not data:
                break
            stream.write(data)
        netstream.close()

    def _check_package_in_local_autoinstall_cache(self, filename):
        if _CACHE_ENV_VAR not in os.environ:
            return False
        path = glob(os.path.join(os.environ[_CACHE_ENV_VAR], filename) + '*')
        if not path:
            return False

        return path[0]

    def _download(self, url, scratch_dir):
        url_path = urlparse.urlsplit(url)[2]
        url_path = os.path.normpath(url_path)  # Removes trailing slash.
        target_filename = os.path.basename(url_path)

        cache = self._check_package_in_local_autoinstall_cache(target_filename)
        if cache:
            return cache

        target_path = os.path.join(scratch_dir, target_filename)
        with open(target_path, "wb") as stream:
            self._download_to_stream(url, stream)

        if _CACHE_ENV_VAR in os.environ:
            dir_util.copy_tree(scratch_dir, os.environ[_CACHE_ENV_VAR])

        return target_path

    def _install(self, scratch_dir, package_name, target_path, url, url_subpath, files_to_remove):
        """Install a python package from an URL.

        This internal method overwrites the target path if the target
        path already exists.

        """
        path = self._download(url=url, scratch_dir=scratch_dir)
        path = self._prepare_package(path, scratch_dir)

        if url_subpath is None:
            source_path = path
        else:
            source_path = os.path.join(path, url_subpath)

        for filename in files_to_remove:
            path = os.path.join(source_path, filename.replace('/', os.sep))
            if os.path.exists(path):
                # Pre-emptively change the permissions to #0777 to try and work around win32 permissions issues.
                os.chmod(path, stat.S_IRWXU | stat.S_IRWXG | stat.S_IRWXO)
                os.remove(path)

        if os.path.exists(target_path):
            if os.path.isdir(target_path):
                shutil.rmtree(target_path, ignore_errors=True)
            else:
                os.remove(target_path)

        # shutil.move() command creates intermediate directories if they do not exist.
        shutil.move(source_path, target_path)

        # ensure all the new directories are importable.
        intermediate_dirs = os.path.dirname(os.path.relpath(target_path, self._target_dir))
        parent_dirname = self._target_dir
        for dirname in intermediate_dirs.split(os.sep):
            parent_dirname = os.path.join(parent_dirname, dirname)
            self._make_package(parent_dirname)

        self._record_url_downloaded(package_name, url)

    def install(self, url, should_refresh=False, target_name=None,
                url_subpath=None, files_to_remove=None):
        """Install a python package from an URL.

        Args:
          url: The URL from which to download the package.

        Optional Args:
          should_refresh: A boolean value of whether the package should be
                          downloaded and reinstalled again even if the
                          package is already present.
          target_name: The name of the folder or file in the autoinstaller
                       target directory at which the package should be
                       installed.  Defaults to the base name of the
                       URL sub-path.  This parameter must be provided if
                       the URL sub-path is not specified.
          url_subpath: The relative path of the URL directory that should
                       be installed.  Defaults to the full directory, or
                       the entire URL contents.

        """
        if target_name is None:
            if not url_subpath:
                raise ValueError('The "target_name" parameter must be '
                                 'provided if the "url_subpath" parameter '
                                 "is not provided.")
            # Remove any trailing slashes.
            url_subpath = os.path.normpath(url_subpath)
            target_name = os.path.basename(url_subpath)

        target_path = os.path.join(self._target_dir, target_name.replace('/', os.sep))
        if not should_refresh and self._is_downloaded(target_name, url) and os.path.exists(target_path):
            return

        files_to_remove = files_to_remove or []
        package_name = target_name.replace(os.sep, '.')
        _log.info("Auto-installing package: %s" % package_name)

        # The scratch directory is where we will download and prepare
        # files specific to this install until they are ready to move
        # into place.
        scratch_dir = self._create_scratch_directory(target_name)

        try:
            self._install(package_name=package_name,
                          target_path=target_path,
                          scratch_dir=scratch_dir,
                          url=url,
                          url_subpath=url_subpath,
                          files_to_remove=files_to_remove)
        except Exception, err:
            # Append existing Error message to new Error.
            message = ("Error auto-installing the %s package to:\n"
                       ' "%s"\n'
                       " --> Inner message: %s"
                       % (target_name, target_path, err))
            raise Exception(message)
        finally:
            shutil.rmtree(scratch_dir, ignore_errors=True)
        _log.debug('Auto-installed %s to:' % url)
        _log.debug('    "%s"' % target_path)
