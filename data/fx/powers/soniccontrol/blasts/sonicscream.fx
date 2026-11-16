#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

###########################################################################################################

Condition
	On 	Time
	Time 	29

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		bhvr	:Offset.bhvr	
		
		LifeSpan	10
	End

	Event
		EName	core3
		Type	start
		At	core
		  Altpiv   4
		part	:MegaSonicRingCore.part
		part	:MegaSonicRing.part		
		
		LifeSpan	10
	End

	Event
		EName	core3
		Type	start
		At	core
		  Altpiv   4
		part	:MegaSonicRingCore2.part
		part	:MegaSonicRing2.part		
		
		LifeSpan	10
	End

End

End	


			