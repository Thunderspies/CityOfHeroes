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
			pyrRotate		0 90 0
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
			PositionOffset		10.5 0 0
			Scale			1 .99 1
		End
	End
		
	Event
		Ename	DoorHinge2
		Type	Local	#Create
		#Inherit	Position Rotation
		At	Base

		#Geom	_cem_gate2_hinge
		
		BhvrOverride
			PositionOffset		-10.5 0 0
			Scale			1 .99 1
			pyrRotate		0 180 0
		End
	End

	
########  Doors   #################

	Event
		PhysicsOnly
		Ename	Door1
		Type	Create
		Inherit	All	#Position Rotation
		At	DoorHinge1

		Geom	ironrod_gate_door
		
		BhvrOverride
			PositionOffset		-.5 .2 .25
			Scale			.98 .98 1
			
			pyrRotate		0 30 0

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			PhysScale		1 1 2

			Drag			0.025
			PhysJointDrag		0.075

			#PhysDebris		2
			
			PhysJointAnchor		.5 0 0
			PhysJointDOFs		RotateY
			PhysJointAngLimit	105
			PhysJointAngSpring	3000
			#PhysJointAngSpringDamp	100
			PhysJointBreakForce	25000
			PhysDensity		2
		End

		JEvent	:IronRodGateJointBreak.fx
		JDestroy 1
	End

	Event
		PhysicsOnly
		Ename	Door2
		Type	Create
		Inherit	All	#Position Rotation
		At	DoorHinge2

		Geom	ironrod_gate_door
		
		BhvrOverride
			PositionOffset		-.5 .2 -.25
			Scale			.98 .98 1
			
			pyrRotate		0 -30 0

			Physics			1
			PhysGravity		1
			PhysRestitution		1.5
			PhysSFriction		.2
			PhysDFriction		.3
			PhysScale		1 1 2

			Drag			0.025
			PhysJointDrag		0.075

			#PhysDebris		2
			
			PhysJointAnchor		.5 0 0
			PhysJointDOFs		RotateY

			PhysJointAngLimit	105
			PhysJointAngSpring	3000
			#PhysJointAngSpringDamp	100
			PhysJointBreakForce	25000
			PhysDensity		2
		End

		JEvent	:IronRodGateJointBreak.fx
		JDestroy 1
	End

End

End