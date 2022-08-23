#include <utilitieslib/stdtypes.h>
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <utilitieslib/components/gridpoly.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include "outputanim.h" 
#include <utilitieslib/assert/assert.h>
#include "processanim.h"
#include <utilitieslib/utils/file.h>
#include <sys/stat.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/Quat.h>

extern int total_uncompressed_position_keys; 
extern int    total_added_rot_nodes;
extern int    total_added_rot_keys;
extern int    total_added_pos_nodes;
extern int    total_added_pos_keys;
extern int total_dummy_rotation_keys;
extern int total_dummy_position_keys;
extern int maxrun[2];

extern int end_run_due_to_too_much_delta_error[2];
extern int end_run_due_to_switched_missing_key[2];
extern int end_run_due_to_end_of_track[2];
extern int total_runs[2];
extern int total_useless_runs[2];
extern int runhist[2][10000];


extern int non_matching_counts;

static int total_tracks[2];
static int total_fullkey_data[2];
static int total_correction_data[2];

extern int total_saved_keys[2];
extern int total_keys[2];

extern g_no_version_control;

//################ Memory Blocks ###################################
typedef struct
{
    char    *name;
    int        total;
    U8        *data;
    int        used;
} MemBlock;


MemBlock mem_blocks[] =
{
    { "ANIMDATA",            32000000 },
    { "ANIMTRACKS",            16000000 },
};

static void *getMemBase(int block_num)
{
MemBlock    *mblock;

    mblock = &mem_blocks[block_num];
    if (!mblock->data)
        mblock->data = calloc(mblock->total,1);
    return &mblock->data[mblock->used];
}

static void *mallocBlockMem(int block_num,int amount)
{
void    *ptr;
MemBlock    *mblock;

    getMemBase(block_num);
    mblock = &mem_blocks[block_num];
    if (amount > (mblock->total - mblock->used))
    {
        printf("Out of memory in anim block %s! (%d used, %d needed)\n",
            mblock->name,mblock->used,amount);
        FatalErrorf("Need to recompile this tool to fix this!\n");
    }
    ptr = &mblock->data[mblock->used];
    mblock->used += amount;
    return ptr;
}

void *allocBlockMem(int block_num,int amount)
{
void    *ptr;

    ptr = mallocBlockMem(block_num,amount);
    memset(ptr,0,amount);
    return ptr;
}
//############### End Manage Memeory Blocks ############################################


//Slightly half assed generalization, really all of outputPackSkeletonAnimTrack should be generalized so both
//vrml reader and old anim tracks cna use them. But this will do, and its better than before
void transferBoneData( BoneAnimTrack * bt, void * rot_fulls, void * pos_fulls )
{
    int track_size;

    //############ Add rotation data
    //Add Full Keys
    //// TO DO don't calculate all this here, do it in the processing step output shouldn't know about it
    if( bt->flags & ROTATION_UNCOMPRESSED )
        track_size = bt->rot_fullkeycount * SIZE_OF_ROTKEY_UNCOMPRESSED;
    else if( bt->flags & ROTATION_COMPRESSED_TO_8_BYTES )
        track_size = bt->rot_fullkeycount * SIZE_OF_ROTKEY_COMPRESSED_TO_8_BYTES;
    else if( bt->flags & ROTATION_COMPRESSED_TO_5_BYTES 
        || bt->flags & ROTATION_COMPRESSED_NONLINEAR )
        track_size = bt->rot_fullkeycount * SIZE_OF_ROTKEY_COMPRESSED_TO_5_BYTES;

    bt->rot_idx = allocBlockMem(MEM_ANIMDATA, track_size );
    memcpy( cpp_const_cast(void*)(bt->rot_idx), rot_fulls, track_size );    //each key value

    total_fullkey_data[ROTATION_KEY] += track_size; //debug

    //############ Add position data
    //Add full keys
    //// TO DO don't calculate all this here, do it in the processing step output shouldn't know about it
    if( bt->flags & POSITION_UNCOMPRESSED )
        track_size = bt->pos_fullkeycount * SIZE_OF_UNCOMPRESSED_POSKEY;
    else if( bt->flags & POSITION_COMPRESSED_TO_6_BYTES )
        track_size = bt->pos_fullkeycount * SIZE_OF_POSKEY_COMPRESSED_TO_6_BYTES;
    else
        assert(0);
    bt->pos_idx = allocBlockMem(MEM_ANIMDATA, track_size );
    memcpy( cpp_const_cast(void*)(bt->pos_idx), pos_fulls, track_size );    //each key value

    total_fullkey_data[POSITION_KEY] += track_size; //debug
}




