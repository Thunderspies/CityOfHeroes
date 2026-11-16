#
FxInfo

	LifeSpan 150


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BM_TameBeasts_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName rigging
			Geom PsiTele02
		End

		Event
			At Head
			Type Local
			EName MussleFlashRigging
			Geom PsionicRigging
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Chest
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Glow_Chest.part
			Part :TrainerCharge_Glow_Chest.part
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 26

		Event
			At Head
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Ripples_Head.part
			Part :TrainerCharge_Ripples_Chest.part
			LifeSpan 50
		End

		Event
			At JAW
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Glow_Head.part
			Part :TrainerCharge_Glow_Head.part
			LifeSpan 50
		End

		Event
			At UArmL
			Type Local
			POther LarmL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Glow.part
			LifeSpan 50
		End

		Event
			At UArmR
			Type Local
			POther LarmR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Glow.part
			LifeSpan 50
		End

		Event
			At LArmR
			Type Local
			POther WepR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Glow.part
			LifeSpan 50
		End

		Event
			At LArmL
			Type Local
			POther WepL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Glow.part
			LifeSpan 50
		End

		Event
			At WepR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Glow_Head.part
			LifeSpan 50
		End

		Event
			At WepL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Glow_Head.part
			LifeSpan 50
		End

	End

	Condition
		On Time
		Time 30

		Event
			AltPiv 3
			At MussleFlashRigging
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.250000 0.38
			End
			Part :Tame_Wind_Streak.part
			Part :Tame_Wind_Vertical.part
			LifeSpan 30
		End

		Event
			AltPiv 3
			At MussleFlashRigging
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.3200000 0
			End
			Part :TrainerShoutCore2.part
			Part :TrainerShoutRing2.part
			Part :Trainer_Steam_Plume_Large.part
			LifeSpan 35
		End

	End

	Condition
		On Time
		Time 45

		Event
			At Head
			Type Local
			ChildFx :CHILD_TameBeasts_Trainer.fx
			EName EyeAnchor
			Bhvr behaviors/GenericParticleFade.bhvr
			LifeSpan 20
		End

	End

End
