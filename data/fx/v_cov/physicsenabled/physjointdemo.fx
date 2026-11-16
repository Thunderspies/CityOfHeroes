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
		Type	start
		At	root  #origin
	
		BhvrOverride
			PositionOffset		0 0 15
		End
	End
	
########  Posts   #################

	Event
		Ename	Post1
		Type	Local
		At	Base

		Geom	ironrod_post_01
		
		BhvrOverride
			PositionOffset		6.35 0 0
			Scale			1 1.5 1
		End
	End
		
	Event
		Ename	Post2	
		Type	Local
		At	Base

		Geom	ironrod_post_01
		
		BhvrOverride
			PositionOffset		-6.35 0 0
			Scale			1 1.5 1
		End
	End
	
	Event
		Ename	DoorHinge1
		Type	Create
		Inherit	Position Rotation
		At	Post1

		Geom	_cem_gate2_hinge
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			.5 .5 .5
		End
	End
		
	Event
		Ename	DoorHinge2
		Type	Create
		Inherit	Position Rotation
		At	Post2

		Geom	_cem_gate2_hinge
		
		BhvrOverride
			PositionOffset		0 0 0
			pyrRotate		0 180 0
			Scale			.5 .5 .5
		End
	End

	
########  Doors   #################
	
	Event
		Ename	Door1
		Type	Create
		Inherit	Position Rotation
		At	Post1

		Geom	cem_gate_door
		
		BhvrOverride
			PositionOffset		0 .2 0
			Scale			.5 .5 .5

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			PhysScale		1 1 2

			Drag			0.025

			PhysDebris		2
			
			PhysJointAnchor		-.5 0 0
			PhysJointDOFs		RotateY
			PhysJointAngLimit	120
			PhysJointAngSpring	250
			PhysJointAngSpringDamp	100
			PhysJointBreakForce	1000
		End

		JEvent	:JointBreakMetalSparks.fx
	End

	Event
		Ename	Door2
		Type	Create
		Inherit	Position Rotation
		At	Post2

		Geom	_cem_gate2_det01
		
		BhvrOverride
			PositionOffset		0 .2 0
			pyrRotate		0 180 0
			Scale			.5 .5 .5

			Physics			1
			PhysGravity		1
			PhysRestitution		1.5
			PhysSFriction		.2
			PhysDFriction		.3
			PhysScale		1 1 2

			Drag			0.025

			PhysDebris		2
			
			PhysJointAnchor		-.5 0 0
			PhysJointDOFs		RotateY

			PhysJointAngLimit	120
			PhysJointAngSpring	250
			PhysJointAngSpringDamp	100
			PhysJointBreakForce	1000
		End

		JEvent	:JointBreakMetalSparks.fx
	End

########  Lines   #################

	Event
		Ename	clothesline
		Type	Create
		Inherit	Position Rotation
		At	Base

		Geom	clothesline_32_01
		
		BhvrOverride
			PositionOffset		0 15 0
			Scale			.4 .4 .4

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3

			Drag			0.01

			#PhysDebris		3
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX
		End
	End

	
	Event
		Ename	clothesline2
		Type	Create
		Inherit	Position Rotation
		At	Base

		Geom	clothesline_32_02
		
		BhvrOverride
			PositionOffset		0 18 0
			Scale			.4 .4 .4

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3

			Drag			0.01

			#PhysDebris		3
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX
		End
	End

	
########  Signs   #################
	
	Event
		Ename	SignFrame1
		Type	Local
		At	Base

		Geom	_sign_hanging_frame
		
		BhvrOverride
			PositionOffset		6.35 14 -2.5
			pyrRotate		0 -90 0
			Scale			.25 .25 .25
		End
	End

	Event
		Ename	sign1
		Type	Create
		Inherit	Position Rotation
		At	SignFrame1

		Geom	_sign_hnging_vert_1_2
		
		BhvrOverride
			PositionOffset		0 -4.1 0
			Scale			.25 .25 .25

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3

			Drag			0.01
			PhysJointDrag		0.025

			#PhysDebris		2
			
			PhysJointAnchor		0 2.75 0
			PhysJointDOFs		RotateX
			PhysJointAngLimit	170
			
			PhysJointBreakForce	750
		End
	End
	
	Event
		Ename	SignFrame2
		Type	Local
		At	Base

		Geom	_sign_hanging_frame
		
		BhvrOverride
			PositionOffset		-6.35 14 -2.5
			pyrRotate		0 -90 0
			Scale			.25 .25 .25
		End
	End

	Event
		Ename	sign2
		Type	Create
		Inherit	Position Rotation
		At	SignFrame2

		Geom	_sign_hnging_1_2
		
		BhvrOverride
			PositionOffset		0 -2 0
			Scale			.25 .25 .25

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3

			Drag			0.01
			PhysJointDrag		0.025

			#PhysDebris		3
			
			PhysJointAnchor		0 1 0
			PhysJointDOFs		RotateX
			PhysJointAngLimit	170
			
			PhysJointBreakForce	750
		End
	End

End


End