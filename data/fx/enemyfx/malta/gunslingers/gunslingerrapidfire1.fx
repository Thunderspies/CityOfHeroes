#########################################################
##	
##

FxInfo
Lifespan 400

Condition
	On Time
	Time 15
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:MuzzleFlashLight.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 21
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:MuzzleFlashLight.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 28
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:MuzzleFlashLight.bhvr
		LifeSpan 5
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Flashnode
		Type	Local 
		At	WepR
		Bhvr	:GunslingerRevolverOffset1.bhvr
	End

	Event
		EName 	Flashnode2
		Type	Local 
		At	WepR
		Bhvr	:GunslingerRevolverOffset2.bhvr
	End

	Event
		EName 	Flashnode3
		Type	Local 
		At	WepR
		Bhvr	:GunslingerRevolverOffset3.bhvr
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Flash1
		Type	Local 
		At	Flashnode
		Part	:GunslingerMuzzleFlash02.part
		Part	:GunslingerMuzzleStar01.part
		Part	:GunslingerMuzzleFlash01.part
	End

	Event
		EName 	Flame1
		Type	Local 
		At	Flashnode
		Part	:GunslingerMuzzleFlame02.part
		Pmagnet	Flashnode3
	End

	Event
		EName 	Flame2
		Type	Local 
		At	Flashnode
		Part	:GunslingerMuzzleFlame01.part
		Pmagnet	Flashnode2
	End

End

Condition
	On 	Time
	Time 	21

	Event
		EName 	Flash2
		Type	Local 
		At	Flashnode
		Part	:GunslingerMuzzleFlash02.part
		Part	:GunslingerMuzzleStar01.part
		Part	:GunslingerMuzzleFlash01.part
	End

	Event
		EName 	Flame3
		Type	Local 
		At	Flashnode
		Part	:GunslingerMuzzleFlame02.part
		Pmagnet	Flashnode3
	End

	Event
		EName 	Flame4
		Type	Local 
		At	Flashnode
		Part	:GunslingerMuzzleFlame01.part
		Pmagnet	Flashnode2
	End
End

Condition
	On 	Time
	Time 	28

	Event
		EName 	Flash3
		Type	Local 
		At	Flashnode
		Part	:GunslingerMuzzleFlash02.part
		Part	:GunslingerMuzzleStar01.part
		Part	:GunslingerMuzzleFlash01.part
	End

	Event
		EName 	Flame5
		Type	Local 
		At	Flashnode
		Part	:GunslingerMuzzleFlame02.part
		Pmagnet	Flashnode3
	End

	Event
		EName 	Flame6
		Type	Local 
		At	Flashnode
		Part	:GunslingerMuzzleFlame01.part
		Pmagnet	Flashnode2
	End

	Event
		EName 	Smoke
		Type	Local 
		At	Flashnode
		Part	:GunslingerMuzzleSmoke01.part
		Lifespan 60
	End
End

End	