/*Packs an node tree into an SkeletonAnimTrack and its animtracks
note that the name of the header is the full path of the .wrl file minus ".wrl"
*/
void outputPackSkeletonAnimTrack( SkeletonAnimTrack * skeleton, Node *root ) 
{
    int     i;
    Node    **nodelist,*node;
    BoneAnimTrack * bt;
    F32    longest = 0;

    nodelist = treeArray( root, &skeleton->bone_track_count );

    skeleton->bone_tracks = allocBlockMem( MEM_BONEANIMTRACKS, skeleton->bone_track_count * sizeof( BoneAnimTrack ) );
    memset( skeleton->bone_tracks, 0, sizeof(BoneAnimTrack) * skeleton->bone_track_count );

    for(i=0 ; i < skeleton->bone_track_count ; i++ )
    {
        node = nodelist[i];
        bt = &(skeleton->bone_tracks[i]);
        
        bt->id = node->anim_id; 
        bt->flags |= node->poskeys.flags;
        bt->flags |= node->rotkeys.flags;
        bt->rot_count = node->rotkeys.count;
        bt->pos_count = node->poskeys.count;
        bt->rot_fullkeycount = node->rotkeys.fullkeycount;
        bt->pos_fullkeycount = node->poskeys.fullkeycount;

        checkKeysBeforeExport( &node->poskeys, &node->rotkeys ); //debug check
    
        transferBoneData( bt, node->rotkeys.vals, node->poskeys.vals );

        //########### Screwy shit I don't get fully at least move it out of the output file
        //Find lengths and times and write them to the skeleton
        /*this is to figure out how long the animation track is by picking the longest of the 
        rotation track and xlation track.  Don't get %100
        */
        { 
            if (bt->rot_count && ( bt->rot_count-1 > longest) )
                longest = bt->rot_count-1;

            if (bt->pos_count && ( bt->pos_count-1 > longest) )
                longest = bt->pos_count-1;
        }
    }

    skeleton->length = longest;
} 


/*reset the globals (called after outputData and outputGroupInfo are done) So this 
*/
void outputResetVars()
{
    int        i;

    for(i=0;i<ARRAY_SIZE(mem_blocks);i++)
        mem_blocks[i].used = 0;
}

