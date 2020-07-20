#include "NovodeX/NwWrapper.h"

#if NOVODEX
//#include "NxMath.h"     // needs to be included before mathutil.h or there's a define conflict

#define NO_TEXT_PARSER 1
#include "NovodeX/NwRagdoll.h"

#include "NxActor.h"
#include "NxPhysicsSDK.h"
#include "NxMaterial.h"
#include "NxScene.h"
#include "NxJoint.h"
#include "NxFixedJoint.h"
#include "NxFixedJointDesc.h"
#include "NxSphericalJointDesc.h"
#include "NxRevoluteJointDesc.h"



extern "C"
{
    #include "seq/anim.h"
    #include "entity/ragdoll.h"
    #include "seq/gfxtree.h"
    #include "seq/seq.h"
    #include <utilitieslib/utils/mathutil.h>
    #include <utilitieslib/components/queue.h>
    #include "entity/entity.h"
    #include "render/renderprim.h"
    #include "entity/motion.h"
    #include "group/groupnovodex.h"
    #include "entity/costume.h"
    #include <utilitieslib/utils/error.h>
    #include <utilitieslib/utils/timing.h>
    #include <utilitieslib/components/earray.h>
    #include <utilitieslib/utils/Quat.h>
}

typedef enum ragGeom
{
    RAG_GEOM_CAPSULE,
    RAG_GEOM_BOX,
    RAG_GEOM_SPHERE

} ragGeom;

typedef struct RagdollGeometry
{
    Mat4 mOffsetMat;
    F32 fHeight;
    F32 fRadius;
    Vec3 vSize;
    bool bCCD;
    ragGeom geom;
    bool bOffset;
} RagdollGeometry;

extern NxState nx_state;
 
// NovodeX-specific parameters.
extern NxPhysicsSDK*    nxSDK; 
extern NxScene*         nxScene[];

static struct 
{
    float swing_spring;// = 0.2f;
    float swing_damper;// = 0.1f;
    float twist_spring;// = 0.2f;
    float twist_damper;// = 0.1f;
    float swing_hardness;// = 0.9f;
    float twist_hardness;// = 0.9f;
    float hinge_hardness;// = 0.9f;
    float projection_distance;// = 0.15f;
    float linearDamping; // novodex default - 0.0f
    float angularDamping; // novodex default 0.05f
    float linearSleepVelocity; // novodex default - 0.15f
    float angularSleepVelocity; // novodex default - 0.14f
    bool doCCD; // true
    bool  bInited;
} ragdollConstants;

static Queue ragdollCreationQueue;

static Queue ragdollDeletionWithFreeQueue;

#define DEFAULT_SWING_SPRING ragdollConstants.swing_spring
#define DEFAULT_SWING_DAMPER ragdollConstants.swing_damper

#define DEFAULT_TWIST_SPRING ragdollConstants.twist_spring
#define DEFAULT_TWIST_DAMPER ragdollConstants.twist_damper

#define DEFAULT_SWING_HARDNESS ragdollConstants.swing_hardness
#define DEFAULT_TWIST_HARDNESS ragdollConstants.twist_hardness
#define DEFAULT_HINGE_HARDNESS ragdollConstants.hinge_hardness

#define DEFAULT_PROJECTON_DIST ragdollConstants.projection_distance
#define DEFAULT_PROJECTION_METHOD NX_JPM_POINT_MINDIST
//#define DEFAULT_PROJECTION_METHOD NX_JPM_NONE

