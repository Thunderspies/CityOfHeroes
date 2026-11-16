#########################################################
## Super Strength - Heavy Hit
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
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_SuperStrengthBubble
		bhvr	CustomizeablePowers\SuperStrength\ScaleBubble.bhvr
		Lifespan	6
	End

	Event
		EName 	Mallet
		Type	start
		At	Chest

		Part1	CustomizeablePowers\SuperStrength\SuperStrengthHitEmber.part
		Part2	CustomizeablePowers\SuperStrength\SuperStrengthHitStar.part
		Part3	CustomizeablePowers\SuperStrength\SuperStrengthHitRing.part
		Part4	CustomizeablePowers\SuperStrength\SuperStrengthHitRing2.part
		Sound PunchHit2 100.0 100.0 .8
	End
End

########################################################

End