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
		Part	:EarthDust.part
		Part	:EarthDust2.part
#		Sound	stonehit2 100 30 .85
		Lifespan	10

	End


End

Condition
	On 	Time
	Time 	0
	Repeat	1

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

		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part

		Lifespan	75
		LifespanJitter	15
		
		CThresh 	.01

	End

End


Condition
	On 	Time
	Time 	0
	Repeat	1 #5

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

		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part

		Lifespan	75
		LifespanJitter	15
		
		CThresh 	.01

	End


End


#Condition
#	On 	Time
#	Time 	0
#	Repeat	10
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
#		Part	:RockDebrisSandStone.part
#		Part	:RockDebrisSandStone2.part
#
#		Lifespan	75
#		LifespanJitter	15
#		
#		CThresh 	.01
#
#	End
#
#End

End
