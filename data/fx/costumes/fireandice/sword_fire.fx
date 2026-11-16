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
				PositionOffset 0 0.35 0
			End
			Part :Helmet_Fire_Flame_Body.part
			Part :Helmet_Fire_Flame_Body_Additive.part
			Part :Helmet_Fire_Flame_Body_Additive2.part
			Part :Helmet_Fire_Flame_Core.part

		End

	End

###########################################################

End
