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
		Geom	SuperStrengthBubble
		bhvr	behaviors/ScaleBubble.bhvr
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