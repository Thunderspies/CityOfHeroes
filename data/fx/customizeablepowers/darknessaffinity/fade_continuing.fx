#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName MainAnchor
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.2 0
			End
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName FadeToBlack
			Flags AdoptParentEntity
			BhvrOverride
				Alpha 80
			End
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type SetLight
			EName FadeToBlack
			Bhvr CustomizeablePowers\DarknessAffinity\Entity_Darken_Fade.bhvr
			LifeSpan 30
		End

	End

	Condition
		On Cycle
		Time 2

		Event
			Type SetLight
			EName Black
			Bhvr CustomizeablePowers\DarknessAffinity\Entity_Darken.bhvr
			LifeSpan 10
		End

	End

	Condition
		On TriggerBits
		TriggerBits MALE
		DoMany 1

		Event
			At Head
			Type Local
			ChildFx CustomizeablePowers\DarknessControl\CHILD_Possess_Eyes_Male.fx
			EName EyeGlows_Male
			Flags OneAtATime
		End

	End

	Condition
		On TriggerBits
		TriggerBits FEMALE
		DoMany 1

		Event
			At Head
			Type Local
			ChildFx CustomizeablePowers\DarknessControl\CHILD_Possess_Eyes_Female.fx
			EName EyeGlows_Female
			Flags OneAtATime
		End

	End

	Condition
		On TriggerBits
		TriggerBits HUGE
		DoMany 1

		Event
			At Head
			Type Local
			ChildFx CustomizeablePowers\DarknessControl\CHILD_Possess_Eyes_Huge.fx
			EName EyeGlows_Huge
			Flags OneAtATime
		End

	End

End
