#ifndef _CUSTOM_DIALOGS_H__
#define _CUSTOM_DIALOGS_H__

typedef void (*dialogHandler1)(void * data);
typedef void (*dialogHandler2)(void * data, char * text);
typedef void (*dialogTimeHandler)(void * data, int minutes);

void freeData(void * data);

void CreateDialogYesNo(HWND hWnd, char * title, char * text, dialogHandler1 onAccept, dialogHandler1 onDecline, void * data);
void CreateDialogPrompt(HWND hWnd, char * title, char * prompt, dialogHandler2 onAccept, dialogHandler1 onDecline, void * data);
void CreateDialogPromptTime(HWND hWnd, char * title, char * prompt, dialogTimeHandler onAccept, dialogHandler1 onDecline, void * data);


#endif  // _CUSTOM_DIALOGS_H__