#########################################################
## Super Strength - Hit
########################################################

FxInfo

Lifespan 60

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start
		At	Chest
		Part1	CustomizeablePowers\SuperStrength\SuperStrengthHitEmber.part
		Part2	CustomizeablePowers\SuperStrength\SuperStrengthHitStar.part
		Part3	CustomizeablePowers\SuperStrength\SuperStrengthHitRing.part
		Part4	CustomizeablePowers\SuperStrength\SuperStrengthHitRing2.part
		Sound PunchHit1 100.0 100.0 .8
	End
End

########################################################

End