NxJoint* jointRagdoll( NxActor* parentActor, NxActor* childActor, U8 boneId, Vec3 vGlobalAnchor, int iScene )
{
    NxJoint* jointToReturn = NULL;
    NxVec3 nxvGlobalAnchor;
    U8 uiBoneId = boneId;
    copyVec3(vGlobalAnchor, nxvGlobalAnchor);
    switch (uiBoneId)
    {
        xcase BONEID_CHEST:
        {
            NxSphericalJointDesc jointDesc;
            jointDesc.actor[0] = parentActor;
            jointDesc.actor[1] = childActor;

            NxMat33 mChildOrientation = childActor->getGlobalOrientation();
            NxMat33 mParentOrientation = parentActor->getGlobalOrientation();
            NxMat33 mUnitMat;
            mUnitMat.id();

            parentActor->setGlobalOrientation(mUnitMat);
            childActor->setGlobalOrientation(mUnitMat);


            NxVec3 vGlobalAxis = mUnitMat.getColumn(1);
            //vGlobalAxis = mParentOrientation * vGlobalAxis;
            jointDesc.setGlobalAxis(vGlobalAxis);


            childActor->setGlobalOrientation(mChildOrientation);
            parentActor->setGlobalOrientation(mParentOrientation);

            jointDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
            jointDesc.swingLimit.value = RAD(25.0f);
            jointDesc.swingLimit.hardness = DEFAULT_SWING_HARDNESS;
            jointDesc.swingLimit.restitution = 0.5;

            jointDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
            jointDesc.swingSpring.spring = 0.5;
            jointDesc.swingSpring.damper = 0.2;

            jointDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
            jointDesc.twistLimit.low.value = -RAD(60.0f);
            jointDesc.twistLimit.low.hardness = DEFAULT_TWIST_HARDNESS;
            jointDesc.twistLimit.low.restitution = 0.5;

            jointDesc.twistLimit.high.value = RAD(60.0f);
            jointDesc.twistLimit.high.hardness = DEFAULT_TWIST_HARDNESS;
            jointDesc.twistLimit.high.restitution = 0.5;

            jointDesc.flags |= NX_SJF_TWIST_SPRING_ENABLED;
            jointDesc.twistSpring.spring = 0.5;
            jointDesc.twistSpring.damper = 0.2;


            jointDesc.jointFlags &= ~NX_JF_COLLISION_ENABLED;
            jointDesc.projectionDistance = DEFAULT_PROJECTON_DIST;
            jointDesc.projectionMode = DEFAULT_PROJECTION_METHOD;


            jointDesc.setGlobalAnchor(nxvGlobalAnchor);
            jointToReturn = nxScene[iScene]->createJoint(jointDesc);
        }

        xcase BONEID_HEAD:
        {
            NxSphericalJointDesc jointDesc;
            jointDesc.actor[0] = parentActor;
            jointDesc.actor[1] = childActor;

            NxMat33 mChildOrientation = childActor->getGlobalOrientation();
            NxMat33 mParentOrientation = parentActor->getGlobalOrientation();
            NxMat33 mUnitMat;
            mUnitMat.id();

            parentActor->setGlobalOrientation(mUnitMat);
            childActor->setGlobalOrientation(mUnitMat);


            NxVec3 vGlobalAxis = mUnitMat.getColumn(1);
            //vGlobalAxis = mParentOrientation * vGlobalAxis;
            jointDesc.setGlobalAxis(vGlobalAxis);


            childActor->setGlobalOrientation(mChildOrientation);
            parentActor->setGlobalOrientation(mParentOrientation);
            
            jointDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
            jointDesc.swingLimit.value = RAD(35.0f);
            jointDesc.swingLimit.hardness = DEFAULT_SWING_HARDNESS;
            jointDesc.swingLimit.restitution = 0.5;

            jointDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
            jointDesc.swingSpring.spring = 0.5;
            jointDesc.swingSpring.damper = 0.2;

            jointDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
            jointDesc.twistLimit.low.value = -RAD(80.0f);
            jointDesc.twistLimit.low.hardness = DEFAULT_TWIST_HARDNESS;
            jointDesc.twistLimit.low.restitution = 0.5;

            jointDesc.twistLimit.high.value = RAD(80.0f);
            jointDesc.twistLimit.high.hardness = DEFAULT_TWIST_HARDNESS;
            jointDesc.twistLimit.high.restitution = 0.5;

#ifdef DEFAULT_TWIST_SPRING
            jointDesc.flags |= NX_SJF_TWIST_SPRING_ENABLED;
            jointDesc.twistSpring.spring = DEFAULT_TWIST_SPRING;
            jointDesc.twistSpring.damper = DEFAULT_TWIST_DAMPER;
#endif


            jointDesc.jointFlags &= ~NX_JF_COLLISION_ENABLED;
            jointDesc.projectionDistance = DEFAULT_PROJECTON_DIST;
            jointDesc.projectionMode = DEFAULT_PROJECTION_METHOD;

            
            jointDesc.setGlobalAnchor(nxvGlobalAnchor);
            jointToReturn = nxScene[iScene]->createJoint(jointDesc);
        }

        xcase BONEID_UARMR:
        case  BONEID_UARML:
        {
            NxSphericalJointDesc jointDesc;
            jointDesc.actor[0] = parentActor;
            jointDesc.actor[1] = childActor;

            // Ok, first we need to rotate the child actor so that it is in the identity matrix wrt the parent
            
            NxMat33 mChildOrientation = childActor->getGlobalOrientation();
            NxMat33 mParentOrientation = parentActor->getGlobalOrientation();
            NxMat33 mUnitMat;
            mUnitMat.id();

            parentActor->setGlobalOrientation(mUnitMat);


            // Orient the child into the right position.
            bool bApplyOffset = false;

            NxMat33 mFixOrientation;
            F32 fFlipSign = 1.0f;

            if ( boneId == 8)
            {
                fFlipSign = -1.0f;
                bApplyOffset = true;
                mFixOrientation.rotX(RAD(180.0f));
                mUnitMat *= mFixOrientation;
            //    mFixOrientation.rotY(RAD(180.0f));
            //    mUnitMat *= mFixOrientation;

            }


            mFixOrientation.rotY(-RAD(60.0f));
            mUnitMat *= mFixOrientation;

            mFixOrientation.rotZ(RAD(90.0f) * fFlipSign);
            mUnitMat *= mFixOrientation;

            childActor->setGlobalOrientation(mUnitMat);


            NxVec3 vGlobalAxis = -mUnitMat.getColumn(1);
            //vGlobalAxis = mParentOrientation * vGlobalAxis;
            jointDesc.setGlobalAxis(vGlobalAxis);



            childActor->setGlobalOrientation(mChildOrientation);
            parentActor->setGlobalOrientation(mParentOrientation);



            jointDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
            jointDesc.swingLimit.value = RAD(70.0f);
            jointDesc.swingLimit.hardness = DEFAULT_SWING_HARDNESS;
            jointDesc.swingLimit.restitution = 0.5;

#ifdef DEFAULT_SWING_SPRING
            jointDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
            jointDesc.swingSpring.spring = DEFAULT_SWING_SPRING;
            jointDesc.swingSpring.damper = DEFAULT_SWING_DAMPER;
#endif

            F32 fOffsetAngle = 0.0f;

            if ( bApplyOffset )
                fOffsetAngle = RAD(180.0f);

            jointDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
            jointDesc.twistLimit.low.value = -RAD(70.0f) + fOffsetAngle;
            jointDesc.twistLimit.low.hardness = DEFAULT_TWIST_HARDNESS;
            jointDesc.twistLimit.low.restitution = 0.5;

            jointDesc.twistLimit.high.value = RAD(70.0f) + fOffsetAngle;
            jointDesc.twistLimit.high.hardness = DEFAULT_TWIST_HARDNESS;
            jointDesc.twistLimit.high.restitution = 0.5;

#ifdef DEFAULT_TWIST_SPRING
            jointDesc.flags |= NX_SJF_TWIST_SPRING_ENABLED;
            jointDesc.twistSpring.spring = DEFAULT_TWIST_SPRING;
            jointDesc.twistSpring.damper = DEFAULT_TWIST_DAMPER;
            //jointDesc.twistSpring.targetValue = -RAD(70.0f) + fOffsetAngle;
#endif

            jointDesc.jointFlags &= ~NX_JF_COLLISION_ENABLED;
            jointDesc.projectionDistance = DEFAULT_PROJECTON_DIST;
            jointDesc.projectionMode = DEFAULT_PROJECTION_METHOD;


            jointDesc.setGlobalAnchor(nxvGlobalAnchor);
            jointToReturn = nxScene[iScene]->createJoint(jointDesc);
        }
    
        xcase BONEID_LARMR:
        case  BONEID_LARML:
        {
            NxRevoluteJointDesc jointDesc;
            jointDesc.actor[0] = parentActor;
            jointDesc.actor[1] = childActor;

            // Ok, first we need to rotate the child actor so that it is in the identity matrix wrt the parent

            NxMat33 mChildOrientation = childActor->getGlobalOrientation();
            NxMat33 mParentOrientation = parentActor->getGlobalOrientation();
            NxMat33 mUnitMat;
            mUnitMat.id();

            parentActor->setGlobalOrientation(mUnitMat);


            NxMat33 mFixOrientation;
            F32 fOffsetAngle = 0.0f;
            F32 fFlipSign = 1.0f;

            if ( boneId == 10)
            {
                fFlipSign = -1.0f;
            }
            /*
                fOffsetAngle = 0.0f;
                mFixOrientation.rotY(RAD(260.0f));
                mUnitMat *= mFixOrientation;

            }
            else
            {
                mFixOrientation.rotX(RAD(180.0f));
                mUnitMat *= mFixOrientation;

            }
            */

            //mFixOrientation.rotZ(RAD(90.0f) * fFlipSign);
            //mUnitMat *= mFixOrientation;

            //fOffsetAngle = RAD(90.0f);
            childActor->setGlobalOrientation(mUnitMat);

            NxVec3 vGlobalAxis = mUnitMat.getColumn(0);
            //vGlobalAxis = mParentOrientation * vGlobalAxis;
            jointDesc.setGlobalAxis(vGlobalAxis);

            childActor->setGlobalOrientation(mChildOrientation);
            parentActor->setGlobalOrientation(mParentOrientation);



            jointDesc.flags |= NX_RJF_LIMIT_ENABLED;
            jointDesc.limit.high.value = RAD(135.0f) + fOffsetAngle;
            jointDesc.limit.high.hardness = DEFAULT_HINGE_HARDNESS;
            jointDesc.limit.high.restitution = 0.5;

            jointDesc.limit.low.value = RAD(5.0f) + fOffsetAngle;
            jointDesc.limit.low.hardness = DEFAULT_HINGE_HARDNESS;
            jointDesc.limit.low.restitution = 0.5;

#ifdef DEFAULT_SWING_SPRING
            jointDesc.flags |= NX_RJF_SPRING_ENABLED;
            jointDesc.spring.spring = 0.1f;
            jointDesc.spring.damper = DEFAULT_SWING_DAMPER;
            jointDesc.spring.targetValue = RAD(10.0f);
#endif

            jointDesc.jointFlags &= ~NX_JF_COLLISION_ENABLED;
            jointDesc.projectionDistance = DEFAULT_PROJECTON_DIST;
            jointDesc.projectionMode = DEFAULT_PROJECTION_METHOD;


            jointDesc.setGlobalAnchor(nxvGlobalAnchor);
            jointToReturn = nxScene[iScene]->createJoint(jointDesc);
        }

        xcase BONEID_ULEGR:
        case  BONEID_ULEGL:
        {
            NxSphericalJointDesc jointDesc;
            jointDesc.actor[0] = parentActor;
            jointDesc.actor[1] = childActor;

            // Ok, first we need to rotate the child actor so that it is in the identity matrix wrt the parent

            NxMat33 mChildOrientation = childActor->getGlobalOrientation();
            NxMat33 mParentOrientation = parentActor->getGlobalOrientation();
            NxMat33 mFixOrientation;
            mFixOrientation.rotX(RAD(-15.0f));
            mParentOrientation *= mFixOrientation;
            if ( boneId == 23)
                mFixOrientation.rotZ(RAD(15.0f));
            else
                mFixOrientation.rotZ(RAD(-15.0f));
            mParentOrientation *= mFixOrientation;
            childActor->setGlobalOrientation(mParentOrientation);

            NxVec3 vGlobalAxis(0.0f,-1.0f,0.0f);
            vGlobalAxis = mParentOrientation * vGlobalAxis;
            jointDesc.setGlobalAxis(vGlobalAxis);

            childActor->setGlobalOrientation(mChildOrientation);
            jointDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
            jointDesc.swingLimit.value = RAD(35.0f);
            jointDesc.swingLimit.hardness = DEFAULT_SWING_HARDNESS;
            jointDesc.swingLimit.restitution = 0.5;

#ifdef DEFAULT_SWING_SPRING
            jointDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
            jointDesc.swingSpring.spring = DEFAULT_SWING_SPRING;
            jointDesc.swingSpring.damper = DEFAULT_SWING_DAMPER;
#endif


            jointDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
            jointDesc.twistLimit.low.value = -RAD(45.0f);
            jointDesc.twistLimit.low.hardness = DEFAULT_TWIST_HARDNESS;
            jointDesc.twistLimit.low.restitution = 0.5;

            jointDesc.twistLimit.high.value = RAD(45.0f);
            jointDesc.twistLimit.high.hardness = DEFAULT_TWIST_HARDNESS;
            jointDesc.twistLimit.high.restitution = 0.5;

#ifdef DEFAULT_TWIST_SPRING
            jointDesc.flags |= NX_SJF_TWIST_SPRING_ENABLED;
            jointDesc.twistSpring.spring = DEFAULT_TWIST_SPRING;
            jointDesc.twistSpring.damper = DEFAULT_TWIST_DAMPER;
#endif

            jointDesc.jointFlags &= ~NX_JF_COLLISION_ENABLED;
            jointDesc.projectionDistance = DEFAULT_PROJECTON_DIST;
            jointDesc.projectionMode = DEFAULT_PROJECTION_METHOD;


            jointDesc.setGlobalAnchor(nxvGlobalAnchor);
            jointToReturn = nxScene[iScene]->createJoint(jointDesc);
        }

        xcase BONEID_LLEGR:
        case  BONEID_LLEGL:
        {
            NxRevoluteJointDesc jointDesc;
            jointDesc.actor[0] = parentActor;
            jointDesc.actor[1] = childActor;

            // Ok, first we need to rotate the child actor so that it is in the identity matrix wrt the parent

            NxMat33 mChildOrientation = childActor->getGlobalOrientation();
            NxMat33 mParentOrientation = parentActor->getGlobalOrientation();
//            NxMat33 mFixOrientation;
//            mFixOrientation.rotX(RAD(80.0f));
//            mParentOrientation *= mFixOrientation;
            childActor->setGlobalOrientation(mParentOrientation);

            NxVec3 vGlobalAxis(1.0f,0.0f,0.0f);
            vGlobalAxis = mParentOrientation * vGlobalAxis;
            jointDesc.setGlobalAxis(vGlobalAxis);

            childActor->setGlobalOrientation(mChildOrientation);

            jointDesc.flags |= NX_RJF_LIMIT_ENABLED;
            jointDesc.limit.high.value = RAD(0.0f);
            jointDesc.limit.high.hardness = DEFAULT_HINGE_HARDNESS;
            jointDesc.limit.high.restitution = 0.5;

            jointDesc.limit.low.value = -RAD(120.0f);
            jointDesc.limit.low.hardness = DEFAULT_HINGE_HARDNESS;
            jointDesc.limit.low.restitution = 0.5;

#ifdef DEFAULT_SWING_SPRING
            jointDesc.flags |= NX_RJF_SPRING_ENABLED;
            jointDesc.spring.spring = DEFAULT_SWING_SPRING;
            jointDesc.spring.damper = DEFAULT_SWING_DAMPER;
#endif

            jointDesc.projectionDistance = DEFAULT_PROJECTON_DIST;
            jointDesc.projectionMode = DEFAULT_PROJECTION_METHOD;


            jointDesc.jointFlags &= ~NX_JF_COLLISION_ENABLED;
            jointDesc.setGlobalAnchor(nxvGlobalAnchor);
            jointToReturn = nxScene[iScene]->createJoint(jointDesc);
        }

        xdefault:
        {
            NxFixedJointDesc jointDesc;
            jointDesc.actor[0] = parentActor;
            jointDesc.actor[1] = childActor;
            jointDesc.setGlobalAnchor(nxvGlobalAnchor);

            /*
            NxVec3 vGlobalAxis = childActor->getGlobalPosition() - parentActor->getGlobalPosition();
            vGlobalAxis.normalize();
            jointDesc.setGlobalAxis(vGlobalAxis);
            */
            jointDesc.jointFlags &= ~NX_JF_COLLISION_ENABLED;
            jointToReturn = nxScene[iScene]->createJoint(jointDesc);
//            assert( jointToReturn );
        }
    }
    
    
//    assert( jointToReturn );
    return jointToReturn;



}




