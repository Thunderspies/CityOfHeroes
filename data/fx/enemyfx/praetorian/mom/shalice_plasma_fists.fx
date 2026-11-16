#
FxInfo

	Flags InheritAlpha

	Input
		InpName Hips
	End


###########################################################

	Condition
		On Time
		Time 0

		Event
			At HandL
			Type Local
			POther LArmL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Plasma_Hand_Glow.part
			Part :Plasma_Core.part
			Part :Plasma_Long.part
			Part :Plasma_Highlight_Hands.part
		End

		Event
			At HandR
			Type Local
			POther LArmR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Plasma_Hand_Glow.part
			Part :Plasma_Core.part
			Part :Plasma_Long.part
			Part :Plasma_Highlight_Hands.part
		End

	End

End
