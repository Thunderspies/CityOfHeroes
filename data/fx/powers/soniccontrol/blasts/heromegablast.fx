#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

###########################################################################################################

condition
	On	Time
	Time	24	

	Event	
		Type	Local
		At	Head
		Sound Resonator3 80 80 .88
	End
End



Condition
	On 	Time
	Time 	29

	Event
		EName	core
		Type	start
		At	head
		Geom	LocalPivot01
		bhvr	:HeroOffset.bhvr	
		
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

#########################################################################################################


End	


			