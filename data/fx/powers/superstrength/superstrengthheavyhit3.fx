#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Chest
End

Lifespan 6

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Sound atomicblast2 100 100 .6
	End
End

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


End
