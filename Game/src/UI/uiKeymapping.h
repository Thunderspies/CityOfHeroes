#ifndef UI_KEYMAPPING_H
#define UI_KEYMAPPING_H



typedef struct Command Command;
typedef struct UIBox UIBox;

extern int g_isBindingKey;

void ParseCommandList();


void keymap_drawReset(float x, float y, float z, float sc);
void keymap_drawCBox(float x, float y, float z, float sc);
float keymap_drawBinds(float x, float y, float z, float sc, float width);
void keymap_getBindValues(void);
void keymap_clearCommands(Command * cmd);
int keybind_CheckBinds( char* command );
void keybind_saveUndo();
void keybind_clearChanges();
void keybind_undoChanges();
void keybind_saveChanges();

void keymap_initProfileChoice(void);
#endif
