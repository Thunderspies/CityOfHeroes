#############################################################
## Summon_Attack.fx
#############################################################

FxInfo
LifeSpan	150

## HANDS #####################################################


Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Root
		Sound Seers_SummonReinforcements_Attack 100 100 1
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	ReticleAnchor
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0 0.2 0
		End
	End


End

Condition
	On	Time
	Time	20

	Event
		Type	PositOnly
		At	Chest
		Part	:Torso_Rings_Falling.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	PositOnly
		At	ReticleAnchor
		Part	:Target_Reticle.part
		Lifespan 30
	End
End

##############################################################################

End