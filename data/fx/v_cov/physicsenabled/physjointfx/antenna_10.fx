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
		
		Geom	Antenna_10
		
	End

	Event
		Ename	AntennaTop1
		Type	create
		Inherit	Position Rotation Scale
		At	AntennaHookup
		altpiv	2
		
		Geom	Antenna_10_Fan01
		
		BhvrOverride
			Physics			1
#			PhysGravity		2.12
			InitialVelocityJitter	0.15 0.15 0.15
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			PhysDebris		3

#			Gravity			.048
			Drag			1.0
			PhysJointDrag		0.75

			PhysJointDrag		.9
			PhysJointAngSpring	300
			PhysJointAngSpringDamp	100
			PhysJointDOFs		RotateX RotateY RotateZ
			PhysJointAngLimit	50

			PhysJointAngSpring	200		
			PhysJointAnchor		0 0 0

			PhysDensity		2
		
		End
	
	End

	Event
		Ename	AntennaTop2
		Type	create
		Inherit	Position Rotation Scale
		At	AntennaHookup
		altpiv	2
		
		Geom	Antenna_10_Fan02
		
		BhvrOverride
			Physics			1
#			PhysGravity		2.12
			InitialVelocityJitter	0.15 0.15 0.15
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			PhysDebris		3

#			Gravity			.048
			Drag			1.0
			PhysJointDrag		0.75

			PhysJointDrag		.9
			PhysJointAngSpring	300
			PhysJointAngSpringDamp	100
			PhysJointDOFs		RotateX RotateY RotateZ
			PhysJointAngLimit	50

			PhysJointAngSpring	200		
			PhysJointAnchor		0 0 0

			PhysDensity		2
		End
	
	End

	Event
		Ename	AntennaTop3
		Type	create
		Inherit	Position Rotation Scale
		At	AntennaHookup
		altpiv	2
		
		Geom	Antenna_10_Fan03
		
		BhvrOverride
			Physics			1
#			PhysGravity		2.12
			InitialVelocityJitter	0.15 0.15 0.15
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			PhysDebris		3

#			Gravity			.048
			Drag			1.0
			PhysJointDrag		0.75

			PhysJointDrag		.9
			PhysJointAngSpring	300
			PhysJointAngSpringDamp	100
			PhysJointDOFs		RotateX RotateY RotateZ
			PhysJointAngLimit	50

			PhysJointAngSpring	200		
			PhysJointAnchor		0 0 0

			PhysDensity		2
		End
		
	End

##################################	TOP	#####################################

	Event
		Ename	AntennaTopFan
		Type	create
		Inherit	Position Rotation Scale
		At	AntennaHookup
		altpiv	1
		
		Geom	Antenna_10_TopFan
		
		BhvrOverride
			Physics			1
#			PhysGravity		2.12
			InitialVelocityJitter	0.15 0.15 0.15
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			PhysDebris		3

#			Gravity			.048
			Drag			1.0
			PhysJointDrag		0.75

			PhysJointDrag		.9
			PhysJointAngSpring	300
			PhysJointAngSpringDamp	100
			PhysJointDOFs		RotateY
			PhysJointAngLimit	180

			PhysJointAngSpring	200		
			PhysJointAnchor		0 0 0

			PhysDensity		2
		End
		
	End
End