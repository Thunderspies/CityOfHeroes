#############################################################
## DominateMind_Hit.fx
#############################################################

FxInfo
LifeSpan	150

## HANDS ###########################################################
Condition
	On	Time
	Time 	0

	Event
		Type	PositOnly
		At	Head
		Sound Seers_DominateMind_Hit 50 50 1
		Part	:Head_Glow_Burst.part
		Part	:Head_Rings_Growing.part
		Lifespan 150
	End
End

Condition
	On	Cycle
	Time	20

	Event
		Type	PositOnly
		At	Head
		Part	:Head_Streaks_Big.part
		Lifespan 30
	End

End

Condition
	On	Time
	Time	10

	Event
		Type	local
		At	Head
		Geom	centerdummy
		Part	:Head_Glow.part
		Lifespan 120
	End

	Event
		EName	EyeFX
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeFX
		AltPiv	1
		Part	:Eye_Glow_Left.part
		Lifespan 120
	End

	Event
		Type	Local
		At	EyeFX
		AltPiv	2
		Part	:Eye_Glow_Right.part
		Lifespan 120
	End
End

Condition
	On	Time
	Time	43

	Event
		Type	PositOnly
		At	Head
		Part	:Head_Glow_Burst.part
		Part	:Hands_Rings_Growing.part
		Part	:Head_Streaks_Big.part
		Lifespan 1
	End
End

##############################################################################

End