static void* createNovodexGeometry(RagdollGeometry* rg, NxGroupType group, F32 fDensity, int iSceneNum)
{
    if ( rg->geom == RAG_GEOM_CAPSULE)
        return nwCreateCapsuleActor(rg->mOffsetMat, rg->fRadius, rg->fHeight, group, fDensity, iSceneNum);
    else if ( rg->geom == RAG_GEOM_SPHERE )
        return nwCreateSphereActor(rg->mOffsetMat, rg->fRadius, group, fDensity, iSceneNum );
    else // iGeom == RAG_GEOM_BOX
        return nwCreateBoxActor(rg->mOffsetMat, rg->vSize, group, fDensity, iSceneNum);

}



static bool findChildRelativePositionFromBoneId(GfxNode* parentNode, BoneId boneId, const Mat4 mParentMat, Vec3 vResult)
{
    // Recurse through tree
    GfxNode* childNode = parentNode->child;

    if(boneId == parentNode->anim_id)
    {
        copyVec3(mParentMat[3], vResult);
        return true;
    }

    while (childNode)
    {
        Mat4 mXform;
        mulMat4(mParentMat, childNode->mat, mXform);
        if(findChildRelativePositionFromBoneId(childNode, boneId, mXform, vResult))
            return true;
        childNode = childNode->next;
    }

    return false;
}


