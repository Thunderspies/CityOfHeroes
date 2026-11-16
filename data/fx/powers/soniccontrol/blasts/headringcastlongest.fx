#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	90

###########################################################################################################

Condition
	On 	Time
	Time 	6

	Event
		Type	local 
		At	head
		Sound HeroSonicBlast2 80 80 .99
			
		LifeSpan	90	

	End
End




Condition
	On 	Time
	Time 	0

	Event
		EName 	MussleFlashRigging
		Type	local 
		At	head
		Geom	PsionicRigging	
			
		LifeSpan	90	
	End

	Event
		EName 	WepLRigg
		Type	local 
		At	WepL
		BHVR	:HandCastRotate.bhvr
		Geom	PsionicRigging	
				
		LifeSpan	90
	End
	
	Event
		EName 	WepRRigg
		Type	local 
		At	WepR
		BHVR	:HandCastRotateR.bhvr
		Geom	PsionicRigging	
			
		LifeSpan	90	
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	start 
		At	MussleFlashRigging
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	50
		
	End


End

End	


			