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
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserHitGlowInner.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserHitStar1.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserHitPop.part
		Sound titanlaserhit2 70 70 .6
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
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserHitPop2.part
		#Sound	titanlaserhit 70 60 .4
		Lifespan 2

	End

		
End