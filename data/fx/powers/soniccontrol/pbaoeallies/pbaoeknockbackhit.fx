#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	40

###########################################################################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	start 
		At	Chest
		Part	:SonicHitRingCore.part
		Part	:SonicHitRing.part
		Sound SonicRepulsion1 100 100 .88
		LifeSpan	18
		
	End

End


End	


			