static GfxNode* findChildNodeFromBoneID(GfxNode* parentNode, BoneId boneId )
{
    // Recurse through tree
    GfxNode* childNode = parentNode->child;

    if(boneId == parentNode->anim_id)
        return parentNode;

    while (childNode)
    {
        GfxNode* childSearch = findChildNodeFromBoneID(childNode, boneId);
        if (childSearch)
            return childSearch;
        childNode = childNode->next;
    }

    return NULL;
}

static void calcGeomParamsFromBoneID(BoneId boneId, RagdollGeometry* rg, const Vec3 geomScale, GfxNode* node, SeqInst* seq, int iSceneNum)
{
    rg->fHeight = 1.0f * geomScale[1];
    rg->fRadius = 0.3f * geomScale[0];
    copyVec3(onevec3, rg->vSize);
    Vec3 vResult;
    rg->bCCD = false;

    switch(boneId)
    {
        xcase BONEID_HIPS:
            rg->bOffset = false;
            rg->geom = RAG_GEOM_BOX;
            rg->bCCD = true;
            {
                Vec3 vResultA, vResultB;
                bool bFoundA, bFoundB;

                // Width
                bFoundA = findChildRelativePositionFromBoneId(node, BONEID_ULEGR, unitmat, vResultA);
                bFoundB = findChildRelativePositionFromBoneId(node, BONEID_ULEGL, unitmat, vResultB);
                if ( bFoundA && bFoundB )
                {
                    Vec3 vDiff;
                    subVec3(vResultA, vResultB, vDiff);
                    rg->vSize[0] = fabsf(vDiff[0] * geomScale[0]);
                }
                else
                {
                    rg->vSize[0] = 0.5f * geomScale[0];
                }

                // Height
                bFoundA = findChildRelativePositionFromBoneId(node, BONEID_WAIST, unitmat, vResultA);
                if ( bFoundA )
                {
                    rg->vSize[1] = fabsf(vResultA[1] * 0.5f * geomScale[1]);
                }
                else
                {
                    rg->vSize[1] = 0.7f * geomScale[1];
                }
            }

            if ( geomScale[0] > 0)
                rg->vSize[2] = (0.4f * rg->vSize[0] / geomScale[0]) * geomScale[2];

        xcase BONEID_ULEGR:
        case  BONEID_ULEGL:
            if(findChildRelativePositionFromBoneId(node, BoneId(boneId+2), unitmat, vResult)) // lower legs
                rg->fHeight = lengthVec3(vResult) * geomScale[1];
            else
                rg->fHeight = 1.2f * geomScale[1]; // default

        xcase BONEID_LLEGR:
        case  BONEID_LLEGL:
            rg->bCCD = true;
            if(findChildRelativePositionFromBoneId(node, BoneId(boneId+4), unitmat, vResult)) // toes
                rg->fHeight = lengthVec3(vResult) * geomScale[1];
            else
                rg->fHeight = 1.6f * geomScale[1]; // default

        xcase BONEID_CHEST:
            rg->bOffset = false;
            rg->geom = RAG_GEOM_BOX;
            rg->bCCD = true;
            {
                Vec3 vResultA, vResultB;
                bool bFoundA, bFoundB;

                // Width
                bFoundA = findChildRelativePositionFromBoneId(node, BONEID_UARMR, unitmat, vResultA);
                bFoundB = findChildRelativePositionFromBoneId(node, BONEID_UARML, unitmat, vResultB);
                if ( bFoundA && bFoundB )
                {
                    Vec3 vDiff;
                    subVec3(vResultA, vResultB, vDiff);
                    rg->vSize[0] = fabsf(vDiff[0] * 0.5f * geomScale[0]);
                }
                else
                {
                    rg->vSize[0] = 0.5f * geomScale[0];
                }

                // Height
                bFoundA = findChildRelativePositionFromBoneId(seq->gfx_root->child, BONEID_NECK, unitmat, vResultA);
                if ( bFoundA )
                {
                    rg->vSize[1] = fabsf(vResultA[1] * 0.5f * geomScale[1]);
                }
                else
                {
                    rg->vSize[1] = 0.7f * geomScale[1];
                }
            }

            if ( geomScale[0] > 0)
                rg->vSize[2] = (0.4f * rg->vSize[0] / geomScale[0]) * geomScale[2];

        xcase BONEID_UARMR:
        case  BONEID_UARML:
            if(boneId == BONEID_UARMR)
                rollMat3(RAD(-90.0f), rg->mOffsetMat);
            else
                rollMat3(RAD(90.0f), rg->mOffsetMat);

            rg->fRadius = 0.3f * geomScale[2];
            if(findChildRelativePositionFromBoneId(node, BoneId(boneId+2), unitmat, vResult)) // lower arm
                rg->fHeight = lengthVec3(vResult) * geomScale[1];
            else
                rg->fHeight = 0.9f * geomScale[0];

        xcase BONEID_LARMR:
        case  BONEID_LARML:
            rg->bCCD = true;
            if(boneId == BONEID_LARMR)
                rollMat3(RAD(-90.0f), rg->mOffsetMat);
            else
                rollMat3(RAD(90.0f), rg->mOffsetMat);

            rg->fRadius = 0.3f * geomScale[2];
            if(findChildRelativePositionFromBoneId(node, BoneId(boneId+4), unitmat, vResult)) // fingers
                rg->fHeight = lengthVec3(vResult) * geomScale[1];
            else
                rg->fHeight = 0.6f * geomScale[0];

        xcase BONEID_HEAD:
            rg->fHeight = 0.5f * geomScale[1];
            rg->bOffset = false;

        xcase BONEID_WAIST:

/*    Snakeman
    case 89:        //Fore_FootL
        uiSubBoneId = 92;
        bSnakeTail = true;
        break;
    case 92:        //Fore_LlegL
        uiSubBoneId = 87;
        bSnakeTail = true;
        break;
    case 87:        //Fore_ULegL
        uiSubBoneId = 79;
        bSnakeTail = true;
        break;
    case 79:        //Hind_ULegL
        uiSubBoneId = 80;
        bSnakeTail = true;
        break;
    case 80:        //Hind_LlegL
        uiSubBoneId = 81;
        bSnakeTail = true;
        break;
    case 81:        //Hind_FootL
        uiSubBoneId = 82;
        bSnakeTail = true;
        break;
    case 82:        //Hind_ToeL
        uiSubBoneId = 91;
        bSnakeTail = true;
        break;
    case 91:        //Hind_ULegR
        uiSubBoneId = 85;
        bSnakeTail = true;
        break;
        */

    xdefault:
        ;
    }
}



