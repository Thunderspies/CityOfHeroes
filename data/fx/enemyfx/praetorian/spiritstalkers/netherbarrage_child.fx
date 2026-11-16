#
FxInfo

	#LifeSpan 250


###########################################################

	Condition
		On Cycle
		Time 5

		Event
			At Origin
			Type Local
			ChildFX :NetherBarrage_Grandchild.fx
			Lifespan 60
		End

	End

End