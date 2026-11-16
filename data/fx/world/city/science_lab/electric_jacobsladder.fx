#
FxInfo


###########################################################


	Condition
		On Time
		Time 0
	Event
		Type	Local
		At	Root
		Sound 	JacobsLadder_Loop 35 35 .1
	End

		Event
			At Root
			Type Local
			EName generator
			BhvrOverride
				#PositionOffset 0 0 0
			End
			#Geom madscience_tesla_coil_02
		End

		Event
			At generator
			Type Local
			EName JacobsAnchor
			BhvrOverride
				PositionOffset 0 9.4 0
			End
		End

	End

	Condition
		On Cycle
		Time 10
		Chance 0.3000
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	JacobsLadder_01 35 35 .5
		Sound 	JacobsLadder_02 35 35 .5
		Sound 	JacobsLadder_03 35 35 .5
		Sound 	JacobsLadder_04 35 35 .5
		Sound 	JacobsLadder_05 35 35 .5
		lifespan 1
	End
		Event
			At JacobsAnchor
			Type Local
			BhvrOverride
				PyrRotate 90 90 0
			End
			Part WORLD\City\Science_Lab\Tesla_Antennas.part
			LifeSpan 15
		End

		Event
			At Root
			Type Local
			EName SOUND_COULD_GO_HERE_TO_SYNC_WITH_FX
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 5
		Chance 0.2000

		Event
			At generator
			Type Local
			EName SparkArcs
			BhvrOverride
				PositionOffset 0 9.500000 0
				StartJitter 1 0.8 1
			End
			Part WORLD\City\Science_Lab\electric_Sparks.part
			Part WORLD\City\Science_Lab\electric_Sparks2.part
			Part WORLD\City\Science_Lab\Electric_SparkGlow.part
			Part WORLD\City\Science_Lab\electric_Arc_Small.part
			LifeSpan 15
			LifeSpanJitter 5.0000
		End

	End

End
