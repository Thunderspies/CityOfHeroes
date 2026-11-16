#
FxInfo

	Input
		InpName Chest
	End

	LifeSpan 50


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Bhvr Behaviors\CameraShake01Subtle.bhvr
			Part :MaceFlash.part
			Part :MaceHitGlow01Sml.part
			Part :MaceHitStreaks.part
			Sound Axe5 80.0 80.0 0.8
			LifeSpan 2
		End

		Event
			At Chest
			Type Local
			Part :MartialArtsHit.part
			Part :MartialArtsHit1.part
			Part :MartialArtsHit2.part
		End

		Event
			At Chest
			Type Local
			Part :MAHitStar01Sml.part
			Part :MAHitStar02Sml.part
			Part :MAHitStar03Sml.part
			Part :MAHitRays01Sml.part
		End

		Event
			At Chest
			Type Local
			SoundNoRepeat 3
			Sound N_Hit_01 100 100 0.8
			Sound N_Hit_02 100 100 0.8
			Sound N_Hit_03 100 100 0.8
			Sound N_Hit_04 100 100 0.8
			Sound N_Hit_05 100 100 0.8
			Sound N_Hit_06 100 100 0.8
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Neck
			Type Local
			POther Waist
			Part :ShatterCracks01.part
			LifeSpan 1
		End

		Event
			At Chest
			Type Local
			POther Hips
			Part :ShatterCracks01.part
			LifeSpan 1
		End

		Event
			At UArmL
			Type Local
			POther LArmL
			Part :ShatterCracks01.part
			LifeSpan 1
		End

		Event
			At UArmR
			Type Local
			POther LArmR
			Part :ShatterCracks01.part
			LifeSpan 1
		End

		Event
			At LArmL
			Type Local
			POther WepL
			Part :ShatterCracks01.part
			LifeSpan 1
		End

		Event
			At LArmR
			Type Local
			POther WepR
			Part :ShatterCracks01.part
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Neck
			Type Local
			POther Waist
			Part :ShatterFlash01.part
			Part :ShatterParticles01.part
			Sound ShatterArmor1 100.0 100.0 1.0
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 22

		Event
			At Chest
			Type Local
			POther Hips
			Part :ShatterFlash01.part
			Part :ShatterParticles01.part
			LifeSpan 2
		End

		Event
			At UArmL
			Type Local
			POther LArmL
			Part :ShatterFlash01.part
			Part :ShatterParticles01.part
			LifeSpan 2
		End

		Event
			At UArmR
			Type Local
			POther LArmR
			Part :ShatterFlash01.part
			Part :ShatterParticles01.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 24

		Event
			At LArmL
			Type Local
			POther WepL
			Part :ShatterFlash01.part
			Part :ShatterParticles01.part
			LifeSpan 2
		End

		Event
			At LArmR
			Type Local
			POther WepR
			Part :ShatterFlash01.part
			Part :ShatterParticles01.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 0
		Repeat 8
		RepeatJitter 2
		Chance 0.6

		Event
			At Hips
			Type Start
			ChildFx CustomizablePowers_Pools\Patron_MaceMastery\MetalSparkEXPLODEUP.fx
			EName Sparkup
		End

	End

	Condition
		On Time
		Time 0
		Repeat 5
		RepeatJitter 1

		Event
			At Chest
			Type Start
			ChildFx CustomizablePowers_Pools\Patron_MaceMastery\MetalSparkUP.fx
			EName Sparkdown
		End

	End

	Condition
		On Time
		Time 0
		Repeat 7
		RepeatJitter 6

		Event
			At Chest
			Type Start
			ChildFx CustomizablePowers_Pools\Patron_MaceMastery\MetalSparkEXPLODE.fx
			EName Sparkfar
		End

	End

	Condition
		On Time
		Time 0
		Repeat 6
		RepeatJitter 4

		Event
			At Chest
			Type Start
			ChildFx CustomizablePowers_Pools\Epic_WeaponMastery\MetalSparkOUT.fx
			EName Sparkout
		End

	End

	Condition
		On Time
		Time 0
		Repeat 10
		RepeatJitter 2

		Event
			At Chest
			Type Start
			ChildFx CustomizablePowers_Pools\Patron_MaceMastery\MetalSparkExplode.fx
			EName Sparkfar
			HardwareOnly
		End

		Event
			At Chest
			Type Start
			ChildFx CustomizablePowers_Pools\Epic_WeaponMastery\MetalSparkOUT.fx
			EName Sparkfar
			HardwareOnly
		End

	End

End
