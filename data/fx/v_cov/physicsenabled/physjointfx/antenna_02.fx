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
		Ename	AntennaHookup
		Type	create
		Inherit	Position Rotation Scale
		At	origin
		
		Geom	Antenna_02
		
#		BhvrOverride
##			physJointCollidesWorld
#			Physics			1
#			PhysGravity		0.001
#			PhysRestitution		.5
#			PhysSFriction		.5
#			PhysDFriction		.5
#
#			Drag			1.8
#
#			PhysJointDrag		.9
#			PhysJointAngSpringDamp	99999999
#			PhysJointDOFs		RotateX RotateY RotateZ
#			PhysJointAngLimit	110
#
##			PhysJointAngSpring	2000		
#			PhysJointAnchor		0 0 -1
#
#			PhysDensity		2
#		End
		
	End

	Event

		Type	create
		Inherit	Position Rotation Scale
		At	AntennaHookup
		altpiv	1
		
		Geom	Antenna_02_Bottem
		
		BhvrOverride
			PositionOffset		-2 0 .5
			Physics			1
			PhysGravity		0.0001
			PhysRestitution		.5
			PhysSFriction		.5
			PhysDFriction		.5

			Drag			1.0

			PhysJointDrag		1
			PhysJointAngSpring	199
			PhysJointAngSpringDamp	900		
			PhysJointDOFs		RotateX RotateY RotateZ
			PhysJointAngLimit	110


			PhysDensity		2
		End
		
	End
	
	Event

		Type	create
		Inherit	Position Rotation Scale
		At	AntennaHookup
		altpiv	2
		
		Geom	Antenna_02_Top
		
		BhvrOverride
			PositionOffset		-2.1 -2 0.5
#			physJointCollidesWorld
			Physics			1
			PhysGravity		0.0001
			PhysRestitution		.5
			PhysSFriction		.5
			PhysDFriction		.5

			Drag			1.0

			PhysJointDrag		1
			PhysJointDOFs		RotateX RotateY RotateZ
			PhysJointAngLimit	180

			PhysJointAngSpring	199
			PhysJointAngSpringDamp	900		
#			PhysJointAnchor		0 0 -1

			PhysDensity		2
		End
		
	End
	
End