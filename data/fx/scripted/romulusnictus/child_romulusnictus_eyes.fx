#########################################################
## Warshade - Eye Glow Continuing FX
########################################################

FxInfo

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	SoundFX
		Type	Local
		At	Root
		BHVR	Behaviors\OffsetEyes.bhvr
		Sound Warshadeeyes3 60 60 .77
	End
End

#Condition
#	On	Time
#	Time	60
#
#	Event
#		EName	2
#		Type	Local
#		At	FootR
#		Sound	blue_loop 60 20 .4
#		bhvr	behaviors\soundFade3.bhvr
#		Lifespan 240
#	End
#
#	Event
#		EName	3
#		Type	Local
#		At	ULEGL
#		Sound	intel_loop 60 20 .33
#		bhvr	behaviors\soundFade3.bhvr
#		Lifespan 240
#	End
#End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Primea
		Type	Local
		At	Eyes
		BHVR	Behaviors\OffsetEyes.bhvr
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	Primea
		AltPiv	1
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_Iris.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_Pupil.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_BLUE.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_PURPLE.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_MistLeftBlack.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_MistLeftPurple.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_MistLeftBlue.part
	End

	Event
		Type	Local
		At	Primea
		AltPiv	2
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_Iris.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_Pupil.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_BLUE.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_PURPLE.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_MistRightBlack.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_MistRightPurple.part
		Part	ENEMYFX\Romans\RomulusNictus\Eyes_MistRightBlue.part
	End
End

#############################################################

End