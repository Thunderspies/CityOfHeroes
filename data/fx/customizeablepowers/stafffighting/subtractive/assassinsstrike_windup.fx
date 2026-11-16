#
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 36

		Event
			At C_Chest
			Type Local
			Bhvr	Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 3
				PyrRotate 0 90 0
			End
			Part :WaveyLines_Shrink.part
			LifeSpan 35
		End

		Event
			At C_Chest
			Type Local
			Bhvr	Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 3
				PyrRotate 0 -90 0
			End
			Part :WaveyLines_Shrink.part
			LifeSpan 35
		End

		Event
			At C_Chest
			Type Local
			Bhvr	Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 3
				PyrRotate 90 90 0
			End
			Part :WaveyLines_Shrink.part
			LifeSpan 35
		End

		Event
			At C_Chest
			Type Local
			Bhvr	Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 3
				PyrRotate -90 -90 0
			End
			Part :WaveyLines_Shrink.part
			LifeSpan 35
		End

	End

End
