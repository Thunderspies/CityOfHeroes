###########################################################

FxInfo

	Flags DontSuppress

###########################################################

	Condition
		On Time
		Time 0

		Event
			Ename HeadAnchor
			At Head
			Type Local
			POther 0
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			Part :Helmet_Ice_Core.part
			Part :Helmet_Ice_Steam.part
			Part :Helmet_Ice_Highlights.part
			Part :Helmet_Ice_Base.part

		End

	End

###########################################################

End
