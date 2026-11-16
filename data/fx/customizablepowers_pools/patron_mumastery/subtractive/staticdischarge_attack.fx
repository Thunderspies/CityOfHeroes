#                                                          
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 60


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName lift
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticSpot.bhvr
			#Geom Testing_Target
			Sound StaticDischarge2 90 90 0.9
			LifeSpan 200
		End

		Event
			At lift
			Type Start
			ChildFx :CHILD_StaticBolt.fx
			LifeSpan 50
		End

		Event
			At lift
			Type Start
			ChildFx :CHILD_StaticBolt.fx
			LifeSpan 50
		End

		Event
			At lift
			Type Start
			ChildFx :CHILD_StaticBolt.fx
			LifeSpan 50
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type PositOnly
			Sound eleccontrol2 100 100 0.5
		End

	End

	Condition
		On Cycle
		Time 7

		Event
			At lift
			Type Start
			ChildFx :CHILD_StaticBolt.fx
			LifeSpan 50
		End

	End

	Condition
		On Cycle
		Time 3

		Event
			At lift
			Type Start
			ChildFx :CHILD_StaticBolt.fx
			LifeSpan 50
		End

	End

	Condition
		On Cycle
		Time 5

		Event
			At lift
			Type Start
			ChildFx :CHILD_StaticBolt.fx
			LifeSpan 50
		End

	End

End