void* createRagdollGeometry(Mat4 mWorldSpaceMat, BoneId boneId, Vec3 geomScale, GfxNode * node, SeqInst* seq, int iSceneNum)
{
    RagdollGeometry rg;
    rg.geom = RAG_GEOM_CAPSULE;
    rg.bOffset = true;
    Vec3 vOffset;

    copyMat4(mWorldSpaceMat, rg.mOffsetMat);


    /*
    if ( bSnakeTail )
    {
        GfxNode* pChildNode = findChildNodeFromBoneID(node, uiSubBoneId);
        if ( pChildNode )
        {
            Vec3 vDiff;
            subVec3(pChildNode->mat[3], node->mat[3], vDiff);
            fHeight = lengthVec3(vDiff) * geomScale[1] * 2.0f;
        }
        else
        {
            fHeight = 0.3f * geomScale[1]; // default
        }
    }
    */

    calcGeomParamsFromBoneID(boneId, &rg, geomScale, node, seq, iSceneNum );


    if ( rg.bOffset )
    {
        scaleVec3(rg.mOffsetMat[1],rg.fHeight * 0.5f, vOffset);
        subVec3(rg.mOffsetMat[3], vOffset, rg.mOffsetMat[3]);
    }

    F32 fDensity = 1.f;

    void* actor = createNovodexGeometry(&rg, nxGroupRagdoll, fDensity, iSceneNum);

    if ( actor )
    {
        nwSetActorSleepVelocities(actor, ragdollConstants.angularSleepVelocity, ragdollConstants.linearSleepVelocity);
        nwSetActorDamping(actor, ragdollConstants.angularDamping, ragdollConstants.linearDamping);
        nwSetActorMass(actor, fDensity*geomScale[0]*geomScale[1]*geomScale[2]);

        
        if ( rg.bCCD && ragdollConstants.doCCD )
        {
            nwAddCCDSkeletonForActor(actor);
        }
        


    }

    return actor;
}

#ifdef CLIENT
static void drawRagdollBone( Mat4 mWorldSpaceMat, BoneId boneId, const Vec3 geomScale, GfxNode * node, SeqInst* seq )
{
    RagdollGeometry rg;
    Vec3 vOffset;

    rg.geom = RAG_GEOM_CAPSULE;
    rg.bOffset = true;
    copyMat4(mWorldSpaceMat, rg.mOffsetMat);
    calcGeomParamsFromBoneID(boneId, &rg, geomScale, node, seq, NX_CLIENT_SCENE);

    if(boneId == BONEID_UARMR)
    {
        int asijck = 0;
    }

    if ( rg.bOffset )
    {
        scaleVec3(rg.mOffsetMat[1],rg.fHeight * 1.0f, vOffset);
        subVec3(rg.mOffsetMat[3], vOffset, rg.mOffsetMat[3]);
    }

    Vec3 vStart, vEnd, vEndXformed;

    copyVec3( rg.mOffsetMat[3], vStart);

    zeroVec3(vEnd);
    vEnd[1] = rg.fHeight;

    mulVecMat3(vEnd, rg.mOffsetMat, vEndXformed);

    addVec3(vStart, vEndXformed, vEnd);

    //if(uiBoneId == BONEID_ULEGR)
        drawLine3D(vStart, vEnd, 0xffffffff);
}

