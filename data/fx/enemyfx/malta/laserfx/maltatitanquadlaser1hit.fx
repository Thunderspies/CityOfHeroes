#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
Input  
	InpName	Origin
End


Input  
	InpName	Target
End


Input  
	InpName	mystic
End

Input  
	InpName	WepR
End

LifeSpan	200

########################################################
###################### Audio ###########################
########################################################

Condition
	On	Time
	Time	0
	Event	
		Type	Local
		At	Origin
		Sound titanlaserhit1 70 70 .4
	End

End

Condition
	On	Time
	Time	0
	Event	
		Type	Local
		At	Origin
		Sound titanlaserhit2 70 70 .7
	End

End




########################################################
###################### Right Hand ######################
########################################################



Condition
	On 	Time
	Time 	0
	
	Event
		EName 	LaserHit1
		Type	Start
		At	Chest
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitGlowInner2.part
		Part	:MaltaQLaserHitSparks.part
		Sound titanlaserhit 70 70 .82
		Lifespan 1

	End

		
End


Condition
	On 	Time
	Time 	6
	
	Event
		EName 	LaserHit1
		Type	Start
		At	Chest
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitGlowInner2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

Condition
	On 	Time
	Time 	12
	
	Event
		EName 	LaserHit1
		Type	Start
		At	Chest
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitGlowInner2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

Condition
	On 	Time
	Time 	18
	
	Event
		EName 	LaserHit1
		Type	Start
		At	Chest
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitGlowInner2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

Condition
	On 	Time
	Time 	24
	
	Event
		EName 	LaserHit1
		Type	Start
		At	Chest
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitGlowInner2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End

Condition
	On 	Time
	Time 	30
	
	Event
		EName 	LaserHit1
		Type	Start
		At	Chest
		Part	:MaltaQLaserHitGlowInner.part
		Part	:MaltaQLaserHitStar2.part
		Part	:MaltaQLaserHitGlowInner2.part
		Part	:MaltaQLaserHitSparks.part
		Lifespan 1

	End

		
End


End			