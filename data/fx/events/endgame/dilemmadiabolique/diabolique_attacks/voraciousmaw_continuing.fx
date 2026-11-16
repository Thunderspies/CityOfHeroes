#############################################################
## BitterIceBolt.fx
#############################################################

FxInfo

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
			EName	MawAchor
			At Root
			Type Local
			Part	:VoraciousMaw_TarPatch_Core.part
			Sound DD_SoulshatterConditional_Loop 300 300 .8
			bhvr	behaviors/Sound/SoundIn2Out10.bhvr
			lifespan 500
	End

	Event
		Type	Local
		At	Root
		Sound 	DD_SoulshatterPet_Loop 300 100 .8
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 500
	End

	Event
		EName	Sigil
		Type	Start
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			PositionOffset 	0 0 0
			Alpha		255
			StartColor	14 15 20 #100 130 150
		#	PrimaryTint	80
		#	SecondaryTint	20
			Scale		120.0 100.0 120.0
		#	SplatFlags	ADDITIVE
		End
		Splat	OilSplat.tga #Oil_Puddle_01.tga
		lifespan 500
	End


End

Condition
	On	Cycle
	Time	15
	Repeat	1

	Event
			Anim FX_Maw_Tintable
			At Root
			Type Start
			BhvrOverride
				StartColor	1 1 1
				PositionOffset 0 -10 0
				StartJitter	30 1 30
				PYRRotateJitter	0 180 0
				Scale 0.08 0.08 0.08
				AnimScale 0.01
			End
			Sound 	DD_VoraciousMawContinuing_01 100 100 .8
			Sound 	DD_VoraciousMawContinuing_02 100 100 .8
			Sound 	DD_VoraciousMawContinuing_03 100 100 .8
			LifeSpan 80
	End

End

#############################################################

End