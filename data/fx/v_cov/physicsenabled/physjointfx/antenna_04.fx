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
		
		Geom	Antenna_04
		
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
		Ename	AntennaTop
		Type	create
		Inherit	Position Rotation Scale
		At	AntennaHookup
		altpiv	1
		
		Geom	Antenna_04_Top
		
		BhvrOverride
			PositionOffset		-3.3 -.3 1.2
#			physJointCollidesWorld
			Physics			1
#			PhysGravity		2.12
#			InitialVelocityJitter	0.15 0.15 0.15
#			physRestitution 	0.5
#			physSFriction 	 	0.5
#			physDFriction 	 	0.5

#			Gravity			.048
			Drag			1.0
			PhysJointDrag		0.75

			PhysJointDrag		.9
			PhysJointAngSpring	3300
			PhysJointAngSpringDamp	100
			PhysJointDOFs		RotateX RotateY RotateZ
			PhysJointAngLimit	1
			PhysJointBreakForce	99999
			PhysJointAngSpring	200		
			PhysJointAnchor		0 0 0
			PhysDensity		2
		End
		
		JEvent	:Antenna_04_Break.fx
		JDestroy	1
	End
	
	
End