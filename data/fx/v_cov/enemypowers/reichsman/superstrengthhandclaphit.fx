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
		Part1	:SuperStrengthHitEmber.part
		Part2	:SuperStrengthHitStar.part
		Part3	:SuperStrengthHitRing.part
		Part4	:SuperStrengthHitRing2.part
		Sound ReichsmanHandclap_01 100.0 100.0 .8
	End
End

########################################################

End