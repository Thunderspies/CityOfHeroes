#
FxInfo

	LifeSpan 180


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BM_TrainBeasts_Attack_01 100 100 .8
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
		Time 15

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

End
