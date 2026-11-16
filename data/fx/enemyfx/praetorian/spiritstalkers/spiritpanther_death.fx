#
FxInfo

	Flags DontSuppress


###########################################################

#	Condition
#		On Time
#		Time 0
#	End

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SpiritPanther_Death 100 100 .8
	End
End


	Condition
		On Time
		Time 30

		Event
			At Chest
			Type Local
			POther Hips
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow2.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 37

		Event
			At Head
			Type Local
			POther Neck
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_Head_Big.part
			LifeSpan 15
		End
	End

	Condition
		On Time
		Time 43

		Event
			At Fore_ULegR
			Type Local
			POther Fore_LlegR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_Big.part
			LifeSpan 20
		End

		Event
			At Fore_ULegL
			Type Local
			POther Fore_LlegL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_Big.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 50

		Event
			At Hind_ToeL
			Type Local
			POther Hind_FootL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_Big.part
			LifeSpan 20
		End

		Event
			At Hind_ToeR
			Type Local
			POther Hind_FootR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_Big.part
			LifeSpan 20
		End

		Event
			At Fore_LlegR
			Type Local
			POther Fore_FootR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_Big.part
			LifeSpan 20
		End

		Event
			At Fore_LlegL
			Type Local
			POther Fore_FootL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_Big.part
			LifeSpan 20
		End

		Event
			At Hind_FootR
			Type Local
			POther Hind_LLegR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_Big.part
			LifeSpan 20
		End

		Event
			At Hind_FootL
			Type Local
			POther Hind_LLegL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_Big.part
			LifeSpan 20
		End

		Event
			EName 	Prime
			Type	start
			At	Head
			Bhvr	:DeathFadeOut.bhvr
			Flags	AdoptParentEntity
			Lifespan	1
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Chest
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			LifeSpan 90
		End

		Event
			At Hips
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			LifeSpan 90
		End

		Event
			Type Local
			At Fore_FootR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			LifeSpan 90
		End

		Event
			Type Local
			At Fore_FootL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			LifeSpan 90
		End

		Event
			At Hind_FootR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			LifeSpan 90
		End

		Event
			At Hind_FootL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			LifeSpan 90
		End

		Event
			At Head
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 0.5
			End
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			LifeSpan 90
		End

	End

End
