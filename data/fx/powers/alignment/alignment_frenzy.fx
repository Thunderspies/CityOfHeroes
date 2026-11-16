#############################################################
## LeadershipTactics.fx
#############################################################

FxInfo

##################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Chest
		Part	:Glow_Large_Red.part
		Part	:Glow_Small.part
		Lifespan 3
	End

	Event
		EName	FXAnchor
		Type	Local
		At	Chest
		Lifespan 500
	End
End

Condition
	On	Cycle
	Time	30

	Event
		Type	Local
		At	FXANchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glow_Small_Continuing.part
		Part	:Glow_Smaller_Continuing.part
		Part	:Glow_Small_Continuing_Subtractive.part
		Part	:Rings_Pulsing.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	13
	Event
		Type	Local
		At	Chest
		Part	:Sparks_Exploding.part
		Part	:Sparks_Exploding_Subtractive.part
		Part	:Streaks_Subtractive.part
		Part	:Rings_Subtractive.part
		Part	:Rings_Large_Subtractive.part
		Lifespan 4
	End
End