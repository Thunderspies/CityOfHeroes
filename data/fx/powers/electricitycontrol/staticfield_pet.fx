#############################################################
## StunningField_Pet.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 .1 0
		End
		Part	:Root_Electricity_Ring2.part
		Part	:Root_Electricity_Glow_Flat.part

	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.5

	Event
		Type	Start
		At	Root
		Part	:Root_Electricity_Ring_Pulse.part
		Lifespan 3
	End
End

Condition
	On	Cycle
	Time	6
	Chance	0.5

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 0.1 0
		End
		Part	:Root_Lightning_Fast_Detached.part
		Part	:Root_Glow_Fast_Detached.part
		Part	:Root_Lightning_Fast_Flat_Detached.part
		Lifespan 2
	End
End

Condition
	On	Cycle
	Time	6
	Chance	0.5

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 0.1 0
		End
		Part	:Root_Lightning_Fast_Flat.part
		Part	:Root_Glow_Flat.part

		Lifespan 2
	End
End
#############################################################

End
