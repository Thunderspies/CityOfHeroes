#########################################################
##	
##

FxInfo


LifeSpan	120
##################################
##################################
##################################

Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local
		At	chest
		Sound bladehit1 70 70 .8
	End
End

##################################
##################################
##################################

Condition
	On 	Time
	Time 	0

	Event	
		ENAME	CrossHairNode
		Type	Start
		At	Chest
		Part	:silverSparks1.part
		Part3	:SilverHitStar01.part
		Part4	:SilverHitGlow01.part
		Part5	:SilverHitRays01.part
		Part2	:SilverHitStar01.part
		Part1	:SilverHitRays01.part
		Lifespan 5
		
	End

End

End				