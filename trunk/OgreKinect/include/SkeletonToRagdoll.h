
#ifndef __SkeletonToRagdoll_h_
#define __SkeletonToRagdoll_h_

#include "Ogre.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

class SkeletonToRagdoll
{

public:
	SkeletonToRagdoll(Ogre::SceneManager* _mSceneMgr):mSceneMgr(_mSceneMgr){};
	virtual ~SkeletonToRagdoll(void);

	void createRagdoll(btDynamicsWorld* ownerWorld, Ogre::SceneNode* skeletonNode);
	void addIgnoreEventObject(btRigidBody* obj){ignoreEventObjs.push_back(obj);};

	std::string update();
	//std::string update(Ogre::AnimationState* anim);

protected:
	btDynamicsWorld*									m_ownerWorld;	

	std::vector<std::pair<btRigidBody*, Ogre::Bone*>>	btBones;
	std::vector<Ogre::SceneNode*>						debugBone;
	std::vector<btRigidBody*>							ignoreEventObjs;
	std::vector<std::pair<Ogre::Bone*, Ogre::Vector3>>	bonesToReset;
	
	int													steps;

	Ogre::Skeleton*										skeleton;
	Ogre::SceneManager*									mSceneMgr;
	Ogre::Vector3										scale;
	Ogre::SceneNode*									skeletonNode;	
	Ogre::AnimationState*								currAnimation;
	
	btRigidBody* localCreateRigidBody (btScalar mass, const btTransform& startTransform, btCollisionShape* shape);
	void setBone(Ogre::Bone* bone,btRigidBody* parentBone = NULL);
	void resetAfterOffset();
};

#endif