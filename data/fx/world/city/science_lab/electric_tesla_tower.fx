#
FxInfo


###########################################################

	Condition
		On Time
		Time 0
	Event
		Type	Local
		At	Root
		Sound 	Tesla_Loop 35 35 .2
	End

		Event
			At Root
			Type Local
			EName generator
			BhvrOverride
				#PositionOffset 0 0 0
			End
			#Geom madscience_tesla_coil_01
		End

		Event
			At generator
			Type Local
			EName generator2
		End

	End

	Condition
		On Cycle
		Time 15
		Chance 0.5000
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	Tesla_01 35 35 .6
		Sound 	Tesla_02 35 35 .6
		Sound 	Tesla_03 35 35 .6
		Sound 	Tesla_04 35 35 .6
		Sound 	Tesla_05 35 35 .6
		lifespan 1
	End

		Event
			At generator2
			Type Local
			EName ArcCenter
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part WORLD\City\Science_Lab\electric_Arc_Big.part
			LifeSpan 5
		End

	End

End