/*Writes the data to .trk and .skel, subtracting pointers along the way.
Am I doing pointer arithmetic right, or does it start on word boundries or something crazy, see ftell
*/
void outputAnimTrackToAnimFile( SkeletonAnimTrack * skeleton, char * targetFilePath ) 
{
    int    i, heirarchySize; 
    BoneAnimTrack * bts;
    BoneAnimTrack * bt;
    const SkeletonHeirarchy * skeletonHeirarchy = NULL;
    FILE *file;
    char *oldData = NULL;
    int oldSize;
    bool bJustCheckedOut = false;    // indicates if we just did checkout and can attempt to revert if necessary
    bool bExistingFile = false;        // true if the target file currently exists on disc with content, not indicative of version control status
    bool bControlNewFile = false;    // true if this asset needs to be added to version control

    // Prepare the path for the destination file if necessary
    mkdirtree(targetFilePath);

    // If the file already exists then load it up. This allows us to compare the newly processed data
    // to the existing data and implicitly tells us if we are going to be editing or adding a file with
    // version control (though not completely accurately in the case of offline processing)
    oldData = fileAlloc(targetFilePath, &oldSize);
    bExistingFile = oldData && oldSize;

    printfColor(COLOR_YELLOW, " [offline] Would %s: \"%s\"\n", bExistingFile ? "checkout":"add", targetFilePath);
    chmod(targetFilePath, _S_IREAD | _S_IWRITE );

    //**
    // Write the new data
    file = fopen( targetFilePath, "wb" );
    if (!file)
    {
        printfColor(COLOR_RED|COLOR_BRIGHT," Can't open file for writing: \"%s\"\n (Maybe file is locked or data dir does not exist?)\n", targetFilePath );
        free(oldData);
        return;
    }

    if( skeleton->skeletonHeirarchy )
    {
        heirarchySize = sizeof( SkeletonHeirarchy );
        //preserve pointer so I can write data below
        skeletonHeirarchy = skeleton->skeletonHeirarchy; 
        //then fix the pointer for the file write
        skeleton->skeletonHeirarchy = (const void *)((U32)sizeof( SkeletonAnimTrack )); //skeletonHeirarchy will be after SkeletonAnimTrack
    }
    else
    {
        heirarchySize = 0;
    }

    // #### write skeleton ######
    skeleton->headerSize = sizeof(SkeletonAnimTrack) + heirarchySize + mem_blocks[MEM_BONEANIMTRACKS].used;

    skeleton->bone_tracks = (void*)(uintptr_t)(sizeof(SkeletonAnimTrack) + heirarchySize); // how far into file bones block starts
            
    fwrite( skeleton, sizeof(SkeletonAnimTrack), 1, file );

    if( skeleton->skeletonHeirarchy )
        fwrite( skeletonHeirarchy, sizeof( SkeletonHeirarchy ), 1, file );

    // #### write bonetracks ######
    //  ( fix up all the pointers, then write the data ) 
    bts    = (void *)mem_blocks[MEM_BONEANIMTRACKS].data;

    for( i = 0 ; i < skeleton->bone_track_count ; i++ )
    {
        bt = &(bts[i]);
        assert( bt->rot_idx && bt->pos_idx );

        //pointer to this track, minus pointer to start of all track, plus size of header
        bt->rot_idx    = (void*)(uintptr_t)((size_t)skeleton->headerSize + ((size_t)bt->rot_idx - (size_t)mem_blocks[MEM_ANIMDATA].data));
        bt->pos_idx    = (void*)(uintptr_t)((size_t)skeleton->headerSize + ((size_t)bt->pos_idx - (size_t)mem_blocks[MEM_ANIMDATA].data));
    }

    fwrite( bts, mem_blocks[MEM_BONEANIMTRACKS].used, 1, file);

    //#### write data blocks ###
    fwrite( mem_blocks[MEM_ANIMDATA].data, mem_blocks[MEM_ANIMDATA].used, 1, file);

    fclose(file);

    // Compare previous and current data for no changes (and undo checkout)
    if (oldData)
    {
        char *newData = NULL;
        int newSize;

        newData = fileAlloc(targetFilePath, &newSize);

        free(oldData);
    }

}
///################### End Write data to a file ##########################################

// Debugging/Conversion

static const char* outputBuildTrackFlagString( int flags )
{
    static char _s[1024];

    *_s = 0;

    if ( flags & ROTATION_UNCOMPRESSED )
        strcat( _s, " | ROTATION_UNCOMPRESSED" );
    if ( flags & ROTATION_COMPRESSED_TO_5_BYTES )
        strcat( _s, " | ROTATION_COMPRESSED_TO_5_BYTES" );
    if ( flags & ROTATION_COMPRESSED_TO_8_BYTES )
        strcat( _s, " | ROTATION_COMPRESSED_TO_8_BYTES" );
    if ( flags & POSITION_UNCOMPRESSED )
        strcat( _s, " | POSITION_UNCOMPRESSED" );
    if ( flags & POSITION_COMPRESSED_TO_6_BYTES )
        strcat( _s, " | POSITION_COMPRESSED_TO_6_BYTES" );
    if ( flags & ROTATION_DELTACODED )
        strcat( _s, " | ROTATION_DELTACODED" );
    if ( flags & POSITION_DELTACODED )
        strcat( _s, " | POSITION_DELTACODED" );
    if ( flags & ROTATION_COMPRESSED_NONLINEAR )
        strcat( _s, " | ROTATION_COMPRESSED_NONLINEAR" );

    return _s;
}

