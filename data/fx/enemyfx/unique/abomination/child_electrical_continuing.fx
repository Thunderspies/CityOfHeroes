#
FxInfo


###########################################################
Condition
	Event
		Type	Local
		At	Root
		Sound 	Sewer_ElectricBurstLow_Loop 100 100 .5
	End
End

	Condition
		On Cycle
		Time 60
		Repeat 1
		Random 1

		Event
			At Root
			Type Local
			ChildFx :GRANDCHILD_Electrical_Continuing.fx
			EName sparks
			SoundNoRepeat 3
			Sound EC_TeslaCage_Hand_01 50 50 .5
			Sound EC_TeslaCage_Hand_02 50 50 .5
			Sound EC_TeslaCage_Hand_03 50 50 .5
			Sound EC_TeslaCage_Hand_04 50 50 .5
			Sound EC_TeslaCage_Hand_05 50 50 .5
			Sound EC_TeslaCage_Hand_06 50 50 .5
			LifeSpan 60
		End

	End

End
