#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	80

###########################################################################################################

Condition
	On	Time
	Time	19

	Event
		Type	Local
		At	Origin
		Sound SonicWave4 90 90 .99
	End
End


Condition
	On 	Time
	Time 	24

	Event
		EName 	MussleFlashRigging
		Type	local 
		At	head
		Geom	PsionicRigging	
		#Sound	SonicWave 90 80 .99		
	End

End

Condition
	On 	Time
	Time 	25

	Event
		Type	start 
		At	MussleFlashRigging
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	40
		
	End

End


Condition
	On 	Time
	Time 	25

	Event
		Type	start 
		At	T_Chest
		Part	:HitRingCoreHuge2.part
		Part	:HitRingHuge2.part
		
		LifeSpan	8
		
	End

End


Condition
	On 	Time
	Time 	45

	Event
		Type	start 
		At	T_Chest
		Part	:HitRingCoreHuge2.part
		Part	:HitRingHuge2.part
		
		LifeSpan	8
		
	End

End

Condition
	On 	Time
	Time 	55

	Event
		Type	start 
		At	T_Chest
		Part	:HitRingCoreHuge2.part
		Part	:HitRingHuge2.part
		
		LifeSpan	8
		
	End

End

End	


			