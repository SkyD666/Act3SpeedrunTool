#include "TimeUtil.h"
#include <chrono>

TimeUtil::TimeUtil()
{
}

BOOLEAN TimeUtil::nanosleep(LONGLONG ns)
{
    /* Declarations */
    HANDLE timer; /* Timer handle */
    LARGE_INTEGER li; /* Time defintion */
    /* Create timer */
    if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
        return FALSE;
    /* Set timer properties */
    li.QuadPart = -ns;
    if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
        CloseHandle(timer);
        return FALSE;
    }
    /* Start & wait for timer */
    WaitForSingleObject(timer, INFINITE);
    /* Clean resources */
    CloseHandle(timer);
    /* Slept without problems */
    return TRUE;
}
