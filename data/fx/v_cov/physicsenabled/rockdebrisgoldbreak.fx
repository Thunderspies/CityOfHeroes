#########################################################
##	PhysicsRing
##
FxInfo  

LifeSpan 100

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Part	\POWERS\EarthControl\GenericEarthHandsDust.part
		Part	\POWERS\EarthControl\GenericEarthHandsDust2.part
		Sound stonehit2 100 100 .85
		Lifespan	10

	End


End

Condition
	On 	Time
	Time 	0
	Repeat	3

	Event
		Type	start
		At	origin

		Bhvr	:Icecubes.bhvr
		BhvrOverride
			Gravity			.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End

		Part1	:RockDebrisGold.part
		Lifespan	75
		LifespanJitter	15
		
		CThresh 	.00001

	End

	
	Event
		HardwareOnly
		Type	start
		At	origin

		Bhvr	:Icecubes.bhvr
		BhvrOverride
			Gravity			.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End

		Part1	:RockDebrisGold.part
		Lifespan	75
		LifespanJitter	15
		
		CThresh 	.00001

	End

#		
#	Event
#		HardwareOnly
#		Type	start
#		At	origin
#
#		Bhvr	:Icecubes.bhvr
#		BhvrOverride
#			Gravity			.05
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.99
#			physDFriction 	 	0.9
#
#			InitialVelocityJitter	1 1 1
#		End
#
#		Part1	:RockDebrisGold.part
#		Lifespan	75
#		LifespanJitter	15
#		
#		CThresh 	.00001
#
#	End

End

End
