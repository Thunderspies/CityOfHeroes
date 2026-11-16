#
FxInfo

	LifeSpan 150


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DNASiphon_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0
		Random 1

		Event
			EName TendrilA
			At Chest
			Type Local
			ChildFX :DnaSiphon_Hit_ChildA.fx
		End

		Event
			EName TendrilA
			At Chest
			Type Local
			ChildFX :DnaSiphon_Hit_ChildB.fx
		End

		Event
			EName TendrilA
			At Chest
			Type Local
			ChildFX :DnaSiphon_Hit_ChildC.fx
		End

		Event
			EName TendrilA
			At Chest
			Type Local
			ChildFX :DnaSiphon_Hit_ChildD.fx
		End

		Event
			EName TendrilA
			At Chest
			Type Local
			ChildFX :DnaSiphon_Hit_ChildE.fx
		End

		Event
			EName TendrilA
			At Chest
			Type Local
			ChildFX :DnaSiphon_Hit_ChildF.fx
		End

	End

End
