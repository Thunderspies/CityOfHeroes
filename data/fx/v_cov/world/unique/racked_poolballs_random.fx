#########################################################
##	template

FxInfo

#########################################################
Condition
	On Time
	Time 3

	Event
		EName	Hookup
		Type 	local
		At	hookup
		
		BhvrOverride
			
			PhysForceType		CCWSwirl
			PhysForceCentripetal	-2
			PhysForceRadius		14
			PhysForcePower		10
			PhysForcePowerJitter	3

		End
		lifespan	1
	End


End

Condition
	On Time
	Time 0

	Event
		EName	Hookup
		Type 	local
		At	root
		
		BhvrOverride
			PositionOffset		0 0 5
		End
	End

	Event
		Type 	Start
		At	Hookup

		BhvrOverride
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		0 0 0
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048
			
		End

		geom  vclub_PoolBall_01

	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		-.2 0 .35
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_02

	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		.2 0 .35
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_14

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		.43 0 .58
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048
		End

		geom  vclub_PoolBall_09		#4

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		0 0 .5
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_08		#5

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		-.43 0 .58
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_06

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		.63 0 .78
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_03

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		.3 0 .8
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_15

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		-.1 0 .84
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_07

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		-.55 0 0.85
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_13

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		.89 0 1.1
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_10

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		.49 0 1.2
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_11

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		.1 0 1.2
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_04

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		-.3 0 1.2
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom  vclub_PoolBall_12

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup
		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		-.7 0 1.2
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048


		End

		geom   vclub_PoolBall_05

		Lifespan	1
	End
	
	Event
		Type 	Start
		At	Hookup

		BhvrOverride
			
			#Spin			3 3 3
			PYRrotateJitter		180 180 180
			PositionOffset		0 0 -2
			PhysScale		1 1 1
			Scale			2 2 2
			Physics			1
			physRestitution 	0.5
			physSFriction 	 	0.6
			physDFriction 	 	0.6
			PhysDebris		1
			physGravity 		1
			Gravity 		.048

		End

		geom   vclub_PoolBall_Cue

		Lifespan	1
	End

End	

End	

	