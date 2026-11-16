#########################################################
## Super Strength - Foot Stomp Hit
########################################################

FxInfo

Lifespan 6

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start
		At	Chest
		bhvr	behaviors/ScaleBubble3.bhvr
		BhvrOverride
			RGB0 0 0 255
		End
		Geom	GEO_WhiteSphere
	End

	Event
		EName 	Mallet
		Type	start
		At	Chest

		Part1	:SuperStrengthHitEmber.part
		Part2	:SuperStrengthHitStar.part
		Part3	:SuperStrengthHitRing.part
		Part4	:SuperStrengthHitRing2.part
		Sound ReichsmanFootStomp_01 100.0 100.0 .8
	End
End

########################################################

End