static void outputPrintAnimHeader( FILE* f, SkeletonAnimTrack* pAnim )
{
    fprintf( f, "%-30s: %d\n", "headerSize",                                pAnim->headerSize );
    fprintf( f, "%-30s: %s\n", "name",                                            pAnim->name );
    fprintf( f, "%-30s: %s\n", "baseAnimName",                            pAnim->baseAnimName );
    fprintf( f, "%-30s: %f\n", "max_hip_displacement",            pAnim->max_hip_displacement );
    fprintf( f, "%-30s: %f\n", "length",                                        pAnim->length );
    fprintf( f, "%-30s: %d\n", "bone_track_count",                    pAnim->bone_track_count );
    fprintf( f, "%-30s: %d\n", "rotation_compression_type", pAnim->rotation_compression_type );
    fprintf( f, "%-30s: %d\n", "position_compression_type", pAnim->position_compression_type );
}

static void outputPrintBoneTrack( FILE* f, BoneAnimTrack* pTrack, SkeletonAnimTrack* pAnim )
{
    const char* strFlags = outputBuildTrackFlagString(pTrack->flags);

    fprintf( f, "BoneTrack: \"%s\"\n", bone_NameFromId(pTrack->id) );
//    fprintf( f, "%-30s: %d\n", "rot_idx",                        (U32)pTrack->rot_idx - (U32)pAnim );
//    fprintf( f, "%-30s: %d\n", "pos_idx",                        (U32)pTrack->pos_idx - (U32)pAnim );
    fprintf( f, "%-30s: %d\n", "rot_fullkeycount",    pTrack->rot_fullkeycount );
    fprintf( f, "%-30s: %d\n", "pos_fullkeycount",    pTrack->pos_fullkeycount );
    fprintf( f, "%-30s: %d\n", "rot_count",                    pTrack->rot_count );
    fprintf( f, "%-30s: %d\n", "pos_count",                    pTrack->pos_count );
    fprintf( f, "%-30s: %d (%s)\n", "id",                        pTrack->id, bone_NameFromId(pTrack->id) );
    fprintf( f, "%-30s: %d %s\n", "flags",                    pTrack->flags, strFlags );
    fprintf( f, "%-30s: %d\n", "pack_pad",                    pTrack->pack_pad );
    fprintf( f, "\n" );
}

static void outputGetKey( int iKey, const BoneAnimTrack * bt, Quat rQuat, Vec3 rPos )
{
    int iRotKey;
    int iPosKey;
    
    if (iKey && iKey >= bt->rot_count)
    {
        iRotKey = bt->rot_count - 1;
    }
    else
    {
        iRotKey = iKey;
    }

    if (iKey && iKey >= bt->pos_count)
    {
        iPosKey = bt->pos_count - 1;
    }
    else
    {
        iPosKey = iKey;
    }

    if( bt->flags & ROTATION_COMPRESSED_TO_5_BYTES )
    {
        U8* pKeyData = &((U8*)bt->rot_idx)[ iRotKey * 5 ];
        animExpand5ByteQuat( pKeyData, rQuat );
    }
    else if( bt->flags & ROTATION_COMPRESSED_NONLINEAR )
    {
        U8* pKeyData = &((U8*)bt->rot_idx)[ iRotKey * 5 ];
        animExpandNonLinearQuat( pKeyData, rQuat );
    }
    else if(bt->flags & ROTATION_UNCOMPRESSED )
    {
        F32* q = (F32*)&((F32*)bt->rot_idx)[ iRotKey * 4 ];
        rQuat[0] = q[0]; rQuat[1] = q[1]; rQuat[2] = q[2]; rQuat[3] = q[3]; 
    }
    else
    {
        printf( "<unknown or unhandled key data format>\n" );
    }

    if( bt->flags & POSITION_UNCOMPRESSED )
    {
        F32 *pos = (F32*)&(((F32*)bt->pos_idx)[iPosKey*3]);
        rPos[0] = pos[0]; rPos[1] = pos[1]; rPos[2] = pos[2];
    }
    else if( bt->flags & POSITION_COMPRESSED_TO_6_BYTES )
    {
        S16* pKeyData = &((S16*)bt->pos_idx)[ iPosKey * 3 ];
        animExpand6BytePosition( pKeyData, rPos );
    }
    else
    {
        printf( "<unknown or unhandled key data format>\n" );
    }
}