void drawRagdollSkeleton( GfxNode* rootNode, Mat4 parentMat, SeqInst* seq  )
{
    GfxNode * node; 

    for(node = rootNode ; node ; node = node->next)       
    {
        Mat4 mChildMat;
        Vec3 vDiff;
        if ( seq->handle != node->seqHandle )
            continue;
        mulMat4(parentMat, node->mat, mChildMat);

        //if(node->anim_id == BONEID_LLEGR)
        drawLine3D(parentMat[3], mChildMat[3], 0x790000ff);
        subVec3(parentMat[3], mChildMat[3], vDiff);

        if (!isRagdollBone(node->anim_id))
        {
            if ( node->child )
                drawRagdollSkeleton(node->child, mChildMat, seq );
            continue;
        }

        drawRagdollBone(mChildMat, node->anim_id, onevec3, node, seq);

        drawRagdollSkeleton(node->child, mChildMat, seq );
    }
}
#endif


static Ragdoll* pLastHips = NULL;

int walkSkeletonAndCreateRagdoll( GfxNode* rootNode, const Mat4 parentMat, Ragdoll* parentRagdoll, SeqInst* seq, Ragdoll** rootRagdoll, Vec3 vInitialVel, bool bCreateNovodexPresence, Vec3 vScale, int iSceneNum )
{    
    GfxNode * node; 
    int iBoneCount = 0; // we are a bone, necessarily


    for(node = rootNode ; node ; node = node->next)       
    {
        // Only deal with nodes attached to this seqinst
        if ( seq->handle != node->seqHandle )
            continue;
        if (!isRagdollBone(node->anim_id))
        {
            // This bone is excluded, but it might have included children
            if ( node->child )
            {
                Mat4 mWorldSpaceMat;
                Mat4 mNodeMat;

                copyMat4(node->mat, mNodeMat);
                mulVecVec3(mNodeMat[3], vScale, mNodeMat[3]);
                mulMat4(parentMat, mNodeMat, mWorldSpaceMat);
                iBoneCount += walkSkeletonAndCreateRagdoll(node->child, mWorldSpaceMat, parentRagdoll, seq, NULL, vInitialVel, bCreateNovodexPresence, vScale, iSceneNum);
            }
            continue;
        }

        // One last test, we must know that there is geometry for this before it can be a ragdoll bone
        /*
        char        buf[100];
        sprintf( buf, "GEO_%s", getBoneNameFromNumber( node->anim_id ) );
        char* cFileName = "player_library/G_steve_blockman.anm";
        Model* pModel = modelFind( buf, cFileName, LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE);
        if (!pModel)
            continue;
        */

        // Ok, this node counts as a ragdoll bone
        ++iBoneCount;

        // Allocate it
        Ragdoll* newRagdoll = (Ragdoll*)calloc(1, sizeof(Ragdoll));

        // Set defaults
        newRagdoll->boneId = node->anim_id;
        newRagdoll->iSceneNum = iSceneNum;
        newRagdoll->child = NULL;
        newRagdoll->next = NULL;

        // Snakeman hack!
        /*
        if ( node->anim_id == 89 )
        {
            parentRagdoll = pLastHips;
        }
        */
        newRagdoll->parent = parentRagdoll;
        newRagdoll->hist_latest = 0;
        memset(newRagdoll->absTimeHist, 0, sizeof(newRagdoll->absTimeHist));
        if ( node->anim_id == 0)
            pLastHips = newRagdoll;

        // Calculate quaternion from initial node orientation
        mat3ToQuat(node->mat, newRagdoll->qCurrentRot);
        quatNormalize(newRagdoll->qCurrentRot);

//        copyVec3(node->mat[3], newRagdoll->vCurrentPos);

        // Calculate world space position, and create actor there
        Mat4 mWorldSpaceMat;
        Mat4 mNodeMat;

        copyMat4(node->mat, mNodeMat);
        mulVecVec3(mNodeMat[3], vScale, mNodeMat[3]);
        mulMat4(parentMat, mNodeMat, mWorldSpaceMat);

        //copyMat3(unitmat, node->mat);

        if (bCreateNovodexPresence)
        {
//            Vec3 vNewInitialVel;
            newRagdoll->pActor = createRagdollGeometry(mWorldSpaceMat, newRagdoll->boneId, vScale, node, seq, iSceneNum);

            assert( newRagdoll->pActor );

            if ( newRagdoll->pActor )
            {
                F32 omega, rho;
                Vec3 vRandAxis;
                Vec3 vResult;
                getRandomPointOnSphereSurface(&omega, &rho );
                sphericalCoordsToVec3(vRandAxis, omega, rho, 1.0f);

                if ( lengthVec3Squared(vInitialVel) > 0.1f )
                {
                    scaleVec3(vRandAxis, lengthVec3(vInitialVel) * 0.6f, vRandAxis );
                }
                else
                {
                    scaleVec3(vRandAxis, 0.5f, vRandAxis);
                }

                addVec3(vRandAxis, vInitialVel, vResult);

                nwSetActorLinearVelocity(newRagdoll->pActor, vResult );
            }



            // Create novodex actor and joint for this ragdoll bone

            /*
            int iVert=0;
            Vec3*    verts = (Vec3*)_alloca(pModel->vert_count * sizeof(*verts));
            geoUnpackDeltas(&pModel->pack.verts,    verts,    3,    pModel->vert_count,    PACK_F32);

            newRagdoll->pActor = nwCreateConvexActor(pModel->name, mWorldSpaceMat, (float*)verts, pModel->vert_count, nxGroupDebris, NULL, 1.0f );
            */
            if ( parentRagdoll && parentRagdoll->pActor )
                newRagdoll->pJointToParent = jointRagdoll( (NxActor*)parentRagdoll->pActor, (NxActor*)newRagdoll->pActor, newRagdoll->boneId, mWorldSpaceMat[3], iSceneNum );
            else
                newRagdoll->pJointToParent = NULL;
        }
        else
        {

            newRagdoll->pActor = newRagdoll->pJointToParent = NULL;
        }

        // Ok, now we have our new ragdoll, insert it into our ragdoll tree
        // assuming we're not the root
        if ( parentRagdoll )
        {
            if ( !parentRagdoll->child )
                parentRagdoll->child = newRagdoll;
            else
            {
                Ragdoll* spotForNewRagdoll = parentRagdoll->child;
                while (spotForNewRagdoll->next)
                    spotForNewRagdoll = spotForNewRagdoll->next;
                spotForNewRagdoll->next = newRagdoll;
            }
        }
        else
        {
            assert(rootRagdoll);
            *rootRagdoll = newRagdoll;
        }

        // Now, continue with our child, if it exists
        if ( node->child )
            iBoneCount += walkSkeletonAndCreateRagdoll(node->child, mWorldSpaceMat, newRagdoll, seq, NULL, vInitialVel, bCreateNovodexPresence, vScale, iSceneNum);
    }

    if ( parentRagdoll )
        parentRagdoll->numBones = iBoneCount+1;

    return iBoneCount;

}


