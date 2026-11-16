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
		Part	:EarthDustFaint.part
		Part	:EarthDust2Faint.part
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

End
