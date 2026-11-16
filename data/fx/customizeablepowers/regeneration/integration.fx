#############################################################
## Integration.fx
#############################################################

FxInfo

########################################################

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors\soundFade1.bhvr
		Sound heal1_loop 60 60 0.45
		Flags	PowerLoopingSound
		Lifespan 130
	End
End

#############################################################

Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Regeneration\core4.part
		Part	CustomizeablePowers\Regeneration\Electrons.part
		Part	CustomizeablePowers\Regeneration\StarStreaks1.part
		Part	CustomizeablePowers\Regeneration\StarStreaksDown1.part
		Part	CustomizeablePowers\Regeneration\StarStreaksRightc1.part
		Sound regen4 60 60 1.0
	End

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Regeneration\tealmist.part
		Part	CustomizeablePowers\Regeneration\CoreRings.part
		Part	CustomizeablePowers\Regeneration\StarStreaksRightb1.part
		Part	CustomizeablePowers\Regeneration\StarStreaksLeftb1.part
		Part	CustomizeablePowers\Regeneration\StarStreaksLeftc1.part
		Sound regen3 60 60 0.8
	End

	Event
		Type	Local
		At	chest
		Part	CustomizeablePowers\Regeneration\SparkliesTrail.part
	End
End

#############################################################

End