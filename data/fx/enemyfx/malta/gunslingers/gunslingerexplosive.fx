#########################################################
##	
##

FxInfo


Input  
	InpName	Origin
End

Lifespan 100


Condition
	On Time
	Time 18
	Event
		Type	Local
		At	origin
		Sound gunslingershot3c 90 90 1.0
	End
End


Condition
	On Time
	Time 19
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:MuzzleFlashLight.bhvr
		LifeSpan 5
	End

End


Condition
	On 	Time
	Time 	18

	Event
		EName 	FlashnodeA
		Type	Local 
		At	WepR
		Bhvr	:GunslingerRevolverOffset1.bhvr
		LifeSpan 30
		Lookat	Target
	End

	Event
		EName 	FlashnodeB
		Type	Local 
		At	WepR
		Bhvr	:GunslingerRevolverOffset2.bhvr
		LifeSpan 30
		Lookat	Target
	End

	Event
		EName 	FlashnodeC
		Type	Local 
		At	WepR
		Bhvr	:GunslingerRevolverOffset3.bhvr
		LifeSpan 30
		Lookat	Target
	End

	Event
		EName 	Smokenode
		Type	Local
		At	WepR
		Bhvr	:GunslingerRevolverOffset1.bhvr
	End
End

Condition
	On 	Time
	Time 	18

	Event
		EName 	Flashnode
		Type	Start 
		At	FlashnodeA
		LifeSpan 30
	End

	Event
		EName 	Flashnode2
		Type	Start 
		At	FlashnodeB
		LifeSpan 30
	End

	Event
		EName 	Flashnode3
		Type	Start 
		At	FlashnodeC
		LifeSpan 30
	End

End


Condition
	On 	Time
	Time 	19

	Event
		EName 	Flash3
		Type	Start 
		At	Flashnode
		Part	:GunslingerMuzzleFlash02.part
		Part	:GunslingerMuzzleStar01.part
		Part	:GunslingerMuzzleFlash01.part
	End

	Event
		EName 	Flame5
		Type	Start 
		At	Flashnode
		Part	:GunslingerMuzzleFlame02.part
		Pmagnet	Flashnode3
	End

	Event
		EName 	Flame6
		Type	Start 
		At	Flashnode
		Part	:GunslingerMuzzleFlame01.part
		Pmagnet	Flashnode2
	End

End

Condition
	On 	Time
	Time 	28

	Event
		EName 	Smoke
		Type	Local
		At	Smokenode
		Part	:GunslingerMuzzleSmoke01.part
		Lifespan 60
	End
End

End	