// -------------------------------------------------------------------------------------------------------------------

static void nwFreeRagdoll(Ragdoll* parentRagdoll, int iScene)
{
    if (!parentRagdoll)
        return;

    Ragdoll* pChildRagdoll = parentRagdoll->child;
    while (pChildRagdoll != NULL )
    {
        nwFreeRagdoll( pChildRagdoll, iScene);
        Ragdoll* pNextRagdoll = pChildRagdoll->next;
        free( pChildRagdoll );

        pChildRagdoll = pNextRagdoll;
    }


    if ( nwEnabled() )
    {
        if (parentRagdoll->pJointToParent)
        {
            assert( iScene >= 0 && iScene < MAX_NX_SCENES);
            if ( nxScene[iScene] )
                nxScene[iScene]->releaseJoint( *((NxJoint*)parentRagdoll->pJointToParent) );
            parentRagdoll->pJointToParent = NULL;
        }
        if (parentRagdoll->pActor)
        {
            assert( iScene >= 0 && iScene < MAX_NX_SCENES);
            //nwDeleteActor(parentRagdoll->pActor);
            if ( nxScene[iScene] )
            {
                NxActor* actor = (NxActor*)parentRagdoll->pActor;
                nwRemoveCCDSkeletonForActor(actor);
                nxScene[iScene]->releaseActor( *actor );
            }
            nx_state.dynamicActorCount--;
            parentRagdoll->pActor = NULL;
        }
    }
    else
    {
        parentRagdoll->pJointToParent = NULL;
        parentRagdoll->pActor = NULL;
    }
    parentRagdoll->iSceneNum = -1; // we've removed it from the scene

}


static void nwDeleteRagdollImmediately(Ragdoll* parentRagdoll);


// QUEUED STUFF
#ifdef SERVER
#if !BEACONIZER
void nwCreateRagdollQueues()
{
    if ( ragdollCreationQueue )
    {
        assert( ragdollDeletionWithFreeQueue );
        return;
    }

    const int iMaxQueueSize = 128;
    ragdollCreationQueue = createQueue();
    ragdollDeletionWithFreeQueue = createQueue();

    qSetMaxSizeLimit( ragdollCreationQueue, iMaxQueueSize );
    qSetMaxSizeLimit( ragdollDeletionWithFreeQueue, iMaxQueueSize );

    initQueue( ragdollCreationQueue, 16 );
    initQueue( ragdollDeletionWithFreeQueue, 16 );
}
// -------------------------------------------------------------------------------------------------------------------
void nwDeleteRagdollQueues()
{
    // flush deletion queues
    processRagdollDeletionQueues();
    assert( qGetSize(ragdollDeletionWithFreeQueue) == 0);

    // ignore creation queues, just destroy the queue
    //nwProcessRagdollQueue();

    destroyQueue(ragdollCreationQueue);
    destroyQueue(ragdollDeletionWithFreeQueue);

    ragdollCreationQueue = NULL;
    ragdollDeletionWithFreeQueue = NULL;
}
// -------------------------------------------------------------------------------------------------------------------
void processRagdollDeletionQueues()
{
    assert( ragdollDeletionWithFreeQueue );
    Ragdoll* pRagdoll;

    pRagdoll = (Ragdoll*)qDequeue(ragdollDeletionWithFreeQueue);
    while ( pRagdoll )
    {
        nwDeleteRagdollImmediately(pRagdoll);
        pRagdoll = (Ragdoll*)qDequeue(ragdollDeletionWithFreeQueue);
    }
}
#endif
#endif
// -------------------------------------------------------------------------------------------------------------------
static void nwDeleteRagdollImmediately(Ragdoll* parentRagdoll)
{
    if (!parentRagdoll)
        return;

    int iScene = parentRagdoll->iSceneNum;

    if ( iScene >= 0 )
    {
        nwEndThread(iScene);
    }

    nwFreeRagdoll(parentRagdoll, iScene );


    if ( nwEnabled() && iScene >= 0 )
    {
        removeInfluenceSphere(iScene);
        nwDeleteScene(iScene);
    }

    free(parentRagdoll);
}

// -------------------------------------------------------------------------------------------------------------------
void nwDeleteRagdoll(Ragdoll* parentRagdoll)
{
    if (!parentRagdoll)
        return;
#ifdef CLIENT
    nwDeleteRagdollImmediately(parentRagdoll);
    return;
#elif SERVER
    if ( nwEnabled() )
    {
        assert( ragdollDeletionWithFreeQueue );
        int iSuccess;
        if(qIsFull(ragdollDeletionWithFreeQueue))
            qSetMaxSizeLimit(ragdollDeletionWithFreeQueue, 2*qGetMaxSize(ragdollDeletionWithFreeQueue));
        iSuccess = qEnqueue( ragdollDeletionWithFreeQueue, (void*) parentRagdoll );
        assert( iSuccess );
    }
    else
    {
        nwDeleteRagdollImmediately(parentRagdoll);
    }
#endif

}
// -------------------------------------------------------------------------------------------------------------------

