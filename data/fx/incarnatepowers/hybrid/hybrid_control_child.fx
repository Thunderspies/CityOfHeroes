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
			Part IncarnatePowers\Hybrid\ArmGlow_Core_Add.part
			LifeSpan 30
		End

		Event
			At UArmR
			Type Local
			LookAt LarmR
			Magnet LarmR
			POther LarmR
			Part IncarnatePowers\Hybrid\ArmGlow_Core_Add.part
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
			Part IncarnatePowers\Hybrid\ArmGlow_Core_Add.part
			LifeSpan 30
		End

		Event
			At LArmR
			Type Local
			LookAt HandR
			Magnet HandR
			POther HandR
			Part IncarnatePowers\Hybrid\ArmGlow_Core_Add.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 20

		Event
			At WepL
			Type Local
			Part IncarnatePowers\Hybrid\HandGlow_Core.part
			Part IncarnatePowers\Hybrid\HandGlow_Core_Add.part
			Part IncarnatePowers\Hybrid\WellDustOut_Hands.part
			LifeSpan 30
		End

		Event
			At WepR
			Type Local
			Part IncarnatePowers\Hybrid\HandGlow_Core.part
			Part IncarnatePowers\Hybrid\HandGlow_Core_Add.part
			Part IncarnatePowers\Hybrid\WellDustOut_Hands.part
			LifeSpan 30
		End

	End

End
