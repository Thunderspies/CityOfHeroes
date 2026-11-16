#########################################################
##	Fire_Ball
##
FxInfo

#LifeSpan	200

###########################################################################################################

Condition
	On 	Time
	Time 	6

	Event
		Type	local 
		At	head
		Sound HeroSonicBlast2 80 80 .99
				
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
			
		LifeSpan	140	
	End

	Event
		EName 	WepLRigg
		Type	local 
		At	WepL
		BHVR	:HandCastRotate.bhvr
		Geom	PsionicRigging	
				
		LifeSpan	140
	End
	
	Event
		EName 	WepRRigg
		Type	local 
		At	WepR
		BHVR	:HandCastRotateR.bhvr
		Geom	PsionicRigging	
			
		LifeSpan	140	
	End
	
End

Condition
	On 	Time
	Time 	14

	Event
		Type	start 
		At	MussleFlashRigging
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	90
		
	End

	Event
		Type	start 
		At	WepLRigg
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	90
		
	End

	Event
		Type	start 
		At	WepRRigg
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	90
		
	End
End

End	


			