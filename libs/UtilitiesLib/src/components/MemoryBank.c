//#pragma warning(push)
//#pragma warning(disable:4028) // parameter differs from declaration
//#pragma warning(disable:4024) // related
//#pragma warning(disable:4022) // related

//#pragma warning(disable:4047) // return type differs in lvl of indirection warning
//#pragma warning(disable:4002) // too manay macro parameters warning

#include "utilitieslib/assert/assert.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crtdbg.h>
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/components/MemoryBank.h"
#include "utilitieslib/components/ArrayOld.h"
#include "utilitieslib/utils/timing.h"

typedef struct MemoryLoanImp MemoryLoanImp;
typedef struct MemoryBankElementImp MemoryBankElementImp;
typedef struct MemoryBankImp MemoryBankImp;

struct MemoryLoanImp{
    MemoryBankImp* bank;
    int    elementIndex;
    unsigned int allocTime;
    unsigned int allocAge;
    //char allocInfo[1024];
};

struct MemoryBankElementImp{
    void* memory;
    unsigned int size;

    unsigned char allocated : 1;
};

struct MemoryBankImp{
    Array* activeLoanRecords;
    MemoryPool memoryLoanMemoryPool;
    
    MemoryBankElementImp* storage;
    int maxSize;
    
    unsigned int lastAllocAge;
};


MemoryBank createMemoryBank(){
    return (MemoryBank)calloc(1, sizeof(MemoryBankImp));
}

void initMemoryBank(MemoryBankImp* bank, unsigned int expectedLoans){
    bank->activeLoanRecords = createArray();

    bank->memoryLoanMemoryPool = createMemoryPoolNamed("MemoryLoanImp", __FILE__, __LINE__);
    initMemoryPool(bank->memoryLoanMemoryPool, sizeof(MemoryLoanImp), expectedLoans);
    
    bank->storage = calloc(expectedLoans, sizeof(MemoryBankElementImp));
    bank->maxSize = expectedLoans;
}

void destroyMemoryBank(MemoryBankImp* bank){
    int i;
    MemoryBankElementImp* element;

    if(!bank) 
        return;

    destroyArray(bank->activeLoanRecords);
    destroyMemoryPool(bank->memoryLoanMemoryPool);

    // Free all memory held by the MemoryBank elements.
    for(i = 0; i < bank->maxSize; i++){
        element = &bank->storage[i];
        if(element->memory){
            free(element->memory);
        }
    }

    free(bank->storage);
    free(bank);
}

static INLINEDBG MemoryBankElementImp* mbLoanGetElement(MemoryLoanImp* loan)
{
    return &loan->bank->storage[loan->elementIndex];
}

/* Function mbAlloc()
 *    Try to retrieve the specified amount of memory from the memory bank.
 *
 *
 *
 */
MemoryLoan mbAlloc_dbg(MemoryBankImp* bank, unsigned int allocSize, const char* filename, int lineno){
    int i;
    int foundEmptySlot = 0;
    MemoryBankElementImp* element;
    int bestFitElementIndex = 0;
    int bestFitSizeDelta;
    MemoryLoanImp* loan;
    int count = 0;

    // Are there any more slots in the memory bank?
    if(bank->activeLoanRecords->size == bank->maxSize){
        // If not, create more loans.
        // FIXME!!!
        // JS: Use an Array instead, since this needs to dynamically resize now.
        bank->storage = realloc(bank->storage, bank->maxSize * 2*sizeof(MemoryBankElementImp));
        
        ZeroStructs(bank->storage + bank->maxSize, bank->maxSize);
        bank->maxSize = bank->maxSize * 2;
    }

    // Find the MemoryBank element that is closest in size to the requested allocation size.
    //    Find an empty slot in the bank as the starting point of the search.
    for(i = 0; i < bank->maxSize; i++){
        if (bank->storage[i].allocated) {
            count++;
        }
        if(!foundEmptySlot && !bank->storage[i].allocated){
            foundEmptySlot = 1;
            bestFitElementIndex = i;
            bestFitSizeDelta = bank->storage[i].size - allocSize;
            //break;
        }
    }

    assert(count==bank->activeLoanRecords->size);
    
    if(!foundEmptySlot)
        return 0;

    
    // Prepare a MemoryBank element to be returned.
    //    Mark the chosen element as allocated.
    element = &bank->storage[bestFitElementIndex];
    element->allocated = 1;

    // Prepare a MemoryLoan for return.
    loan = mpAlloc(bank->memoryLoanMemoryPool);
    loan->allocAge = bank->lastAllocAge++;                    // Record loan age.
    //sprintf(loan->allocInfo, "%s: %i", filename, lineno);    // Who took out the loan?
    //dumpStackToBuffer(loan->allocInfo, 1024);
    loan->allocTime = timerCpuTicks();                        // When was the loan taken out?
    loan->bank = bank;                                        // Record which bank the loan is from.
    loan->elementIndex = bestFitElementIndex;                // Record where in the bank the loan came from.

    // Update MemoryBank stats
    arrayPushBack(bank->activeLoanRecords, loan);

    // Make sure there are enough memory in the loan record to satisfy the requested allocation size.
    mbRealloc(loan, allocSize);
    return (MemoryLoan)loan;
}

