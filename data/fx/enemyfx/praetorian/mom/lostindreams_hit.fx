#
FxInfo

	LifeSpan 240


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			#Sound Sewer_ToxicGasInward_01 100 100 0.8
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName GasIn
			BhvrOverride
				PositionOffset 0 3 0
			End
			Part :Coalescing_Smoke.part
			Part :Coalescing_Smoke_Hilight.part
			LifeSpan 240
		End

	End

End
