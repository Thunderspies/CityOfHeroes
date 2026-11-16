#                                                          
FxInfo

	Flags DontSuppress


###########################################################

	#Condition
		#On Time
		#Time 0

	#End
Condition
	On 	Time
	Time	4
	Event
		Type	Local
		At	Root
		Sound 	Wodan_Death 100 100 .8
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
			Part :FortifyPack_Glow2_XL.part
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
			Part :FortifyPack_Glow_Head_XL.part
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
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Fore_ULegL
			Type Local
			POther Fore_LlegL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
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
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Hind_ToeR
			Type Local
			POther Hind_FootR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Fore_LlegR
			Type Local
			POther Fore_FootR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Fore_LlegL
			Type Local
			POther Fore_FootL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Hind_FootR
			Type Local
			POther Hind_LLegR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Hind_FootL
			Type Local
			POther Hind_LLegL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Head
			Type Start
			EName Prime
			Flags AdoptParentEntity
			Bhvr :DeathFadeOut.bhvr
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shockwave_Big.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shockwave_Ground_Big.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Flash_Big.part
			LifeSpan 20
		End

		Event
			At Chest
			Type Local
			POther Hips
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

		Event
			At Hips
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

		Event
			At Fore_LlegR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

		Event
			At Fore_LlegL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

		Event
			At Fore_FootR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

		Event
			At Fore_FootL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

		Event
			At Hind_LlegR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

		Event
			At Hind_LlegL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

		Event
			At Hind_FootR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

		Event
			At Hind_FootL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
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
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

		Event
			At Neck
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 0.5
			End
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Shreds.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Bits.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Death_Smoke.part
			Part ENEMYFX\Praetorian\SpiritStalkers\DarkTendrils.part
			LifeSpan 90
		End

	End

End
