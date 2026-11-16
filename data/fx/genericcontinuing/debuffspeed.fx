#############################################################
## HEADER
#############################################################

FxInfo

Flags    InheritAlpha

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	HandR
		Sound glowroom_loop 20 20 .14
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 280
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Body_Glow
		Type	Local
		At	HandR
		part	:Slow_01.part
		part	:Slow_03.part
	End

	Event
		EName	Body_Glow
		Type	Local
		At	HandR
		part	:Slow_02.part
		POther	Hips
	End

	Event
		EName	Body_Glow
		Type	Local
		At	HandR
		part	:Slow_02.part
		POther	Chest
	End

	Event
		EName	Body_Glow
		Type	Local
		At	HandL
		part	:Slow_01.part
		part	:Slow_03.part
	End

	Event
		EName	Body_Glow
		Type	Local
		At	HandL
		part	:Slow_02.part
		POther	Hips
	End

	Event
		EName	Body_Glow
		Type	Local
		At	HandL
		part	:Slow_02.part
		POther	Chest
	End

	Event
		EName	Body_Glow
		Type	Local
		At	FootR
		part	:Slow_01.part
		part	:Slow_03.part
	End

	Event
		EName	Body_Glow
		Type	Local
		At	FootL
		part	:Slow_01.part
		part	:Slow_03.part
	End

	Event
		EName	Body_Glow
		Type	Local
		At	FootR
		part	:Slow_02.part
		POther	LLEgL
	End

	Event
		EName	Body_Glow
		Type	Local
		At	FootL
		part	:Slow_02.part
		POther	LLegR
	End
End

#############################################################

End