#ifdef CLIENT
Ragdoll* nwCreateRagdollNoPhysics( Entity* e )
{

    assert(e);


    if ( !e || !e->seq || !e->seq->gfx_root || !e->seq->gfx_root->child)
        return NULL;


    PERFINFO_AUTO_START("nwCreateRagdollNoPhysics", 1);

    int iRagdollScene = -1;


    GfxNode* rootNode = e->seq->gfx_root->child;

    //printf("Animation = %s\n", e->seq->animation.move->name);
    //printf("RootNode up = %.2f\n", rootNode->mat[2][1]);

    Ragdoll* rootRagdoll = NULL;

    Vec3 vInitialVel;
    copyVec3(e->motion->vel, vInitialVel);

#ifdef RAGDOLL
    // Calculate first frame ragdoll offset, since we switch from mat corresponding
    // from root to hips
    {
        Vec3 vOffsetPos;
        Vec3 vCombatBias;
        zeroVec3(vCombatBias);
        vCombatBias[2] = -0.8f;
        addVec3(vCombatBias, e->seq->gfx_root->child->mat[3], vOffsetPos);
        mulVecMat3(vOffsetPos, ENTMAT(e), e->ragdoll_offset_pos);
        //mat3ToQuat(e->seq->gfx_root->child->mat, e->ragdoll_offset_qrot);
    }
#endif

    Vec3 vScale;
    F32 fCostumeScale = e->costume ? (100.0f + e->costume->appearance.fScale) * 0.01f : 1.0f;
    scaleVec3(e->seq->currgeomscale, fCostumeScale, vScale);

    walkSkeletonAndCreateRagdoll( rootNode, ENTMAT(e), NULL, e->seq, &rootRagdoll, vInitialVel, false, vScale, iRagdollScene);


    //rootRagdoll->numBones++;
    //assert (rootRagdoll->next == NULL);

    PERFINFO_AUTO_STOP();
    return rootRagdoll;
}
#endif
// -------------------------------------------------------------------------------------------------------------------
static void initRagdollConstants()
{
    ragdollConstants.swing_spring = 0.05f; // STO pre-init defaults (previously 0.2f)
    ragdollConstants.swing_damper = 0.025f; // STO pre-init defaults (previously 0.1f)
    ragdollConstants.twist_spring = 0.2f;
    ragdollConstants.twist_damper = 0.1f;
    ragdollConstants.swing_hardness = 0.9f;
    ragdollConstants.twist_hardness = 0.9f;
    ragdollConstants.hinge_hardness = 0.9f;
    ragdollConstants.projection_distance = 0.15f;
    ragdollConstants.linearDamping = 0.15f;
    ragdollConstants.angularDamping = 0.20f;
    ragdollConstants.linearSleepVelocity = 0.35f;
    ragdollConstants.angularSleepVelocity = 0.34f;
    ragdollConstants.doCCD = true;

    ragdollConstants.bInited = true;

}
#ifdef SERVER
void nwPushRagdoll( Entity* e )
{
    assert(e && ragdollCreationQueue );
    if ( !ragdollConstants.bInited )
    {
        initRagdollConstants();
    }


    if (qFind(ragdollCreationQueue, (void*)e ))
        return; // already enqueued


    int iSuccess = qEnqueue( ragdollCreationQueue, (void*) e );
    // ARM NOTE:  This devassert seems to be causing false positive crashes internally.
    // I used I Win Button to kill 720 dudes at the same instant, and the server stood through it.
    // Feel free to re-add it if you know that I did something wrong here.
    //devassert( iSuccess );
}
// -------------------------------------------------------------------------------------------------------------------
static void nwCreateDelayedRagdoll( Entity* e )
{

    assert(e);


    if ( !nwEnabled())
        return;

    e->firstRagdollFrame = 1;

    if ( !e || !e->seq || !e->seq->gfx_root || !e->seq->gfx_root->child)
        return;


    PERFINFO_AUTO_START("nwCreateRagdoll", 1);

    int iRagdollScene = nwCreateScene();
    if ( iRagdollScene < 0 )
    {
        PERFINFO_AUTO_STOP();

        return;
    }
    addInfluenceSphere(iRagdollScene, e);

    if ( !ragdollConstants.bInited )
    {
        initRagdollConstants();
    }

    GfxNode* rootNode = e->seq->gfx_root->child;

    Ragdoll* rootRagdoll = NULL;

    Vec3 vInitialVel;
    copyVec3(e->motion->vel, vInitialVel);


    Vec3 vScale;
    F32 fCostumeScale = (100.0f + e->costume->appearance.fScale) * 0.01f;
    scaleVec3(e->seq->currgeomscale, fCostumeScale, vScale);

    walkSkeletonAndCreateRagdoll( rootNode, ENTMAT(e), NULL, e->seq, &rootRagdoll, vInitialVel, true, vScale, iRagdollScene);

    //rootRagdoll->numBones++;
    //assert (rootRagdoll->next == NULL);

    PERFINFO_AUTO_STOP();
    e->ragdoll = rootRagdoll;
}

// -------------------------------------------------------------------------------------------------------------------

void nwProcessRagdollQueue( )
{
    Entity* e;
    assert( ragdollCreationQueue );

    e = (Entity*)qDequeue(ragdollCreationQueue);
    while ( e )
    {
        nwCreateDelayedRagdoll(e);
        e = (Entity*)qDequeue(ragdollCreationQueue);
    }
}
#endif SERVER

// -------------------------------------------------------------------------------------------------------------------

void nwSetRagdollFromQuaternions( Ragdoll* ragdoll, Quat qRoot )
{    
    Ragdoll* pRagdoll;
    for( pRagdoll = ragdoll; pRagdoll; pRagdoll = pRagdoll->next )
    {
        // Set quat from actor
        NxActor* pActor = (NxActor*)pRagdoll->pActor;
        if ( pActor )
        {
            NxQuat  nxqWorldQuat = pActor->getGlobalOrientationQuat();
            Quat qWorldQuat;
            copyQuat((float*)&nxqWorldQuat, qWorldQuat);

            Quat qLocal;
            Quat qInverseRoot;
            float fFixAngle = 0.0f;
        

            //Quat qActorSpaceQuat;

            quatInverse(qRoot, qInverseRoot);






            switch (pRagdoll->boneId)
            {
            case 7:
            case 9:
                fFixAngle = RAD(90.0f);
                break;
            case 8:
            case 10:
                fFixAngle = RAD(-90.0f);
                break;
            default:
                break;
            };

            if ( fFixAngle != 0.0f)
            {
                Mat3 mFix;
                quatToMat(qWorldQuat, mFix);
                rollMat3World(fFixAngle, mFix);
                mat3ToQuat(mFix, qWorldQuat);
                //assert(validateMat3(mFix));

            }

            // Snakeman hack!
            /*
            if ( pRagdoll->boneId == 89)
            {
                unitquat(qInverseRoot);
            }
            */


            quatMultiply(qInverseRoot, qWorldQuat, qLocal);


            quatInverse(qLocal,qLocal);

            copyQuat(qLocal, pRagdoll->qCurrentRot);

            //assert(quatIsValid(qWorldQuat));

            nwSetRagdollFromQuaternions(pRagdoll->child, qWorldQuat);
        }
        else
        {
            // we're not updating this quat, so multiply it through
            Quat qNewRoot;
            if (0 && quatIsValid(pRagdoll->qCurrentRot))
                quatMultiply(qRoot, pRagdoll->qCurrentRot, qNewRoot);
            else
                copyQuat(qRoot, qNewRoot);

            //assert(quatIsValid(qNewRoot));

            nwSetRagdollFromQuaternions(pRagdoll->child, qNewRoot);

        }
    }
    
}
#endif
