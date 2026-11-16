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
		
		Geom	Antenna_03
		
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
		
		Geom	Antenna_03_Bottem
		
		BhvrOverride
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
		
		Geom	Antenna_03_Top
		
		BhvrOverride
#			PositionOffset		-1.8 -2 .7
#			physJointCollidesWorld
			Physics			1
			PhysGravity		0.0001
			PhysRestitution		.5
			PhysSFriction		.5
			PhysDFriction		.5

			Drag			1.0

			PhysJointDrag		.9
#			PhysJointAngSpringDamp	99999999
			PhysJointDOFs		RotateY
			PhysJointAngLimit	180

#			PhysJointAngSpring	2000		
#			PhysJointAnchor		0 0 -1

			PhysDensity		2
		End
		
	End
	
End