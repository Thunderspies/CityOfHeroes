#
FxInfo

	Input
		InpName Hips
	End


###########################################################

	Condition
		On Time
		Time 0

		Event
			At UArmL
			Type Local
			POther Neck
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.3 0
			End
			Part :Plasma_Core.part
			Part :Plasma_Highlight.part
			Part :Plasma_black.part
		End

		Event
			At UArmR
			Type Local
			POther Neck
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.0 -0.3 0
			End
			Part :Plasma_Core.part
			Part :Plasma_Highlight.part
			Part :Plasma_black.part
		End

		Event
			At Hips
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.4 0
			End
			Part :Plasma_Core.part
			Part :Plasma_Highlight.part
			Part :Plasma_black.part
		End

		Event
			At Hips
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.7 0
			End
			Part IncarnatePowers\Interface\Plasma_CoreA.part
		End

	End

End
