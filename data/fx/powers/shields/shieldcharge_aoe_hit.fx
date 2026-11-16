#########################################################
## Super Strength - Hand Clap Hit
########################################################

FxInfo

Lifespan 180

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start
		At	Chest
		Part1	POWERS\SuperStrength\SuperStrengthHitEmber.part
		Part2	POWERS\SuperStrength\SuperStrengthHitStar.part
		Part3	POWERS\SuperStrength\SuperStrengthHitRing.part
		Part4	POWERS\SuperStrength\SuperStrengthHitRing2.part
		#Sound	PunchHit1 100.0 30.0 .8
	End
End

########################################################

End