int mbRealloc(MemoryLoanImp* loan, unsigned int allocSize){
    void* newMemory;

    // Does the loan already satisfy the allocation request?
    if(mbLoanGetElement(loan)->size >= allocSize)
        // If so, nothing needs to be altered, the caller can continue using the same piece of memory.
        return 1;

    // Otherwise, the memory must be reallocated.
    newMemory = realloc(mbLoanGetElement(loan)->memory, allocSize);
    if(newMemory){
        mbLoanGetElement(loan)->memory = newMemory;
        mbLoanGetElement(loan)->size = allocSize;
        return 1;
    }else
        return 0;
}

void mbFree(MemoryLoanImp* loan){
    int loanIndex;

    // Return the memory bank element to the bank
    mbLoanGetElement(loan)->allocated = 0;

    loanIndex = arrayFindElement(loan->bank->activeLoanRecords, loan);
    arrayRemoveAndFill(loan->bank->activeLoanRecords, loanIndex);

    // Return the memory loan to its pool
    mpFree(loan->bank->memoryLoanMemoryPool, loan);
}

void* mbGetMemory(MemoryLoanImp* loan){
    return mbLoanGetElement(loan)->memory;
}


static void mbDumpLoan(MemoryLoanImp* loan){
    printf("Loan record begin\n");
    printf("Loan age is %i\n", loan->allocAge);
    //printf("Loan info is %s\n", loan->allocInfo);
    printf("Loan time was %f seconds ago\n", timerSeconds(timerCpuTicks() - loan->allocTime));
    printf("Loan record end\n");
}

static void mbDumpBank(MemoryBankImp* bank){
    int i;
    for(i = 0; i < bank->activeLoanRecords->size; i++){
        mbDumpLoan(bank->activeLoanRecords->storage[i]);
        printf("\n");
    }

    printf("There are %i active loans\n", bank->activeLoanRecords->size);
    printf("The bank can have up to %i outstanding loans\n", bank->maxSize);
    printf("The last allocation age is %i\n", bank->lastAllocAge);
}

void testMemoryBank(){
    MemoryBank bank;
    MemoryLoan loan;
    MemoryLoan loan2;
    MemoryLoan loan3;
    #define allocSize 1024

    bank = createMemoryBank();
    initMemoryBank(bank, 2);

    // Test memory allocation.
    printf("Taking out loan 1\n");
    loan = mbAlloc(bank, allocSize);

    printf("Taking out loan 2\n");
    loan2 = mbAlloc(bank, allocSize);

    // Test memory allocation limiter.
    //    The memory bank is expecting 2 memory loans only.  Try to take out a 3rd loan now.
    printf("Taking out loan 3\n");
    loan3 = mbAlloc(bank, 1024);


    // Print out the status of all three loans all at once.
    printf("\n");
    if(loan){
        printf("Got loan 1 successfully\n");
    }else
        printf("loan 1 failed\n");

    
    if(loan2){
        printf("Got loan 2 successfully\n");
    }else
        printf("loan 2 failed\n");

    
    if(loan3){
        printf("Got loan 3 successfully\n");
    }else
        printf("loan 3 failed\n");



    // Try to write to the first two (valid) loans.
    printf("\n");
    printf("Filling loan 1 memory\n");
    memset(mbGetMemory(loan), 123, allocSize);
    if(_CrtCheckMemory()){
        printf("Memory write test succeeded\n");
    }else
        printf("Memory write test failed\n");
        

    printf("Filling loan 2 memory\n");
    memset(mbGetMemory(loan), 123, allocSize);
    if(_CrtCheckMemory()){
        printf("Memory write test succeeded\n");
    }else
        printf("Memory write test failed\n");
    


    // Print out all allocation stats
    printf("\n");
    mbDumpBank(bank);

    // Test memory loan deallocation.
    printf("\n");
    printf("Freeing loan 1\n");
    mbFree(loan);
    mbDumpBank(bank);


    printf("\n");
    printf("Freeing loan 2\n");
    mbFree(loan2);
    mbDumpBank(bank);

    // Test memory bank deallocation.
    destroyMemoryBank(bank);
    _CrtDumpMemoryLeaks();    
}

//#pragma warning(pop)
