#
FxInfo

	Input
		InpName Origin
	End

	Input
		InpName Target
	End

	Input
		InpName Mystic
	End

	Input
		InpName HandR
	End


###########################################################

	Condition
		On Time
		Time 10

		Event
			At Origin
			Type Local
			Sound elecattack4 80 80 0.86
		End

	End

	Condition
		On Cycle
		Time 4

		Event
			At Root
			Type Local
			EName ArmR
			Bhvr V_COV\EnemyPowers\Arachnos\mu_ShockHold_GroundR.bhvr
			LifeSpan 1
		End

		Event
			At HandR
			Type PositOnly
			PMagnet HandR
			POther ArmR
			Part :mu_ShockHold_Ground.part
			LifeSpan 40
		End

		Event
			At Root
			Type Local
			EName ArmL
			Bhvr V_COV\EnemyPowers\Arachnos\mu_ShockHold_GroundL.bhvr
			LifeSpan 1
		End

		Event
			At HandL
			Type PositOnly
			PMagnet HandL
			POther ArmL
			Part :mu_ShockHold_Ground.part
			LifeSpan 20
		End

	End

End
