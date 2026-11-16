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
		Geom	Tintable_Dark_SuperStrengthBubble
		bhvr	CustomizeablePowers\SuperStrength\ScaleBubble.bhvr
		Lifespan	6
	End

	Event
		EName 	Mallet
		Type	start
		At	Chest

		Part1	:SuperStrengthHitEmber.part
		Part2	:SuperStrengthHitStar.part
		Part3	:SuperStrengthHitRing.part
		Part4	:SuperStrengthHitRing2.part
		Sound PunchHit2 100.0 100.0 .8
	End
End

########################################################

End