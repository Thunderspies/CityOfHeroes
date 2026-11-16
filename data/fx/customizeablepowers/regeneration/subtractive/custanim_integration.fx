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
		Part	CustomizeablePowers\Regeneration\Subtractive\core4.part
		Part	CustomizeablePowers\Regeneration\Subtractive\Electrons.part
		Part	CustomizeablePowers\Regeneration\Subtractive\StarStreaks1.part
		Part	CustomizeablePowers\Regeneration\Subtractive\StarStreaksDown1.part
		Part	CustomizeablePowers\Regeneration\Subtractive\StarStreaksRightc1.part
		Sound regen4 60 60 1.0
	End

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Regeneration\Subtractive\tealmist.part
		Part	CustomizeablePowers\Regeneration\Subtractive\CoreRings.part
		Part	CustomizeablePowers\Regeneration\Subtractive\StarStreaksRightb1.part
		Part	CustomizeablePowers\Regeneration\Subtractive\StarStreaksLeftb1.part
		Part	CustomizeablePowers\Regeneration\Subtractive\StarStreaksLeftc1.part
		Sound regen3 60 60 0.8
	End

	Event
		Type	Local
		At	chest
		Part	CustomizeablePowers\Regeneration\Subtractive\SparkliesTrail.part
	End
End

#############################################################

End