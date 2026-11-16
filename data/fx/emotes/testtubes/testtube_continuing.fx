#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo

#############################################################
## LOOP
#############################################################
Condition
	On 	Time
	Time 	0

	Event
		Ename	RightFlask
		Type	Local
		At	WepR
		Geom	GEO_ErlenmeyerFlask_01
		sound	SSTestTubeContinuing_Loop 200 190 .1
	End

	Event
		Ename	LeftFlask
		Type	Local
		At	WepL
		Geom	GEO_ErlenmeyerFlask_01
	End

	Event
		Ename	RightFlaskOffset
		Type	Local
		At	RightFlask
		AltPiv	1
		Part	:Smoke_Trail.part
	End

	Event
		Ename	LeftFlaskOffset
		Type	Local
		At	LeftFlask
		AltPiv	1
		Part	:Smoke_Trail.part
	End
End

#############################################################

Condition
	On	DEATH

	Event
		Type	Destroy
		Ename	ALL
	End
End

#############################################################

End

