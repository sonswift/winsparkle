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

#ifndef _appcontroller_h_
#define _appcontroller_h_

#include "winsparkle.h"
#include "threads.h"


namespace winsparkle
{

struct Appcast;

/**
    Interface to controlling the hosting application.
 */
class ApplicationController
{
public:
    /**
        Control functions.
     */
    //@{

    /// Ask the application if it is ready to terminate itself.
    static bool IsReadyToShutdown();

    /// Tell the host to terminate.
    static void RequestShutdown();

    //@}

    /**
        Notifications.
     */
    //@{

    /// Notify that an error occurred.
    static void NotifyUpdateError();

    /// Notify that an update has been found.
    static void NotifyUpdateFound();

    /// Notify that an update has not been found.
    static void NotifyUpdateNotFound();

    /// Notify that an update was cancelled.
    static void NotifyUpdateCancelled();

    /// Notify that an update was skipped
    static void NotifyUpdateSkipped();

    /// Notify that an update was skipped
    static void NotifyUpdatePostponed();

    /// Notify that update dialog was dismissed (closed)
    static void NotifyUpdateDismissed();

    /// Run the user installer callback if one is set.
    static int UserRunInstallerCallback(const wchar_t*);

    /// Notify the host UI that an update is available.
    static bool NotifyHostUpdateAvailable(const Appcast& appcast);

    /// Notify the host UI that no update was found.
    static bool NotifyHostNoUpdate();

    /// Notify the host UI about download progress.
    static bool NotifyHostDownloadProgress(unsigned long long downloaded, unsigned long long total);

    /// Notify the host UI that an update was downloaded.
    static bool NotifyHostUpdateDownloaded(const wchar_t* updateFile);

    /// Notify the host UI that an update error occurred.
    static bool NotifyHostUpdateError(int errorCode,
                                      const char* stage = NULL,
                                      const char* message = NULL,
                                      const char* url = NULL,
                                      unsigned long win32Error = 0,
                                      int httpStatus = 0);

    //@}

    /**
        Behavior customizations -- callbacks.
     */
    //@{

    /// Set the win_sparkle_error_callback_t function
    static void SetErrorCallback(win_sparkle_error_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbError = callback;
    }

    /// Set the win_sparkle_can_shutdown_callback_t function
    static void SetCanShutdownCallback(win_sparkle_can_shutdown_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbIsReadyToShutdown = callback;
    }

    /// Set the win_sparkle_shutdown_request_callback_t function
    static void SetShutdownRequestCallback(win_sparkle_shutdown_request_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbRequestShutdown = callback;
    }

    /// Set the win_sparkle_did_find_update_callback_t function
    static void SetDidFindUpdateCallback(win_sparkle_did_find_update_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbDidFindUpdate = callback;
    }

    /// Set the win_sparkle_did_not_find_update_callback_t function
    static void SetDidNotFindUpdateCallback(win_sparkle_did_not_find_update_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbDidNotFindUpdate = callback;
    }

    /// Set the win_sparkle_update_cancelled_callback_t function
    static void SetUpdateCancelledCallback(win_sparkle_update_cancelled_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbUpdateCancelled = callback;
    }

    /// Set the win_sparkle_update_skipped_callback_t function
    static void SetUpdateSkippedCallback(win_sparkle_update_skipped_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbUpdateSkipped = callback;
    }

    /// Set the win_sparkle_update_postponed_callback_t function
    static void SetUpdatePostponedCallback(win_sparkle_update_postponed_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbUpdatePostponed = callback;
    }

    /// Set the win_sparkle_update_dismissed_callback_t function
    static void SetUpdateDismissedCallback(win_sparkle_update_dismissed_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbUpdateDismissed = callback;
    }

    static void SetUserRunInstallerCallback(win_sparkle_user_run_installer_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbUserRunInstaller = callback;
    }

    static void SetHostUpdateAvailableCallback(win_sparkle_host_update_available_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbHostUpdateAvailable = callback;
    }

    static void SetHostNoUpdateCallback(win_sparkle_host_no_update_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbHostNoUpdate = callback;
    }

    static void SetHostDownloadProgressCallback(win_sparkle_host_download_progress_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbHostDownloadProgress = callback;
    }

    static void SetHostUpdateDownloadedCallback(win_sparkle_host_update_downloaded_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbHostUpdateDownloaded = callback;
    }

    static void SetHostUpdateErrorCallback(win_sparkle_host_update_error_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbHostUpdateError = callback;
    }

    static void SetHostUpdateErrorDetailsCallback(win_sparkle_host_update_error_details_callback_t callback)
    {
        CriticalSectionLocker lock(ms_csVars);
        ms_cbHostUpdateErrorDetails = callback;
    }

    //@}

private:
    ApplicationController(); // cannot be instantiated

    // guards the variables below:
    static CriticalSection ms_csVars;

    static win_sparkle_error_callback_t               ms_cbError;
    static win_sparkle_can_shutdown_callback_t        ms_cbIsReadyToShutdown;
    static win_sparkle_shutdown_request_callback_t    ms_cbRequestShutdown;
    static win_sparkle_did_find_update_callback_t     ms_cbDidFindUpdate;
    static win_sparkle_did_not_find_update_callback_t ms_cbDidNotFindUpdate;
    static win_sparkle_update_cancelled_callback_t    ms_cbUpdateCancelled;
    static win_sparkle_update_skipped_callback_t      ms_cbUpdateSkipped;
    static win_sparkle_update_postponed_callback_t    ms_cbUpdatePostponed;
    static win_sparkle_update_dismissed_callback_t    ms_cbUpdateDismissed;
    static win_sparkle_user_run_installer_callback_t  ms_cbUserRunInstaller;
    static win_sparkle_host_update_available_callback_t ms_cbHostUpdateAvailable;
    static win_sparkle_host_no_update_callback_t        ms_cbHostNoUpdate;
    static win_sparkle_host_download_progress_callback_t ms_cbHostDownloadProgress;
    static win_sparkle_host_update_downloaded_callback_t ms_cbHostUpdateDownloaded;
    static win_sparkle_host_update_error_callback_t      ms_cbHostUpdateError;
    static win_sparkle_host_update_error_details_callback_t ms_cbHostUpdateErrorDetails;
    
};

} // namespace winsparkle

#endif // _appcontroller_h_
