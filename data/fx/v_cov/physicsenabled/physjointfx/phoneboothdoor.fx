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
			pyrRotate		0 0 0
		End
	End
	
########  Hinges   #################

	
	Event
		Ename	DoorHinge1
		Type	Local	#Create
		#Inherit	Position Rotation
		At	Base

		#Geom	_cem_gate2_hinge
		
		BhvrOverride
			PositionOffset		.1 0 .1
			Scale			1 1 1
		End
	End

	
########  Doors   #################

	Event
		PhysicsOnly
		Ename	Door1
		Type	Create
		Inherit	All
		At	DoorHinge1

		Geom	_cap_pdr_03
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
			
			pyrRotate		0 165 0
			pyrRotateJitter		0 12.5 0

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			PhysScale		3 1 1

			Drag			0.025
			PhysJointDrag		0.125

			#PhysDebris		3
			PhysDensity		2
			
			PhysJointAnchor		0 0 .1
			PhysJointDOFs		RotateY
			PhysJointAngLimit	75
			PhysJointAngSpring	500
			#PhysJointAngSpringDamp	100
			PhysJointBreakForce	750
		End

		JEvent	:PhoneBoothDoorJointBreak.fx
		JDestroy	1
	End


End

End