static void outputPrintBoneTrackKeys( FILE* f, BoneAnimTrack * bt )
{
    int i;
    const char* strFlags = outputBuildTrackFlagString(bt->flags);

    fprintf( f, "BoneTrack Keys: \"%s\"     %s\n\n", bone_NameFromId(bt->id), strFlags );

    fprintf( f, " ROTATION: %d\n", bt->rot_fullkeycount );
    if( bt->flags & ROTATION_COMPRESSED_TO_5_BYTES )
    {
        for(i = 0 ; i < bt->rot_fullkeycount ; i++ )
        {
            U8* pKeyData = &((U8*)bt->rot_idx)[ i * 5 ];
            Quat q;
            animExpand5ByteQuat( pKeyData, q );
            fprintf( f, "%3d: %f %f %f %f\n", i, q[0], q[1], q[2], q[3] );
        }
    }
    else if( bt->flags & ROTATION_COMPRESSED_NONLINEAR )
    {
        for(i = 0 ; i < bt->rot_fullkeycount ; i++ )
        {
            U8* pKeyData = &((U8*)bt->rot_idx)[ i * 5 ];
            Quat q;
            animExpandNonLinearQuat( pKeyData, q );
            fprintf( f, "%3d: %f %f %f %f\n", i, q[0], q[1], q[2], q[3] );
        }
    }
    else if(bt->flags & ROTATION_UNCOMPRESSED )
    {
        for(i = 0 ; i < bt->rot_fullkeycount ; i++ )
        {
            F32* q = (F32*)&((F32*)bt->rot_idx)[ i * 4 ];
            fprintf( f, "%3d: %f %f %f %f\n", i, q[0], q[1], q[2], q[3] );
        }
    }
    else
    {
        fprintf( f, "<unknown or unhandled key data format>\n" );
    }

    fprintf( f, "\n" );
    fprintf( f, " POSITION: key count: %d\n", bt->pos_fullkeycount );
    if( bt->flags & POSITION_UNCOMPRESSED )
    {
        for( i = 0 ; i < bt->pos_fullkeycount ; i++ )
        {
            F32 *pos = (F32*)&(((F32*)bt->pos_idx)[i*3]);
            ConvertCoordsGameTo3DSMAX( pos, pos );
            fprintf( f, "%3d: %f %f %f\n", i, pos[0], pos[1], pos[2] );
        }
    }
    else if( bt->flags & POSITION_COMPRESSED_TO_6_BYTES )
    {
        for( i = 0 ; i < bt->pos_fullkeycount ; i++ )
        {
            S16* pKeyData = &((S16*)bt->pos_idx)[ i * 3 ];
            Vec3 pos;
            animExpand6BytePosition( pKeyData, pos );
            // back to 3DS Max coord system
            ConvertCoordsGameTo3DSMAX( pos, pos );
            fprintf( f, "%3d: %f %f %f\n", i, pos[0], pos[1], pos[2] );
        }
    }
    else
    {
        fprintf( f, "<unknown or unhandled key data format>\n" );
    }
    fprintf( f, "\n\n" );
}


static void indent(FILE* f, int depth)
{
    int i;
    for (i=0; i< depth;++i)
    {
        fprintf( f, "    " );
    }
}

static void outputPrintHeaderANIMX( FILE* f, char* srcFile, SkeletonAnimTrack* pAnim )
{
    const int kVersionANIMX = 200;
    fprintf(f, "# NCsoft CoH Animation Extraction\n" );
    fprintf(f, "# Generated from GetAnimation2\n"    );                    
    fprintf(f, "#\t\tSource File: %s\n\n", srcFile );

    fprintf(f,"Version %d\n", kVersionANIMX );
    fprintf(f,"SourceName %s\n", (srcFile && strlen(srcFile) > 0) ? srcFile:"" );
    fprintf(f,"TotalFrames %d\n", (int)pAnim->length );
    fprintf(f,"FirstFrame %d\n", 0 );
}

