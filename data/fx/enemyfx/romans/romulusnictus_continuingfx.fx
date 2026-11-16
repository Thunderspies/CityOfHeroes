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

Condition
	On	Time
	Time	60

	Event
		EName	2
		Type	Local
		At	FootR
		Sound blue_loop 60 60 .4
		bhvr	behaviors\soundFade3.bhvr
		Lifespan 240
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		Sound intel_loop 60 60 .33
		bhvr	behaviors\soundFade3.bhvr
		Lifespan 240
	End
End

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

#Condition
#
#	On	Time
#	Time	20
#
#	Event
#		Type	Local
#		At	Eyes
#		Sound	TransBurst 70 60 .55
#	End
#End

Condition
	On	Time
	Time	0

#	Event
#		Type	Local
#		At	Head
#		Part	ENEMYFX\Romans\RomulusNictus\BlueTendrils.part
#		Part	ENEMYFX\Romans\RomulusNictus\PurpleTendrils.part
#		Part	ENEMYFX\Romans\RomulusNictus\BlackTendrils.part
#		pOther	Neck
#	End

	Event
		Type	Local
		At	SpadL
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils2.part
		pOther	SpadL
	End

	Event
		Type	Local
		At	SpadR
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils2.part
		pOther	SpadR
	End

	Event
		Type	Local
		At	UArmL
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils2.part
		pOther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils2.part
		pOther	LArmR
	End

	Event
		Type	Local
		At	LArmL
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils.part
		pOther	HandL
	End

	Event
		Type	Local
		At	LArmR
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils.part
		pOther	HandR
	End

	Event
		Type	Local
		At	WepL
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils.part
		pOther	WepL
	End

	Event
		Type	Local
		At	WepR
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils.part
		pOther	WepR
	End

	Event
		Type	Local
		At	ULEGL
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils2.part
		pOther	LLEGL
	End

	Event
		Type	Local
		At	ULEGR
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils2.part
		pOther	LLEGR
	End

	Event
		Type	Local
		At	LLEGL
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils.part
		pOther	FootL
	End

	Event
		Type	Local
		At	LLEGR
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils.part
		pOther	FootR
	End

	Event
		Type	Local
		At	ToeL
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils.part
		pOther	Footl
	End

	Event
		Type	Local
		At	ToeR
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils.part
		pOther	FootR
	End
End

#############################################################

End