#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	120

###########################################################################################################

Condition
	On 	Time
	Time 	6

	Event
		Type	local 
		At	head
		#Sound	HeroSonicBlast2 80 70 .99
		Sound Snakewail2 80 80 .99
		LifeSpan	120	

	End
End


Condition
	On 	Time
	Time 	6

	Event
		Type	local 
		At	head
		#Sound	HeroSonicBlast2 90 70 .99
		Sound avalanche2 180 180 .99
		LifeSpan	120	

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
			
		LifeSpan	120	
	End

	Event
		EName 	WepLRigg
		Type	local 
		At	WepL
		BHVR	:HandCastRotate.bhvr
		Geom	PsionicRigging	
				
		LifeSpan	120
	End
	
	Event
		EName 	WepRRigg
		Type	local 
		At	WepR
		BHVR	:HandCastRotateR.bhvr
		Geom	PsionicRigging	
			
		LifeSpan	120	
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	local 
		At	MussleFlashRigging
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	70
		
	End


End

End	


			