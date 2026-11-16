#########################################################
##	iceweapon
########################################################
FxInfo

LifeSpan	100

Input  
	Inpname	Chest
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start
		At	Chest
		Part1	:PowerPunchHit.part
		Part2	:PowerPunchHit1.part
		Part3	:PowerPunchHit2.part
		Part4	:PowerPunchHitRaysSmall.part
		Sound energytransferhit 80 80 .85
	End


	Event
		Type	start
		At	Chest
		Part1	:HitRing.part
		Part2	:PowerPunchBallz.part
		Part3	:HitRing2.part
		Part4	:PowerPunchRingz.part
	End


End


End