static void outputPrintBoneTrackANIMX( FILE* f, BoneAnimTrack* pTrack, SkeletonAnimTrack* pAnim )
{
    int maxKeys = max( pTrack->rot_count, pTrack->pos_count );
    int i;
    int nIndent = 0;

    indent(f,nIndent); fprintf( f, "Bone \"%s\"\n", bone_NameFromId(pTrack->id) );
    indent(f,nIndent); fprintf( f, "{\n" );
    nIndent++;

    // first keys are the base pose which we get in the SKELX export and shouldn't be part of ANIMX
    if (maxKeys==1) maxKeys++;
        
    for(i = 1 ; i < maxKeys ; i++ )
    {
        Quat q;
        Vec3 pos, axis;
        float angle;

        outputGetKey( i, pTrack, q, pos );

        quatToAxisAngle( q, axis, &angle );

        if (angle < 1e-5  || normalVec3(axis) < 1e-5 )
        {
            // MAX exporter spits out 0 vector for axis when no rotation
            // TODO change that?
            axis[0] = axis[1] = axis[2] = 0.0f;
        }

        // TODO: Need to convert back to world coords

        // back to 3DS Max coord system
        ConvertCoordsGameTo3DSMAX( axis, axis );
        ConvertCoordsGameTo3DSMAX( pos, pos );

        // we can either negate the angle or negate the axis
        // negating the axis direction fits how stuff comes out of MAX
        axis[0] = axis[0] != 0.0f ? -axis[0] : 0.0f; // -0 is ugly in text exports
        axis[1] = axis[1] != 0.0f ? -axis[1] : 0.0f;
        axis[2] = axis[2] != 0.0f ? -axis[2] : 0.0f;

        indent(f,nIndent); fprintf( f, "Transform\n");
        indent(f,nIndent); fprintf( f, "{\n" );
        nIndent++;
        indent(f,nIndent); fprintf( f, "Axis %.7g %.7g %.7g \n", axis[0], axis[1], axis[2] );
        indent(f,nIndent); fprintf( f, "Angle %.7g\n", angle );
        indent(f,nIndent); fprintf( f, "Translation %.7g %.7g %.7g \n", pos[0], pos[1], pos[2] );
        indent(f,nIndent); fprintf( f, "Scale %.7g %.7g %.7g \n",  1.0f, 1.0f, 1.0f );
        nIndent--;
        indent(f,nIndent); fprintf( f, "}\n\n" );
    }
    nIndent--;
    indent(f,nIndent); fprintf( f, "}\n\n" );
}

/******************************************************************************
*****************************************************************************/
static void outputPrintBoneHierarchyComment( FILE* f, int start, const BoneLink* pSkeletonLinks, int** peaDecoStack ) 
{
    while (start >= 0)
    {
        char decotext[1024];
        U32 i;
        int next = pSkeletonLinks[start].next;
        int child = pSkeletonLinks[start].child;
        U32 depth = ea32Size(peaDecoStack);
        fprintf( f, "#\t" );
        *decotext = 0;
        for (i = 0; i < depth; ++i)
        {
            if ((*peaDecoStack)[i])
                strcat(decotext, "| " );
            else
                strcat(decotext, "  " );
        }
        fprintf( f, "%s|_ %s\n", decotext, bone_NameFromId(start) );
        if (child >= 0)
        {
            if (next >= 0)
                    ea32Push(peaDecoStack, 1);
                else
                    ea32Push(peaDecoStack, 0);
                outputPrintBoneHierarchyComment( f, child, pSkeletonLinks, peaDecoStack );
                ea32Pop(peaDecoStack);
        }
        start = next;
    }
}

static int countChildren( int boneID, const BoneLink* pSkeletonLinks )
{
    int numChildren = 0;
    if ( boneID >= 0 )
    {
        int child = pSkeletonLinks[boneID].child;
        while (child >= 0)
        {
            child = pSkeletonLinks[child].next;    // walk through siblings
            numChildren++;
        }
    }
    return numChildren;
}

