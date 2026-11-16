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
		Type	create
		Inherit	Position Rotation Scale
		At	origin
		
		Geom	Antenna_04_Top
		
		BhvrOverride
			PositionOffset		-3.3 -.3 1.2
			physJointCollidesWorld
			Physics			1
			PhysGravity		2.12
			InitialVelocityJitter	0.15 0.15 0.15
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5

			Gravity			.048

			PhysDensity		2
			Physdebris		3
		End
		
	End
	
	
End