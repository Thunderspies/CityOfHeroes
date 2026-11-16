#
FxInfo

	LifeSpan 70


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type PositOnly
			EName Prime
			Bhvr IncarnatePowers\Hybrid\ExpandingDust.bhvr
			BhvrOverride
				EndScale 2 2 2
			End
			Part :Dust_out.part
			Part :Dust_Out_LocalFacing.part
			Part :DustRing_Out.part
		End

		Event
			At Chest
			Type PositOnly
			EName Prime2
			Bhvr IncarnatePowers\Hybrid\AccumulatingDust.bhvr
			Part :DustRing_In.part
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Chest
			Type PositOnly
			EName Prime
			Bhvr IncarnatePowers\Hybrid\ExpandingDust.bhvr
			BhvrOverride
				EndScale 2 2 2
			End
			Part :Dust_out.part
			Part :Dust_Out_LocalFacing.part
			Part :DustRing_Out.part
		End

		Event
			At Chest
			Type PositOnly
			EName Prime2
			Bhvr IncarnatePowers\Hybrid\AccumulatingDust.bhvr
			Part :DustRing_In.part
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Chest
			Type PositOnly
			EName Prime
			Bhvr IncarnatePowers\Hybrid\ExpandingDust.bhvr
			BhvrOverride
				EndScale 2 2 2
			End
			Part :Dust_out.part
			Part :Dust_Out_LocalFacing.part
			Part :DustRing_Out.part
		End

		Event
			At Chest
			Type PositOnly
			EName Prime2
			Bhvr IncarnatePowers\Hybrid\AccumulatingDust.bhvr
			Part :DustRing_In.part
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Chest
			Type PositOnly
			EName Prime
			Bhvr IncarnatePowers\Hybrid\ExpandingDust.bhvr
			BhvrOverride
				EndScale 2 2 2
			End
			Part :Dust_out.part
			Part :Dust_Out_LocalFacing.part
			Part :DustRing_Out.part
		End

		Event
			At Chest
			Type PositOnly
			EName Prime2
			Bhvr IncarnatePowers\Hybrid\AccumulatingDust.bhvr
			Part :DustRing_In.part
		End

	End

End