static void outputPrintBoneHierarchy( FILE* f, int start, const SkeletonAnimTrack * pAnim, const BoneLink* pSkeletonLinks, int depth ) 
{
    while (start >= 0)
    {
        int next = pSkeletonLinks[start].next;
        int nIndent = depth;
        int numChildren = countChildren(start, pSkeletonLinks); // count number of immediate children of this node

        indent(f,nIndent); fprintf( f, "Bone \"%s\"\n", bone_NameFromId(start) );
        indent(f,nIndent); fprintf( f, "{\n" );
        nIndent++;

        // extract the key 0 position and rotation
        {
            Quat q;
            Vec3 pos, axis;
            float angle;
            Mat3 M;
            const BoneAnimTrack* pBoneTrack = animFindTrackInSkeleton( pAnim, start );

            outputGetKey( 0, pBoneTrack, q, pos );

            quatToAxisAngle( q, axis, &angle );
            if (lengthVec3Squared(axis) < 1e-10 )
            {
                axis[0] = axis[2] = 0.0f;
                axis[1] = 1.0f;
            }

            // back to 3DS Max coord system
            ConvertCoordsGameTo3DSMAX( axis, axis );
            ConvertCoordsGameTo3DSMAX( pos, pos );

            indent(f,nIndent); fprintf( f, "Axis %.7g %.7g %.7g \n", axis[0], axis[1], axis[2] );
            indent(f,nIndent); fprintf( f, "Angle %.7g\n", angle );
            indent(f,nIndent); fprintf( f, "Translation %.7g %.7g %.7g \n", pos[0], pos[1], pos[2] );
            indent(f,nIndent); fprintf( f, "Scale %.7g %.7g %.7g \n",  1.0f, 1.0f, 1.0f );
            indent(f,nIndent); fprintf( f, "\n" );

            axisAngleToQuat( axis, angle, q );
            quatToMat(q, M);
            indent(f,nIndent); fprintf( f, "Row0 %.7g %.7g %.7g \n", M[0][0], M[0][1], M[0][2] );
            indent(f,nIndent); fprintf( f, "Row1 %.7g %.7g %.7g \n", M[1][0], M[1][1], M[1][2] );
            indent(f,nIndent); fprintf( f, "Row2 %.7g %.7g %.7g \n", M[2][0], M[2][1], M[2][2] );
            indent(f,nIndent); fprintf( f, "Row3 %.7g %.7g %.7g \n", pos[0], pos[1], pos[2] );
            indent(f,nIndent); fprintf( f, "\n" );
        }

        if (numChildren > 0)
        {
            int child = pSkeletonLinks[start].child;

            indent(f,nIndent); fprintf( f, "\n" );
            indent(f,nIndent); fprintf( f, "Children %d\n\n", numChildren );
            depth += 1;
            outputPrintBoneHierarchy( f, child, pAnim, pSkeletonLinks, depth );
            depth -= 1;
        }
        nIndent--;
        indent(f,nIndent); fprintf( f, "}\n\n" );
        start = next;
    }
}


/******************************************************************************
dump the binary runtime animation skel hierarchy as a text file for easy debugging/comparison
*****************************************************************************/
static void outputAnimFileSkeltonHierarchyToText( char * dstFilePath, SkeletonAnimTrack * pAnim, char* pSrcName )
{
    const int kVersionSKELX = 200;
    int* eaDecoStack = NULL;
    FILE* f;
    
    f = fopen(dstFilePath, "wt");

    fprintf(f, "# NCsoft CoH Skeleton Extraction\n" );
    fprintf(f, "# Generated from GetAnimation2\n"    );                    
    fprintf(f, "#\t\tSource File: %s\n\n", (pSrcName && strlen(pSrcName) > 0) ? pSrcName:"" );

    fprintf(f,"Version %d\n", kVersionSKELX );
    fprintf(f,"SourceName %s\n", (pSrcName && strlen(pSrcName) > 0) ? pSrcName:"" );

    // text representation of node hierarchy
    fprintf( f, "\n# NODE HIERARCHY\n" );
    outputPrintBoneHierarchyComment( f, pAnim->skeletonHeirarchy->heirarchy_root, pAnim->skeletonHeirarchy->skeleton_heirarchy, &eaDecoStack );
    fprintf( f, "\n" );

    outputPrintBoneHierarchy( f, pAnim->skeletonHeirarchy->heirarchy_root, pAnim, pAnim->skeletonHeirarchy->skeleton_heirarchy, 0 );
    fprintf( f, "\n\n" );

    fclose(f);
}

