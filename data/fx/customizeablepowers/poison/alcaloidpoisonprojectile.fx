#########################################################
##	Fire_Ball
##

#########################################################

FxInfo
LifeSpan	200

#########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	Spike
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\Poison\YellowGlassScale.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_PoisonGlassBall
		Sound Alkaloid1 80 80 .88
		LifeSpan	23
	End

End

Condition
	On 	Time
	Time 	34

	Event
		EName	Prime
		Type	start
		At	Spike

		Bhvr	CustomizeablePowers\Poison\LobbYellowGlassScaleLarge.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_PoisonGlassBall

		Magnet	Target
		LookAt	Target

		Part	CustomizeablePowers\Poison\QuillTrailBlack.part
		Part	CustomizeablePowers\Poison\QuillTrail.part
		Part	CustomizeablePowers\Poison\QuillTrailLight.part

		Part	CustomizeablePowers\Poison\QuillTrailStreak.part
		Part2	CustomizeablePowers\Poison\YellowSmallBallzTrail.part
		Part3	CustomizeablePowers\Poison\QuillBubblesTrail.part
		Part3	CustomizeablePowers\Poison\QuillBubblesTrail.part
		Part5	CustomizeablePowers\Poison\QuillBallzTrail.part
		Part5	CustomizeablePowers\Poison\QuillBallzTrailBlack.part
		Part	CustomizeablePowers\Poison\YellowSmallStarsTrail.part

	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy


	End

End

End