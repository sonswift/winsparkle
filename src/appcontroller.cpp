/*
 *  This file is part of WinSparkle (https://winsparkle.org)
 *
 *  Copyright (C) 2013 Vaclav Slavik
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 */

#include "appcontroller.h"
#include "appcast.h"


namespace winsparkle
{

CriticalSection ApplicationController::ms_csVars;

win_sparkle_error_callback_t               ApplicationController::ms_cbError = NULL;
win_sparkle_can_shutdown_callback_t        ApplicationController::ms_cbIsReadyToShutdown = NULL;
win_sparkle_shutdown_request_callback_t    ApplicationController::ms_cbRequestShutdown = NULL;
win_sparkle_did_find_update_callback_t     ApplicationController::ms_cbDidFindUpdate = NULL;
win_sparkle_did_not_find_update_callback_t ApplicationController::ms_cbDidNotFindUpdate = NULL;
win_sparkle_update_cancelled_callback_t    ApplicationController::ms_cbUpdateCancelled = NULL;
win_sparkle_update_skipped_callback_t      ApplicationController::ms_cbUpdateSkipped = NULL;
win_sparkle_update_postponed_callback_t    ApplicationController::ms_cbUpdatePostponed = NULL;
win_sparkle_update_dismissed_callback_t    ApplicationController::ms_cbUpdateDismissed = NULL;
win_sparkle_user_run_installer_callback_t  ApplicationController::ms_cbUserRunInstaller = NULL;
win_sparkle_host_update_available_callback_t ApplicationController::ms_cbHostUpdateAvailable = NULL;
win_sparkle_host_no_update_callback_t        ApplicationController::ms_cbHostNoUpdate = NULL;
win_sparkle_host_download_progress_callback_t ApplicationController::ms_cbHostDownloadProgress = NULL;
win_sparkle_host_update_downloaded_callback_t ApplicationController::ms_cbHostUpdateDownloaded = NULL;
win_sparkle_host_update_error_callback_t      ApplicationController::ms_cbHostUpdateError = NULL;
win_sparkle_host_update_error_details_callback_t ApplicationController::ms_cbHostUpdateErrorDetails = NULL;

bool ApplicationController::IsReadyToShutdown()
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if ( ms_cbIsReadyToShutdown )
            return (*ms_cbIsReadyToShutdown)() == 0 ? false : true;
    }

    // default implementations:

    return true;
}

void ApplicationController::RequestShutdown()
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if ( ms_cbRequestShutdown )
        {
            (*ms_cbRequestShutdown)();
            return;
        }
    }

    // default implementations:

    // nothing yet
}

void ApplicationController::NotifyUpdateError()
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if ( ms_cbError )
        {
            (*ms_cbError)();
            return;
        }
    }
}

void ApplicationController::NotifyUpdateFound()
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if (ms_cbDidFindUpdate)
        {
            (*ms_cbDidFindUpdate)();
            return;
        }
    }
}

void ApplicationController::NotifyUpdateNotFound()
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if ( ms_cbDidNotFindUpdate )
        {
            (*ms_cbDidNotFindUpdate)();
            return;
        }
    }
}

void ApplicationController::NotifyUpdateCancelled()
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if ( ms_cbUpdateCancelled )
        {
            (*ms_cbUpdateCancelled)();
            return;
        }
    }
}

void ApplicationController::NotifyUpdateSkipped()
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if (ms_cbUpdateSkipped)
        {
            (*ms_cbUpdateSkipped)();
            return;
        }
    }
}

void ApplicationController::NotifyUpdatePostponed()
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if (ms_cbUpdatePostponed)
        {
            (*ms_cbUpdatePostponed)();
            return;
        }
    }
}

void ApplicationController::NotifyUpdateDismissed()
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if (ms_cbUpdateDismissed)
        {
            (*ms_cbUpdateDismissed)();
            return;
        }
    }
}


int ApplicationController::UserRunInstallerCallback(const wchar_t* filePath)
{
    if (!ms_cbUserRunInstaller)
        return false;

    return ms_cbUserRunInstaller(filePath);
}

bool ApplicationController::NotifyHostUpdateAvailable(const Appcast& appcast)
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if (!ms_cbHostUpdateAvailable)
            return false;

        win_sparkle_update_info info = {};
        info.struct_size = sizeof(info);
        info.version = appcast.Version.c_str();
        info.short_version = appcast.ShortVersionString.c_str();
        info.title = appcast.Title.c_str();
        info.description = appcast.Description.c_str();
        info.release_notes_url = appcast.ReleaseNotesURL.c_str();
        info.download_url = appcast.enclosure.DownloadURL.c_str();
        info.web_browser_url = appcast.WebBrowserURL.c_str();
        info.installer_arguments = appcast.enclosure.InstallerArguments.c_str();
        info.critical_update = appcast.CriticalUpdate ? 1 : 0;

        (*ms_cbHostUpdateAvailable)(&info);
        return true;
    }
}

bool ApplicationController::NotifyHostNoUpdate()
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if (!ms_cbHostNoUpdate)
            return false;

        (*ms_cbHostNoUpdate)();
        return true;
    }
}

bool ApplicationController::NotifyHostDownloadProgress(unsigned long long downloaded, unsigned long long total)
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if (!ms_cbHostDownloadProgress)
            return false;

        (*ms_cbHostDownloadProgress)(downloaded, total);
        return true;
    }
}

bool ApplicationController::NotifyHostUpdateDownloaded(const wchar_t* updateFile)
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if (!ms_cbHostUpdateDownloaded)
            return false;

        (*ms_cbHostUpdateDownloaded)(updateFile);
        return true;
    }
}

bool ApplicationController::NotifyHostUpdateError(int errorCode,
                                                 const char* stage,
                                                 const char* message,
                                                 const char* url,
                                                 unsigned long win32Error,
                                                 int httpStatus)
{
    {
        CriticalSectionLocker lock(ms_csVars);
        if (ms_cbHostUpdateErrorDetails)
        {
            win_sparkle_update_error_info info = {};
            info.struct_size = sizeof(info);
            info.error_code = errorCode;
            info.stage = stage ? stage : "";
            info.message = message ? message : "";
            info.url = url ? url : "";
            info.win32_error = win32Error;
            info.http_status = httpStatus;

            (*ms_cbHostUpdateErrorDetails)(&info);
            return true;
        }

        if (!ms_cbHostUpdateError)
            return false;

        (*ms_cbHostUpdateError)(errorCode);
        return true;
    }
}

} // namespace winsparkle
