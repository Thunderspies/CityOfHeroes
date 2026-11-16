#############################################################
## MegaBlast.fx
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
		BHVR	Behaviors/SonicGunSpinner.bhvr
		Geom	SonicSpinner2
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Spinner
		Sound Resonator5 80 80 .89
	End
End

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
		Geom	LocalPivot01
		bhvr	:Offset.bhvr
		LifeSpan 10
	End

	Event
		Type	start
		At	core
		Altpiv	4
		part	:MegaSonicRingCore.part
		part	:MegaSonicRingCore2.part
		part	:MegaSonicRing.part
		part	:MegaSonicRing2.part
		LifeSpan 10
	End
End

#############################################################

End