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
		Type	Local
		At	origin
	
		BhvrOverride
			PositionOffset		0 0 13
			pyrRotate		0 -90 0
		End
	End
	
########  Hinges   #################

	
	Event
		Ename	DoorHinge1
		Type	Local	#Create
		#Inherit	Position Rotation
		At	Base

		Geom	strggate_hinge
		
		BhvrOverride
			PositionOffset		12.5 .5 -.5
			Scale			1 .99 1
		End
	End
		
	Event
		Ename	DoorHinge2
		Type	Local	#Create
		#Inherit	Position Rotation
		At	Base

		Geom	strggate_hinge
		
		BhvrOverride
			PositionOffset		-12.5 .5 0
			Scale			1 .99 1
			pyrRotate		0 180 0
		End
	End

	
########  Doors   #################
	
	Event
		Ename	DoorOffset1
		Type	Local
		At	DoorHinge1

		BhvrOverride
			PositionOffset		-.65 .2 0
		End
	End

	Event
		Ename	Door1
		Type	Create
		Inherit	All	#Position Rotation
		At	DoorOffset1

		Geom	strggate_gate01
		
		BhvrOverride
			#PositionOffset		-.5 .2 0
			Scale			.95 1 1
			pyrRotate		0 45 0

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			PhysScale		1 1 1

			Drag			0.025
			PhysJointDrag		0.2

			#PhysDebris		2
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateY
			PhysJointAngLimit	90
			PhysJointLinLimit	0
			PhysJointAngSpring	200000
			#PhysJointAngSpringDamp	100
			#PhysJointBreakForce	50000
			PhysDensity		3
		End

		JEvent	:CemGateJointBreak.fx
	End
	
	Event
		Ename	DoorOffset2
		Type	Local
		At	DoorHinge2

		BhvrOverride
			PositionOffset		-.65 .2 0
		End
	End

	Event
		Ename	Door2
		Type	Create
		Inherit	All	#Position Rotation
		At	DoorOffset2

		Geom	strggate_gate01
		
		BhvrOverride
			#PositionOffset		.5 .2 0
			Scale			.95 1 1
			pyrRotate		0 -45 0

			Physics			1
			PhysGravity		1
			PhysRestitution		1.5
			PhysSFriction		.2
			PhysDFriction		.3
			PhysScale		1 1 1

			Drag			0.025
			PhysJointDrag		0.2

			#PhysDebris		2
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateY

			PhysJointAngLimit	90
			PhysJointAngSpring	200000
			#PhysJointAngSpringDamp	100
			#PhysJointBreakForce	50000
			PhysDensity		3
		End

		JEvent	:CemGateJointBreak.fx
	End

End

End