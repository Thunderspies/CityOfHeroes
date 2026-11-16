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

End	
########  Object   #################

Condition
	On	Time
	Time	0
	Repeat	6
	RepeatJitter	2


	Event
		PhysicsOnly
		Ename	Object
		Type	Create
		Inherit	All
		At	Base

		Geom	Bone_SL_01
		Geom	Bone_SL_02
		Geom	Bone_SL_05
		Geom	Bone_SL_05
		##Geom	Bone_SL_06
		Geom	Bone_SL_07
		Geom	Bone_SL_09
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			physJointCollidesWorld

			PositionOffset		0 .5 0
			StartJitter		2 0 2
			Scale			1 1 1
			
			pyrRotateJitter		180 180 180

			Physics			1
			PhysGravity		1
			PhysRestitution		.2
			PhysSFriction		.4
			PhysDFriction		.5
			PhysScale		1.25 1.25 1.25
			PhysDensity		2

			#PhysDebris		3

			Drag			0.005
			PhysJointDrag		0.02
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX RotateY RotateZ TranslateX TranslateY TranslateZ
			PhysJointAngLimit	90
			PhysJointLinLimit	10
			#PhysJointAngSpring	25
			#PhysJointAngSpringDamp	5
			PhysJointBreakForce	1
			PhysJointBreakTorque	1
		End

		#JEvent	:IronRodGateJointBreak.fx
	End


End

End