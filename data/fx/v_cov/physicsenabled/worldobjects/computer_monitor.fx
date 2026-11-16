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

		Geom	Computer_Monitor_01
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			physJointCollidesWorld

			PositionOffset		0 0 0
			Scale			1 1 1
			
			pyrRotateJitter		0 0 0

			Physics			1
			PhysGravity		1
			PhysRestitution		.2
			PhysSFriction		.4
			PhysDFriction		.5
			PhysScale		1 1 1
			PhysDensity		2

			#PhysDebris		3

			Drag			0.005
			PhysJointDrag		0.02
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX RotateY RotateZ TranslateX TranslateY TranslateZ
			PhysJointAngLimit	10
			PhysJointLinLimit	10
			PhysJointAngSpring	50
			PhysJointAngSpringDamp	10
			PhysJointBreakForce	10
			PhysJointBreakTorque	10
		End

	End


End

End