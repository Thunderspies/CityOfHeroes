#############################################################
## SonicCannonConeBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Spinner
		Type	Local
		At	wepR
		Geom	SonicSpinnerOffset
	End

	Event
		Type	Local
		At	Spinner
		altpiv	1
		Geom	GoldBricker_Spinner
		BHVR	Behaviors/SonicGunSpinner.bhvr
		LifeSpan 30
	End

	Event
		Type	Local
		At	Spinner
		Sound Resonator5 80 80 .89
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	riggingoffset
		Type	local
		At	Spinner
		altpiv	2
	End

	Event
		EName	core
		Type	Local
		At	riggingoffset
		bhvr	:Offset.bhvr
		Geom	LocalPivot01
		LifeSpan 10
	End

	Event
		EName	core3
		Type	start
		At	core
		Altpiv	4
		part	:MegaSonicRingCore.part
		part	:MegaSonicRing.part
		LifeSpan 10
	End

	Event
		EName	core3
		Type	start
		At	core
		Altpiv	4
		part	:MegaSonicRingCore2.part
		part	:MegaSonicRing2.part
		LifeSpan 10
	End
End

#############################################################

End