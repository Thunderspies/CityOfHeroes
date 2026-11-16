#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	50

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
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		part	:MassiveMegaSonicRingCore.part
		part	:MassiveMegaSonicRing.part		
		
		LifeSpan	10
	End

	Event
		EName	core3
		Type	start
		At	core
		  Altpiv   4
		part	:MassiveMegaSonicRingCore2.part
		part	:MassiveMegaSonicRing2.part		
		
		LifeSpan	10
	End

End

###############################################################################################################3

Condition
	On	Cycle
	Chance	.8
	Time	9
	
	Event
		EName	HeadRigging
		Type	start
		At	head
		part	:MassiveMegaSonicHeadCore.part	
		part	:MassiveMegaSonicHeadCoreDiffuse.part	
		
		LifeSpan	1

	End

End

Condition
	On	Time
	Time	3

	Event
		EName	HeadRigging
		Type	Destroy
		
	End
End	

End


			