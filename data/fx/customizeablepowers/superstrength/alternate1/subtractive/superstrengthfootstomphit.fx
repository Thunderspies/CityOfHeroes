#########################################################
## Super Strength - Foot Stomp Hit
########################################################

FxInfo

Lifespan 6

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
			Alpha		80
			TintGeom	1
		End
		Geom	Tintable_Dark_SuperStrengthBubble
		bhvr	CustomizeablePowers\SuperStrength\ScaleBubble.bhvr
	End

	Event
		EName 	Mallet
		Type	start
		At	Chest

		Part1	CustomizeablePowers\SuperStrength\Subtractive\SuperStrengthHitEmber.part
		Part2	CustomizeablePowers\SuperStrength\Subtractive\SuperStrengthHitStar.part
		Part3	CustomizeablePowers\SuperStrength\Subtractive\SuperStrengthHitRing.part
		Part4	CustomizeablePowers\SuperStrength\Subtractive\SuperStrengthHitRing2.part
		Sound PunchHit2 100.0 100.0 .8
	End
End

########################################################

End