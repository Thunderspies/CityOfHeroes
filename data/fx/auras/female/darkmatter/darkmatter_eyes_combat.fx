#                                                          
FxInfo


###########################################################

	Condition
		On TriggerBits
		TriggerBits COMBAT

		Event
			At Root
			Type Local
			ChildFx :DarkMatter_Eyes.fx
			EName CombatFX
			Flags OneAtaTime
			While COMBAT
		End

	End

End
