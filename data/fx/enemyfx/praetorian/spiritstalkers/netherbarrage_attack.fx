#
FxInfo

	LifeSpan 250


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SpiritStalkers_NetherBarrage_Attack 100 100 .8
	End
End

	Condition
		On Time
		Time 20

		Event
			At Head
			Type Local
			ChildFX :NetherBarrage_Child.fx
			Lifespan 60
		End

	End

############################################



End