// a raw text output that tells us the header details and bone track flags
static void outputAnimFileToRawText( char* dstFileName, SkeletonAnimTrack* pAnim )
{
    int uiBoneIndex;
    FILE* f = fopen(dstFileName, "wt");

    // comments
    fprintf( f, "# NOTE: Key values have been converted back to the source 3DS MAX\n");
    fprintf( f, "#       system for easier comparison. In game is (-X,Z,-Y)\n");
    fprintf( f, "\n\n" );

    // header
    outputPrintAnimHeader( f, pAnim );
    fprintf( f, "\n\n" );

    for (uiBoneIndex = 0; uiBoneIndex < pAnim->bone_track_count; ++uiBoneIndex)
    {
        BoneAnimTrack* pBoneTrack = &pAnim->bone_tracks[uiBoneIndex];
        outputPrintBoneTrack( f, pBoneTrack, pAnim );
    }
    fprintf( f, "\n\n" );

    for (uiBoneIndex = 0; uiBoneIndex < pAnim->bone_track_count; ++uiBoneIndex)
    {
        BoneAnimTrack* pBoneTrack = &pAnim->bone_tracks[uiBoneIndex];
        outputPrintBoneTrackKeys( f, pBoneTrack );
    }
    fprintf( f, "\n\n" );

    fclose(f);
}

// Convert the runtime animation back to a source format ANIMX file that
// can be used for debugging or reprocessing (e.g. source MAX file is not
// available for re-export)
static void outputAnimFileToANIMX( char* dstFileName, SkeletonAnimTrack* pAnim )
{
    int uiBoneIndex;
    FILE* f = fopen(dstFileName, "wt");

    // header
    outputPrintHeaderANIMX( f, dstFileName, pAnim );
    fprintf( f, "\n\n" );

    for (uiBoneIndex = 0; uiBoneIndex < pAnim->bone_track_count; ++uiBoneIndex)
    {
        BoneAnimTrack* pBoneTrack = &pAnim->bone_tracks[uiBoneIndex];
        outputPrintBoneTrackANIMX( f, pBoneTrack, pAnim );
    }
    fprintf( f, "\n\n" );

    fclose(f);
}

/******************************************************************************
dump the binary runtime animation as a text files for easy debugging/comparison
*****************************************************************************/
void outputAnimFileToText( char * srcAnimFilePath )
{
    SkeletonAnimTrack * pAnim;
    char textbuf[1024];

    printf("Exporting runtime .anim to text files: %s\n", srcAnimFilePath );

    pAnim = animLoadAnimFile( srcAnimFilePath );
    if (!pAnim)
        return;

    // if the .anim file has a skeleton hierarchy
    // extract that as it's own text file for potential use as a processing skeleton (.SKELX)
    if (pAnim->skeletonHeirarchy)
    {
        sprintf( textbuf, "%s.skelx.txt", srcAnimFilePath );
        outputAnimFileSkeltonHierarchyToText( textbuf, pAnim, textbuf );
    }

//    sprintf( textbuf, "%s.animx.txt", srcAnimFilePath );
//    outputAnimFileToANIMX( textbuf, pAnim );

    sprintf( textbuf, "%s.raw.txt", srcAnimFilePath );
    outputAnimFileToRawText( textbuf, pAnim );
}

/******************************************************************************
    Extracts source files such as .SKELX skeleton hierarchy from the given
    runtime .anim that can be used for animation processing
*****************************************************************************/
void outputAnimFileToSourceAssets( char * srcAnimFilePath, char* pDstPath )
{
    SkeletonAnimTrack * pAnim;
    char path[1024], dstName[1024];
    char* s;

    printf("Exporting runtime .anim to source asset files: %s\n", srcAnimFilePath );

    pAnim = animLoadAnimFile( srcAnimFilePath );
    if (!pAnim)
    {
        printf("Error: .anim file could not be loaded");
        return;
    }

    if (!pDstPath)
    {
        pDstPath = srcAnimFilePath;
    }

    strcpy_s(path, sizeof(path), pDstPath );
    s = strrchr(path, '.');
    if (s) *s = 0;    // chop off extension

    // if the .anim file has a skeleton hierarchy
    // extract that as it's own text file for potential use as a processing skeleton (.SKELX)
    if (pAnim->skeletonHeirarchy)
    {
        sprintf_s( dstName, sizeof(dstName), "%s.SKELX", path );
        outputAnimFileSkeltonHierarchyToText( dstName, pAnim, srcAnimFilePath );
    }
    else
    {
        printf("File does not contain a skeletal hierarchy\n");
    }

    // TODO, Add ANIMX export here, need to convert back to world coords instead of relative
#if 0
    sprintf_s( dstName, sizeof(dstName), "%s.ANIMX", path );
    outputAnimFileToANIMX( dstName, pAnim );
#endif
}
