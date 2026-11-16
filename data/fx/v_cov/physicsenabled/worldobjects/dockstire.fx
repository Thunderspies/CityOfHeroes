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

		Geom	Barge_A_Tire_Full  ###Invalid shape!!!###
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			physJointCollidesWorld

			PositionOffset		0 0 0
			Scale			1 1 1
			
			pyrRotateJitter		0 180 0

			Physics			1
			PhysGravity		1
			PhysRestitution		.4
			PhysSFriction		.6
			PhysDFriction		.8
			PhysScale		1 1 1
			PhysDensity		2

			#PhysDebris		3

			Drag			0.01
			PhysJointDrag		0.03
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX RotateY RotateZ TranslateX TranslateY TranslateZ
			PhysJointAngLimit	30
			PhysJointLinLimit	10
			PhysJointAngSpring	245
			PhysJointAngSpringDamp	10
			PhysJointBreakForce	1
			PhysJointBreakTorque	1
		End

		#JEvent	:IronRodGateJointBreak.fx
	End


End

End