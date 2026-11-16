#
FxInfo

	#LifeSpan 200


###########################################################

	Condition
		On Time
		Time 0

		Event
			At UArmL
			Type PositOnly
			POther Head
			Part :BodySmoke_Smoke.part
		End

		Event
			At Head
			Type PositOnly
			Part :BodySmoke_Smoke.part
		End

		Event
			At WepR
			Type PositOnly
			POther LArmR
			Part :BodySmoke_Smoke.part
		End

		Event
			At WepL
			Type PositOnly
			POther LArmL
			Part :BodySmoke_Smoke.part
		End

		Event
			At LLegR
			Type PositOnly
			POther ULegR
			Part :BodySmoke_Smoke.part
		End

		Event
			At LLegL
			Type PositOnly
			POther ULegR
			Part :BodySmoke_Smoke.part
		End

	End

End
