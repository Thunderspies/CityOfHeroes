#ifndef _LAUNCHERCOMM_H
#define _LAUNCHERCOMM_H

int launcherCount(void);
int launcherCountStaticMapLaunchers(void);
void launcherLaunchBeaconizers(void);
int launchersStillConnecting(void);
int launcherCommStartProcess(const char* db_hostname, U32 host_ip, MapCon* map_con, bool preload_transient);
int launcherCommStartServerProcess(const char *command, U32 host_ip, ServerAppCon *server_app, int trackByExe, int monitorOnly);
int launcherCommExec(const char *cmd,U32 host_ip);
int launcherCommRecordCrash(U32 host_ip);
void serverStatusCb(LauncherCon *container,const char *buf);
void launcherCommInit(void);
void resetSlowTickTimer(void);
void delinkDeadLaunchers(void);
F32 dbTickLength(void);
void launcherCommLog(int log);
int launcherFindByIp(U32 ip);
void launcherStatusCb(LauncherCon *container,char *buf);
void launcherReconcile(const char *ipStr);
void launcherCommandSuspend(const char *ipStr);
void launcherCommandResume(const char *ipStr);
void launcherOverloadProtectionTick(void);

#endif
