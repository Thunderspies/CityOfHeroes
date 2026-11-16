##############################
###  PHYS-FORCE Template   ###
##############################

FxInfo

#LifeSpan	300

########  Main   #################

Condition
	On	Time
	Time	0
	
	Event
		Ename	Base
		Type	create
		Inherit	Position Rotation
		At	origin
	
		BhvrOverride
			PositionOffset		0 0 0
		End
	End

	
########  Object   #################

	Event
		PhysicsOnly
		Ename	Object
		Type	Create
		Inherit	All
		At	Base

		Geom	FifthClmn_ammoCrate1
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			physJointCollidesWorld

			PositionOffset		0 0 0
			Scale			1 1 1
			
			#pyrRotateJitter		0 180 0

			Physics			1
			PhysGravity		1
			PhysRestitution		.15
			PhysSFriction		.5
			PhysDFriction		.5
			PhysScale		1 1 1

			#PhysDebris		3

			Drag			0.015
			PhysJointDrag		0.015
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX RotateY RotateZ TranslateX TranslateY TranslateZ
			#PhysJointAngLimit	90
			#PhysJointLinLimit	5
			PhysJointAngSpring	100
			PhysJointAngSpringDamp	10
			PhysJointBreakForce	1
			PhysJointBreakTorque	1
		End

#		JEvent	Explosions\CardboardBoxExplosion\CardboardExplosion_LG.Fx
#		JDestroy	1
	End


End

End