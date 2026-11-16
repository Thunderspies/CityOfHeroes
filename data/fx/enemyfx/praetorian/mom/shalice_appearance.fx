#
FxInfo

	LifeSpan 200


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MoM_ShaliceAppearance_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			#Sound Sewer_ToxicGasInward_01 100 100 0.8
			LifeSpan 1
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
			CHILDFX	GENERIC\ENTITYVANISH.FX
			Part :Coalescing_Smoke.part
			Part :Coalescing_Smoke_Hilight.part
			LifeSpan 120
		End

	End

End
