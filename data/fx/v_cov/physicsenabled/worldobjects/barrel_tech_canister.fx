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

		Geom	Tek1_Canister_sml_02
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			physJointCollidesWorld

			PositionOffset		0 0 0
			Scale			1 1 1
			
			pyrRotateJitter		0 180 0

			Physics			1
			PhysGravity		1
			PhysRestitution		.25
			PhysSFriction		.4
			PhysDFriction		.8
			PhysScale		1 1 1
			PhysDensity		2

			#PhysDebris		3

			Drag			0.01
			PhysJointDrag		0.03
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX RotateY RotateZ TranslateX TranslateY TranslateZ
			PhysJointAngLimit	15
			PhysJointLinLimit	10
			PhysJointAngSpring	200
			PhysJointAngSpringDamp	10
			PhysJointBreakForce	50
			PhysJointBreakTorque	10
		End

		#JEvent	:IronRodGateJointBreak.fx
	End


End

End