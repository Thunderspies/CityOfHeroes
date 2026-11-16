#########################################################
## Rikti Transport Shuttle - Persistent FX
#########################################################

FxInfo

## SOUND FX #######################################################

#Condition
#
#	On 	Time	
#	Time	18
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans4 80 30 .88
#	End
#
#End
#
#Condition
#
#	On 	Time	
#	Time	70
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans3 80 30 .78
#	End
#
#End

## GEOMETRY #######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename	RiktiShuttle
#		Type	Local
#		At	Hips
#		Geom	RiktiShuttle
#	End
#End

## ANTANNAE LIGHTS #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		-11 -29 53
		End
		Part	:BlinkingLight_Core.part
		Part	:BlinkingLight_Glow.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		11 -29 53
		End
		Part	:BlinkingLight_Core.part
		Part	:BlinkingLight_Glow.part
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		1.5 39 -14.5
		End
		Part	:BlinkingLight_Core.part
		Part	:BlinkingLight_Glow.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		1.5 61 -47
		End
		Part	:BlinkingLight_Core.part
		Part	:BlinkingLight_Glow.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		1.5 39 -50
		End
		Part	:BlinkingLight_Core.part
		Part	:BlinkingLight_Glow.part
	End
End

## TAIL FIN ORBS #######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename	Lights
#		Type	local
#		At	WepR
#		Part	:LargeLight_Glow.part
#	End
#
#	Event
#		Ename	Lights
#		Type	local
#		At	WepL
#		Part	:LargeLight_Glow.part
#	End
#End

## STARBOARD ENGINE #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	UArmR
		BhvrOverride
			PositionOffset		0 0 2
		End
		Part	:Thruster_Idle_Glow1.part
	End

	Event
		Ename	Upwards
		Type	local
		At	LArmR
		BhvrOverride
			PositionOffset		0 0 2
		End
		Part	:Thruster_Idle_Glow1.part
	End
End

## PORT ENGINE #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	UArmL
		BhvrOverride
			PositionOffset		0 0 2
		End
		Part	:Thruster_Idle_Glow1.part
	End

	Event
		Ename	Upwards
		Type	local
		At	LArmL
		BhvrOverride
			PositionOffset		0 0 2
		End
		Part	:Thruster_Idle_Glow1.part
	End
End

## EXHAUST STARBOARD #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	ULegR
		BhvrOverride
			PyrRotate	90 0 0
			PositionOffset	0 0 1
		End
		part	:ExhaustPort_Core1.part
		part	:ExhaustPort_Core2.part
	End

	Event
		Ename	Upwards
		Type	local
		At	ULegR
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
	End

#########################################################

	Event
		Ename	Upwards
		Type	local
		At	LLegR
		BhvrOverride
			PyrRotate	90 0 0
			PositionOffset	0 0 1
		End
		part	:ExhaustPort_Core1.part
		part	:ExhaustPort_Core2.part
	End

	Event
		Ename	Upwards
		Type	local
		At	LLegR
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
	End

#########################################################

	Event
		Ename	Upwards
		Type	local
		At	FootR
		BhvrOverride
			PyrRotate	90 0 0
			PositionOffset	0 0 1
		End
		part	:ExhaustPort_Core1.part
		part	:ExhaustPort_Core2.part
	End

	Event
		Ename	Upwards
		Type	local
		At	FootR
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
	End
End

## EXHAUSTS PORT #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	ULegL
		BhvrOverride
			PyrRotate	90 0 0
			PositionOffset	0 0 1
		End
		part	:ExhaustPort_Core1.part
		part	:ExhaustPort_Core2.part
	End

	Event
		Ename	Upwards
		Type	local
		At	ULegL
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
	End

#########################################################

	Event
		Ename	Upwards
		Type	local
		At	LLegL
		BhvrOverride
			PyrRotate	90 0 0
			PositionOffset	0 0 1
		End
		part	:ExhaustPort_Core1.part
		part	:ExhaustPort_Core2.part
	End

	Event
		Ename	Upwards
		Type	local
		At	LLegL
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
	End

#########################################################

	Event
		Ename	Upwards
		Type	local
		At	FootL
		BhvrOverride
			PyrRotate	90 0 0
			PositionOffset	0 0 1
		End
		part	:ExhaustPort_Core1.part
		part	:ExhaustPort_Core2.part
	End

	Event
		Ename	Upwards
		Type	local
		At	FootL
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
	End
End

## FAKE AIR BOMBS #######################################################

Condition
	On	Cycle
	Time	50
	Chance	0.25

	Event
		Ename	FakeBombs
		Type	Start
		At	Root
		ChildFX	:FakeBombs\FakeAirBomb_Starboard1.fx
		Lifespan	300
	End
End

Condition
	On	Cycle
	Time	60
	Chance	0.25

	Event
		Ename	FakeBombs
		Type	Start
		At	Root
		ChildFX	:FakeBombs\FakeAirBomb_Starboard2.fx
		Lifespan	300
	End
End

Condition
	On	Cycle
	Time	70
	Chance	0.25

	Event
		Ename	FakeBombs
		Type	Start
		At	Root
		ChildFX	:FakeBombs\FakeAirBomb_Port1.fx
		Lifespan	300
	End
End

Condition
	On	Cycle
	Time	80
	Chance	0.25

	Event
		Ename	FakeBombs
		Type	Start
		At	Root
		ChildFX	:FakeBombs\FakeAirBomb_Port2.fx
		Lifespan	300
	End
End

## FAKE BOMBS #######################################################

Condition
	On	Cycle
	Time	90
	Chance	0.5

	Event
		Ename	FakeBombs
		Type	Start
		At	Root
		ChildFX	:FakeBombs\FakeBombs.fx
		Lifespan	300
	End
End

#########################################################

End		