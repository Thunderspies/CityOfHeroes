#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	80

Input  
	InpName	Chest
End


###########################################################

Condition
	On 	Time
	Time 	2
	
	Event
		EName 	LaserHit
		Type	Local
		At	Chest
		Part	:MaltaLaserHitGlowInner.part
		Part	:MaltaLaserHitStar1.part
		Part	:MaltaLaserHitPop.part
		Sound titanlaserhit2 70 70 .45
		Lifespan 2

	End

		
End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	LaserHit
		Type	Local
		At	Chest
		Part	:MaltaLaserHitPop2.part
		Sound titanlaserhit 70 70 .4
		Lifespan 2

	End

		
End