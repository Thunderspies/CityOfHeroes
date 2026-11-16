#############################################################
## TestTube_Success.fx
#############################################################

FxInfo
Lifespan 255

#############################################################
Condition
	On	Time
	Time	0

	Event
		Ename	RightFlask
		Type	Local
		At	WepR
		Geom	GEO_ErlenmeyerFlask_01
		Lifespan 255
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
		Lifespan 255
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
	On Time
	Time 	5

	Event
		Type	Local
		At	Root
		sound	SSTestTubeSuccess 200 190 1
	End
End

Condition
	On 	Time
	Time 	22

	Event
		Type	PositOnly
		At	LeftFlaskOffset
		Part	:LiquidPour.part
		POther	RightFlaskOffset
		Lifespan 20
	End
End

Condition
	On	Time
	Time	65

	Event
		Type	PositOnly
		At	RightFlaskOffset
		Part	:Geyser.part
		Part	:Geyser_Alpha.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	70

	Event
		Type	PositOnly
		At	WepR
		BhvrOverride
			PositionOffset 0 1 0
		End
		Part	:Smoke_Pop.part
		Part	:SmokeThin_Pop.part
		Lifespan 120
	End

	Event
		Type	PositOnly
		At	RightFlaskOffset
		Part	:SmokeThin.part
		Part	:Smoke.part
		Part	:Bubbles.part
		Lifespan 150
	End
End

Condition
	On	Time
	Time	220

	Event
		Type	PositOnly
		At	RightFlaskOffset
		Part	:Smoke_Trail.part
		LIfespan 30
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