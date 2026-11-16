#                                                          
FxInfo

	LifeSpan 500


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			EName Hookup
			Sound BM_SummonSpears_Attack 200 200 0.9
		End

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0 1.5 0
			End
			Part ENEMYFX\Praetorian\BattleMaiden\DarkLocator_Shockwave.part
		End

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0.0 0.2 0.0
			End
			Part ENEMYFX\Praetorian\BattleMaiden\DarkLocator_Shockwave_Small.part
		End

	End

	Condition
		On Cycle
		Time 5

		Event
			At Hookup
			Type Local
			Bhvr behaviors/powers/firecontrol/Infernobubble4.bhvr
			BhvrOverride
				PositionOffset 0 1.5 0
				Scale 0.4 0.4 0.4
				ScaleRate 1.05 1.05 1.05
				EndScale 1.5 1.5 1.5
			End
			Part ENEMYFX\Praetorian\BattleMaiden\DarkLocator_DustRing.part
			Part ENEMYFX\Praetorian\BattleMaiden\DarkLocator_DustRing_Highlight.part
			LifeSpan 15
		End

	End

End
