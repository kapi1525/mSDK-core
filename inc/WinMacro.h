#pragma once

// This header defines some macross that are usefull inside WindowProcs



#ifdef _WIN32
    // WM_COMMAND parameters
    #define	wmCommandID             (int)LOWORD(wParam)
    #define	wmCommandHCtl           (HWND)lParam
    #define	wmCommandNotif          (int)HIWORD(wParam)

    // WM_PARENTNOTIFY parameters
    #define	wmParentNotifyFlags     LOWORD(wParam)
    #define	wmParentNotifyChildID   HIWORD(wParam)
    #define	wmParentNotifyX         LOWORD(lParam)
    #define	wmParentNotifyY         HIWORD(lParam)
    #define	wmParentNotifyHChild    (HWND)lParam

    // WM_HSCROLL & WM_VSCROLL parameters
    #define	wmScrollCode            LOWORD(wParam)
    #define	wmScrollPos             HIWORD(wParam)
    #define	wmScrollHCtl            (HWND)lParam

    // WM_MENUSELECT parameters
    #define	wmMenuSelectIDItem      LOWORD(wParam)
    #define	wmMenuSelectFlags       HIWORD(wParam)
    #define	wmMenuSelectHMenu       (HMENU)lParam

    // WM_MDIACTIVATE parameters
    #define	wmMDIActivateDeact      (HWND)wParam
    #define	wmMDIActivateAct        (HWND)lParam

    // WM_ACTIVATE parameters
    #define	wmActivateFActive       LOWORD(wParam)
    #define	wmActivateFMinimized    HIWORD(wParam)
    #define	wmActivateHwnd          (HWND)lParam
#else
    // WM_COMMAND parameters
    #define	wmCommandID             (int)wParam
    #define	wmCommandHCtl           (HWND)LOWORD(lParam)
    #define	wmCommandNotif          (int)HIWORD(lParam)

    // WM_PARENTNOTIFY parameters
    #define	wmParentNotifyFlags     wParam
    #define	wmParentNotifyChildID   HIWORD(lParam)
    #define	wmParentNotifyX         LOWORD(lParam)
    #define	wmParentNotifyY         HIWORD(lParam)
    #define	wmParentNotifyHChild    (HWND)LOWORD(lParam)

    // WM_HSCROLL & WM_VSCROLL parameters
    #define	wmScrollCode            wParam
    #define	wmScrollPos             LOWORD(lParam)
    #define	wmScrollHCtl            (HWND)HIWORD(lParam)

    // WM_MENUSELECT parameters
    #define	wmMenuSelectIDItem      wParam
    #define	wmMenuSelectFlags       LOWORD(lParam)
    #define	wmMenuSelectHMenu       (HMENU)HIWORD(lParam)

    // WM_MDIACTIVATE parameters
    #define	wmMDIActivateDeact      (HWND)HIWORD(lParam)
    #define	wmMDIActivateAct        (HWND)LOWORD(lParam)

    // WM_ACTIVATE parameters
    #define	wmActivateFActive       wParam
    #define	wmActivateFMinimized    HIWORD(lParam)
    #define	wmActivateHwnd          (HWND)LOWORD(lParam)
#endif
