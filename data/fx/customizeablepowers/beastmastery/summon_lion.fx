#
FxInfo

	LifeSpan 180


###########################################################

Condition
	On 	Time
	Time	8
	Event
		Type	Local
		At	Root
		Sound 	BM_SummonLions_01 100 100 .8
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
		Time 8

		Event
			AltPiv 3
			At MussleFlashRigging
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.250000 0.38
			End
			Part :Train_Wind_Streak.part
			Part :Train_Wind_Vertical.part
			LifeSpan 10
		End

		Event
			AltPiv 3
			At MussleFlashRigging
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.3200000 0
			End
			Part :TrainerShoutCore.part
			Part :TrainerShoutRing.part
			Part :Trainer_Steam_Plume.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 40

		Event
			AltPiv 3
			At MussleFlashRigging
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.250000 0.38
			End
			Part :Train_Wind_Streak.part
			Part :Train_Wind_Vertical.part
			LifeSpan 20
		End

		Event
			AltPiv 3
			At MussleFlashRigging
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.3200000 0
			End
			Part :TrainerShoutCore.part
			Part :TrainerShoutRing.part
			Part :Trainer_Steam_Plume.part
			LifeSpan 20
		End

	End

End
