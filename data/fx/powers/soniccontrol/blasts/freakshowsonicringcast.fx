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
		EName 	RightSpeakerRigging
		Type	local 
		At	head

		BhvrOverride
			PositionOffset	-1.2 0 0
		End

		Geom	PsionicRigging	
			
		LifeSpan	140	
	End

	Event
		EName 	LeftSpeakerRigging
		Type	local 
		At	head

		BhvrOverride
			PositionOffset	1.2 0 0
		End

		Geom	PsionicRigging	
			
		LifeSpan	140	
	End

	
End

Condition
	On 	Time
	Time 	17

	Event
		Type	start 
		At	RightSpeakerRigging
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	90
		
	End

	Event
		Type	start 
		At	leftSpeakerRigging
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	90
		
	End

End

End	


			