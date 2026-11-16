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
		Sound heal1_loop 60 60 0.45
		Bhvr	Behaviors\soundFade1.bhvr
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
		Part	:core4.part
		Part	:Electrons.part
		Part	:StarStreaks1.part
		Part	:StarStreaksDown1.part
		Part	:StarStreaksRightc1.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound regen4 60 60 1.0
	End

	Event
		Type	Local
		At	chest
		Part	:tealmist.part
		Part	:CoreRings.part
		Part	:StarStreaksRightb1.part
		Part	:StarStreaksLeftb1.part
		Part	:StarStreaksLeftc1.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound regen3 60 60 0.8
	End

	Event
		EName	dummy2
		Type	Local
		At	chest
		Part	:SparkliesTrail.part
	End
End

#############################################################

End
