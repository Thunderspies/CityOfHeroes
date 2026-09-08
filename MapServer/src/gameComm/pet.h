#ifndef PET_H
#define PET_H

#define MAX_PETNAME_LENGTH 15
#define MAX_NUM_PETNAME 20

typedef struct Entity Entity;
typedef struct Packet Packet;

typedef struct PetName
{
    char            DEPRECATED_pchPowerName[MAX_NAME_LEN];            //        deprecated
    int                petNumber;
    char            petName[MAX_PETNAME_LENGTH+1];
    char            pchEntityDef[MAX_NAME_LEN];
} PetName;

PetName *petname_Create(void);
void PetNameDestroy(void* pnData);
void petReceiveCommand( Entity * e, Packet * pak );
void petReceiveSay( Entity * e, Packet * pak );
void petReceiveRename( Entity * e, Packet * pak );
void petUpdateClientUI(Entity* e, int action, int stance);
void petResetPetNames(Entity* e);
#endif