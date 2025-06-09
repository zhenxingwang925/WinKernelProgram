#ifndef DRIVER_OPS_H
#define DRIVER_OPS_H

#include <windows.h>
#include <stdio.h>

int gcInstallService();

int gcStopService();

HANDLE getServerHandle();

int gcStartService();

int gcDeleteService();

#endif