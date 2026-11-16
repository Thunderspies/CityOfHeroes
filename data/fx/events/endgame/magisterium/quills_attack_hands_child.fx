#
FxInfo

	LifeSpan 70


###########################################################

	Condition
		On Time
		Time 0

		Event
			At UArmL
			Type Local
			LookAt LarmL
			Magnet LarmL
			POther LarmL
			Part :ArmGlow_Core_Add.part
			LifeSpan 30
		End

		Event
			At UArmR
			Type Local
			LookAt LarmR
			Magnet LarmR
			POther LarmR
			Part :ArmGlow_Core_Add.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 10

		Event
			At LArmL
			Type Local
			LookAt HandL
			Magnet HandL
			POther HandL
			Part :ArmGlow_Core_Add.part
			LifeSpan 30
		End

		Event
			At LArmR
			Type Local
			LookAt HandR
			Magnet HandR
			POther HandR
			Part :ArmGlow_Core_Add.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 20

		Event
			At WepL
			Type Local
			Part :HandGlow_Core.part
			Part :HandGlow_Core_Add.part
			Part :WellDustOut_Hands.part
			LifeSpan 30
		End

		Event
			At WepR
			Type Local
			Part :HandGlow_Core.part
			Part :HandGlow_Core_Add.part
			Part :WellDustOut_Hands.part
			LifeSpan 30
		End

	End

End
