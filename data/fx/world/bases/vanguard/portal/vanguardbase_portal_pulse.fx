#########################################################
## Vanguard Base Porta - Pulse Child FX
#########################################################

FxInfo

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound EntrancePortal1_loop 80 80 .58
	End
End

Condition
	On 	Cycle
	Time 	155
	Chance	0.9

	Event
		EName 	Close
		Type	Start 
		Bhvr	:VanguardBase_Portal_RiftCloser.bhvr
		part	:VanguardBase_Portal_Ring01.part
		Lifespan	30
	End

	Event
		EName 	Close
		Type	Start 
		part	:VanguardBase_Portal_Ring01.part
		Lifespan	30
	End

	Event
		EName 	closeHelix1
		Type	Local 
		At	Close
		Bhvr	:VanguardBase_Portal_RiftOffset.bhvr
		part	:VanguardBase_Portal_Glow01.part
		Lifespan	30
	End

	Event
		EName 	closeHelix2
		Type	Local 
		At	Close
		Bhvr	:VanguardBase_Portal_RiftOffset2.bhvr
		part	:VanguardBase_Portal_Glow01.part
		Lifespan	30
	End
End

Condition
	On 	Cycle
	Time 	190	
	Chance	0.8

	Event
		EName 	gravWell
		Type	Local
		Magnet	helix2
		part	:VanguardBase_Portal_Mist01.part
		part	:VanguardBase_Portal_Mist02.part
		Lifespan	5
	End
End

Condition
	On 	Cycle
	Time 	45
	Chance	0.5
	
	Event
		EName 	Arcs
		Type	Start
		part	:VanguardBase_Portal_LightningArcs1.part
		Lifespan	20
	End
End

Condition
	On 	Cycle
	Time 	175
	Chance	0.9

	Event
		EName 	Start
		Type	Start 
		part	:VanguardBase_Portal_Ring02.part
		Lifespan	30
	End

	Event
		EName 	Open
		Type	Start 
		Bhvr	:VanguardBase_Portal_RiftOpener.bhvr
		Lifespan	30
	End

	Event
		EName 	openHelix1
		Type	Local 
		At	Open
		Bhvr	:VanguardBase_Portal_RiftOffset.bhvr
		part	:VanguardBase_Portal_Glow01.part
		Lifespan	30
	End

	Event
		EName 	openHelix2
		Type	Local 
		At	Open
		Bhvr	:VanguardBase_Portal_RiftOffset2.bhvr
		part	:VanguardBase_Portal_Glow01.part
		Lifespan	30
	End
End

Condition
	On 	Cycle
	Time 	135
	Chance	0.6

	Event
		EName 	flashhh
		Type	Start 
		part	:VanguardBase_Portal_Rift03.part
		part	:VanguardBase_Portal_Rift04.part
		Lifespan	5
	End
End

#########################################################

End