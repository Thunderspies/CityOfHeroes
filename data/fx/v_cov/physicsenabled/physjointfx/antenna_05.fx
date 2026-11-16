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
		
		Geom	Antenna_05
		
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
		Ename	AntennaTop1
		Type	create
		Inherit	Position Rotation Scale
		At	AntennaHookup
		altpiv	1
		
		Geom	Antenna_05_Right
		
		BhvrOverride
			PositionOffset		0 .5 0
#			physJointCollidesWorld
			Physics			1
			PhysGravity		0.12
			InitialVelocityJitter	0.15 0.15 0.15
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			PhysDebris		3

#			Gravity			.048
			Drag			1.0
			PhysJointDrag		0.75

			PhysJointDrag		.9
			PhysJointAngSpring	5
			PhysJointAngSpringDamp	1
			PhysJointDOFs		RotateX RotateY RotateZ
			PhysJointAngLimit	50

#			PhysJointBreakForce	 100
			PhysJointAngSpring	1000		
			PhysJointAnchor		0 0 0

			PhysDensity		12
#			Physdebris		3
		End
		
	End
	
	Event
		Ename	AntennaTop2
		Type	create
		Inherit	Position Rotation Scale
		At	AntennaHookup
		altpiv	1
		
		Geom	Antenna_05_Left
		
		BhvrOverride
			PositionOffset		0 -.5 -.5
#			physJointCollidesWorld
			Physics			1
			PhysGravity		0.12
			InitialVelocityJitter	0.15 0.15 0.15
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			PhysDebris		3

#			Gravity			.048
			Drag			1.0
			PhysJointDrag		0.75

			PhysJointDrag		.9
			PhysJointAngSpring	5
			PhysJointAngSpringDamp	1
			PhysJointDOFs		RotateX RotateY RotateZ
			PhysJointAngLimit	50

#			PhysJointBreakForce	 100
			PhysJointAngSpring	1000		
			PhysJointAnchor		0 0 0

			PhysDensity		12
#			Physdebris		3
		End
		
	End
	
End