#########################################################
##	
##

FxInfo
Lifespan 400

#Condition
#	On Time
#	Time 1
#	Event
#		EName 	Pulse
#		Type	SetLight
#		Bhvr	:MuzzleFlashLight.bhvr
#		LifeSpan 5
#	End
#
#End
#
#Condition
#	On Time
#	Time 7
#	Event
#		EName 	Pulse
#		Type	SetLight
#		Bhvr	:MuzzleFlashLight.bhvr
#		LifeSpan 5
#	End
#
#End
#
#Condition
#	On Time
#	Time 14
#	Event
#		EName 	Pulse
#		Type	SetLight
#		Bhvr	:MuzzleFlashLight.bhvr
#		LifeSpan 5
#	End
#
#End
#
#
Condition
	On 	Time
	Time 	0

	Event
		EName 	Flash1
		Type	Local 
		At	Chest
		Part	:GunslingerHitFlash02.part
		Part	:GunslingerRFHitFlash01.part
		Part	:GunslingerHitStar01.part
	End

End

Condition
	On 	Time
	Time 	6

	Event
		EName 	Flash1
		Type	Local 
		At	Chest
		Part	:GunslingerHitFlash02.part
		Part	:GunslingerRFHitFlash01.part
		Part	:GunslingerHitStar01.part
	End

End

Condition
	On 	Time
	Time 	13

	Event
		EName 	Flash1
		Type	Local 
		At	Chest
		Part	:GunslingerHitFlash02.part
		Part	:GunslingerRFHitFlash01.part
		Part	:GunslingerHitStar01.part
	End

End

End	
