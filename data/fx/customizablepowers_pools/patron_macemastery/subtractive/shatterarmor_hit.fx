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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MaceFlash.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MaceHitGlow01Sml.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MaceHitStreaks.part
			Sound Axe5 80.0 80.0 0.8
			LifeSpan 2
		End

		Event
			At Chest
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MartialArtsHit.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MartialArtsHit1.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MartialArtsHit2.part
		End

		Event
			At Chest
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MAHitStar01Sml.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MAHitStar02Sml.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MAHitStar03Sml.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MAHitRays01Sml.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterCracks01.part
			LifeSpan 1
		End

		Event
			At Chest
			Type Local
			POther Hips
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterCracks01.part
			LifeSpan 1
		End

		Event
			At UArmL
			Type Local
			POther LArmL
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterCracks01.part
			LifeSpan 1
		End

		Event
			At UArmR
			Type Local
			POther LArmR
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterCracks01.part
			LifeSpan 1
		End

		Event
			At LArmL
			Type Local
			POther WepL
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterCracks01.part
			LifeSpan 1
		End

		Event
			At LArmR
			Type Local
			POther WepR
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterCracks01.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterFlash01.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterParticles01.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterFlash01.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterParticles01.part
			LifeSpan 2
		End

		Event
			At UArmL
			Type Local
			POther LArmL
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterFlash01.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterParticles01.part
			LifeSpan 2
		End

		Event
			At UArmR
			Type Local
			POther LArmR
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterFlash01.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterParticles01.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterFlash01.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterParticles01.part
			LifeSpan 2
		End

		Event
			At LArmR
			Type Local
			POther WepR
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterFlash01.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ShatterParticles01.part
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
			ChildFx CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MetalSparkEXPLODEUP.fx
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
			ChildFx CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MetalSparkUP.fx
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
			ChildFx CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MetalSparkEXPLODE.fx
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
			ChildFx CustomizablePowers_Pools\Epic_WeaponMastery\Subtractive\MetalSparkOUT.fx
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
			ChildFx CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MetalSparkExplode.fx
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
