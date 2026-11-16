#########################################################
##	Fire_Ball
##
FxInfo

Condition
	On 	Time
	Time 	0

	Event
		EName 	rigging
		Type	local 
		At	head
		Geom	PsiTele02

	End

	Event
		EName 	MussleFlashRigging
		Type	local 
		At	head
		Geom	PsionicRigging	
				
	End

End


Condition
	On 	Time
	Time 	14

	Event
		Type	local 
		At	MussleFlashRigging
		altpiv	3
		Part	Powers/SonicControl/Blasts/MussleFlashRingCore.part
		Part	Powers/SonicControl/Blasts/MussleFlashRing.part
		
		LifeSpan	8
		
	End

End

Condition
	On 	Time
	Time 	15


	Event
			
		Type	start
		At	T_Root
		
		Part	:SonicRing.part
		Part	:SonicRingCore.part

	End

End

End	


			