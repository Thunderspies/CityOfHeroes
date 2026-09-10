// memtrack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <map>
#include <fstream>
#include <algorithm>
#include "SymbolHelper.h"
#include <sqlite3.h>

using namespace std;

typedef map<unsigned, AddressSymbol> AddressMap;

// Read the callstack data file and generate a map with each address
// that appears in any callstack and its symbol resolution. We can use
// this map when we want to generate a human readable symbol of an address
// within a callstack.
// note: If the address data is of manageable size then it may be quicker and
// easier to make/use a simple flat file for the resolved symbol information.
bool build_address_map(AddressMap& a_map, const char* srcName, SymbolHelper& sh)
{
    ifstream fin(srcName, ios::binary);

    if (!fin.is_open())
    {
        fprintf(stderr, "ERROR: could not open dictionary log: %s\n", srcName);
        return false;
    }

    printf("building address map...");

    // read file header which is currently a single version string
    if (!fin.eof())
    {
        unsigned int version;
        fin.read((char*)(&version), sizeof(version)); // number of entries in this callstack
        printf(" dictionary version: %d\n", version);
    }

    unsigned int longest_callstack = 0;

    while (!fin.eof())
    {
        // process the addresses in each call stack
        unsigned int hash, count, address;
        fin.read((char*)(&hash), sizeof(hash));   // hash code for call stack
        fin.read((char*)(&count), sizeof(count)); // number of entries in this callstack
        if (count > 1000)
        {
            // @todo
            // This is probably a corrupt entry
            fprintf(stderr, "\nERROR: Corrupt callstack dictionary, %d entries in a callstack\n", count);
            return false;
        }
        longest_callstack = max(count, longest_callstack);
        for (unsigned int i = 0; i < count; ++i)
        {
            fin.read((char*)(&address), sizeof(address));
            // get symbol information for this address an add it to the map
            // if we have not already done so
            if (a_map.count(address) == 0)
            {
                AddressSymbol as(address);
                sh.ResolveAddress(as);
                a_map[address] = as;
                //                printf( as.str().c_str() );
            }
        }
    }
    printf(" longest callstack: %d\n", longest_callstack);
    printf("done.\n");
    return true;
}

// open the named sql db and create/replace a table with every address
// and its symbol resolution. We can use that db for generating reports
// using python, etc
void build_address_db(const char* db_name, AddressMap& a_map)
{
    sqlite3* db;
    int rc;

    rc = sqlite3_open(db_name, &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // delete existing table, if any
    char cmd_drop[] = "DROP TABLE IF EXISTS symbols";
    rc = sqlite3_exec(db, cmd_drop, NULL, NULL, NULL);
    if (rc)
    {
        fprintf(stderr, "Can't drop table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // create table
    // CREATE TABLE symbols (address INTEGER PRIMARY KEY, symbol TEXT,displacement INTEGER,filename TEXT, linenumber INTEGER );
    char cmd_create[] = "CREATE TABLE symbols ("
                        " address INTEGER PRIMARY KEY,"
                        " symbol TEXT,"
                        " displacement INTEGER,"
                        " filename TEXT,"
                        " linenumber INTEGER"
                        ");";

    rc = sqlite3_exec(db, cmd_create, NULL, NULL, NULL);
    if (rc)
    {
        fprintf(stderr, "Can't create table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // add table entries
    // N.B. I'm turning of the transaction checks here because I'm more interested in speed than safety
    rc = sqlite3_exec(db, "PRAGMA synchronous =  0", NULL, NULL, NULL);

    const char* sql_insert = "INSERT INTO symbols (address, symbol, displacement, filename, linenumber) VALUES (%d,'%s',%d,'%s',%d)";
    int count = 0;
    for (AddressMap::const_iterator it = a_map.begin(); it != a_map.end(); ++it)
    {
        AddressSymbol const& as = it->second;
        char cmd_buffer[1024];
        printf("Adding address to sqlite database:  %8d of %zd\r", count++, a_map.size());
        // may need to sanitize text for inclusion in commands
        std::string name_sanitized = std::string(as.symbolName);
        name_sanitized.erase(remove(name_sanitized.begin(), name_sanitized.end(), '\''), name_sanitized.end());
        std::string filename_sanitized = std::string(as.filename);
        filename_sanitized.erase(remove(filename_sanitized.begin(), filename_sanitized.end(), '\''), filename_sanitized.end());

        sprintf_s(cmd_buffer, sizeof(cmd_buffer), sql_insert, as.address, name_sanitized.c_str(), as.displacement, filename_sanitized.c_str(), as.lineNumber);
        rc = sqlite3_exec(db, cmd_buffer, NULL, NULL, NULL);
        if (rc)
        {
            fprintf(stderr, "\nError updating table: %s\n", sqlite3_errmsg(db));
        }
    }
}

// open the named sql db and create/replace a table with every information
// for every log event. This can be useful to run arbitrary SQL queries
// against that you may not currently have a tool UI for or are otherwise
// too complex/awkward for a UI. e.g. you can query for all the events that
// affect a certain memory range when looking for information about a memory stomp
// @todo
void build_event_db(const char* db_name)
{
    // not yet implemented
}

void usage()
{
    printf("memtrack v0.1\n");
    printf("Currently this application will simply create a sqlite\n"
           "database named 'address.db' with all the resolved callstack\n"
           "address symbols found in a memtrack capture session dictionary.\n"
           "This database is used for analysis scripts that do not want to deal\n"
           "with symbol resolution.\n\n"
           "Future revisions will likely have additional functionality.\n\n"
           "Usage: memtrack <Module name> <memtrack dictionary>\n"
           "   eg: memtrack CityofHeroes.exe memtrack_dict.dat\nn"
           "Make sure that the PDB is located in the same place as the module.\n\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
    SymbolHelper sh;
    const _TCHAR* module_name = "CityofHeroes.exe";
    const _TCHAR* dict_name = "memtrack_dict.dat";

    if (argc < 3)
    {
        usage();
        fprintf(stderr, "ERROR: incorrect number of arguments\n");
        return 0;
    }
    module_name = argv[1];
    dict_name = argv[2];

    // @todo the application should generate an initial packet in the log stream that has
    // the module information from the run which we can use to lookup symbols
    printf("loading module symbols...");
    if (!sh.LoadSymbolsForModule((const void*)0x400000, 0x80000000, 0, module_name))
    {
        fprintf(stderr, "\nERROR: Could not load symbols for '%s'\n", module_name);
        return -1;
    }
    printf("done.\n");
    //    sh.PrintSymbolSummary( 0x012dceac );

    // read the callstack data log and build a map of symbol resolutions
    AddressMap a_map;
    if (build_address_map(a_map, dict_name, sh))
    {
        printf("Address count: %zd\n\n", a_map.size());

        // populate sqlite db for other tools to more easily retrieve symbol information
        printf("Generating sql symbol db\n\n");
        build_address_db("address.db", a_map);
    }
    else
        return -1;

    return 0;
}
