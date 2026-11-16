#############################################################
## TestTube_Failure.fx
#############################################################

FxInfo
Lifespan 265

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	LeftFlask
		Type	Local
		At	WepL
		Geom	GEO_ErlenmeyerFlask_01
	End

	Event
		Ename	LeftFlaskOffset
		Type	Local
		At	LeftFlask
		AltPiv	1
		Part	:Smoke_Trail.part
	End
End

Condition
	On	Time
	Time 	12

	Event
		Type	Local
		At	Root
		sound	SSTestTubeFailure 200 190 1
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	LeftFlaskOffset
		Part	:LiquidPour.part
		POther	RightFlaskOffset
		Lifespan 35
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	RightFlask
		Type	Local
		At	WepR
		Geom	GEO_ErlenmeyerFlask_01
		Lifespan 154
	End

	Event
		Ename	RightFlaskOffset
		Type	Local
		At	RightFlask
		AltPiv	1
		Part	:Smoke_Trail.part
		Lifespan 68
	End
End

Condition
	On	Time
	Time	68

	Event
		Type	PositOnly
		At	RightFlask
		AltPiv	1
		Part	:SmokeThin_Fail.part
		Part	:Smoke_Fail.part
		Part	:BeakerSparks_Fail.part
		Part	:BeakerSparksVertical_Fail.part
		Part	:SmokePillarThin_Fail.part
		Part	:SmokePillarThin2_Fail.part
		Lifespan 90
	End

	Event
		Type	PositOnly
		At	RightFlask
		AltPiv	1
		Part	:Geyser_Fail.part
		Part	:Geyser_Alpha_Fail.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	184

	Event
		Ename	RightFlask
		Type	Local
		At	WepR
		Geom	GEO_ErlenmeyerFlask_01
	End

	Event
		Type	Local
		At	RightFlask
		AltPiv	1
		Part	:Smoke_Trail.part
	End
End

#############################################################

Condition
	On	Time
	Time	154

	Event
		Ename	ThrownFlask
		Type	Local
		At	Mystic
		Geom	GEO_ErlenmeyerFlask_01
		Lifespan 2
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