#########################################################
##	PhysicsRing
##
FxInfo  

LifeSpan 220

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Part	:EarthDust.part
		Part	:EarthDust2.part
#		Sound	stonehit2 100 30 .85
		Lifespan	15

	End


End

Condition
	On 	Time
	Time 	0
	Repeat	3

	Event
		Type	start
		At	origin

		Bhvr	:RockPieces.bhvr
		BhvrOverride
			Gravity			.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			StartColor		255 180 220

		End

		Part	:RockDebrisSandStoneLarge2.part

		Lifespan	75
		LifespanJitter	15
		
		CThresh 	.01

	End

	
	Event
		hardwareOnly
		Type	start
		At	origin

		Bhvr	:RockPieces.bhvr
		BhvrOverride
			Gravity			.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			StartColor		255 180 220

		End

		Part	:RockDebrisSandStoneLarge2.part

		Lifespan	75
		LifespanJitter	15
		
		CThresh 	.01

	End

#		
#	Event
#		hardwareOnly
#		Type	start
#		At	origin
#
#		Bhvr	:RockPieces.bhvr
#		BhvrOverride
#			Gravity			.05
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.99
#			physDFriction 	 	0.9
#			StartColor		255 180 220
#
#			InitialVelocityJitter	1 1 1
#
#		End
#
#		Part	:RockDebrisSandStoneLarge2.part
#
#		Lifespan	75
#		LifespanJitter	15
#		
#		CThresh 	.01
#
#	End

End

Condition
	On 	Time
	Time 	0

	Event
		SoftwareOnly
		Type	start
		At	origin

		Bhvr	:RockPieces.bhvr
		BhvrOverride
		
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.7 0.5 0.7

			Gravity			.05
			physGravity		1.4
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			
			Scale			.3 .3 .3
			StartColor		255 180 220

		End

		Geom	_cvsmhlls_FxRockBreak_med_01


		Lifespan	1200
		LifespanJitter	150
		
		CEvent 	V_COV\PhysicsEnabled\RockDebrisSandstoneBreak.fx
		CThresh .9

	End

	Event
		SoftwareOnly
		Type	start
		At	origin

		Bhvr	:RockPieces.bhvr
		BhvrOverride
		
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.7 0.5 0.7

			Gravity			.05
			physGravity		1.4
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			
			Scale			.3 .3 .3
			StartColor		255 180 220
		End

		Geom	_cvsmhlls_FxRockBreak_med_02

		Lifespan	1200
		LifespanJitter	150
		
		CEvent 	V_COV\PhysicsEnabled\RockDebrisSandstoneBreak.fx
		CThresh .9

	End

	
	Event
		HardwareOnly
		Type	start
		At	origin

		Bhvr	:RockPieces.bhvr
		BhvrOverride
		
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.7 0.5 0.7

			Gravity			.05
			physGravity		1.4
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			
			Scale			.3 .3 .3
			StartColor		255 180 220
			
			PhysDebris		2
			PhysDensity		3

		End

		Geom	_cvsmhlls_FxRockBreak_med_01


		Lifespan	30
		LifespanJitter	15
		
		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSandstoneBreak.fx
		CThresh .9

	End

	Event
		HardwareOnly
		Type	start
		At	origin

		Bhvr	:RockPieces.bhvr
		BhvrOverride
		
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.7 0.5 0.7

			Gravity			.05
			physGravity		1.4
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			
			Scale			.3 .3 .3
			StartColor		255 180 220

			PhysDebris		2
			PhysDensity		3
		End

		Geom	_cvsmhlls_FxRockBreak_med_02

		Lifespan	30
		LifespanJitter	15
		
		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSandstoneBreak.fx
		CThresh .9

	End
End

End
