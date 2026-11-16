#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName generator
			BhvrOverride
				PositionOffset 0 0 0
			End
			#Geom _madscience_device_02
		End

		Event
			At generator
			Type Local
			EName generator2
			BhvrOverride
				PositionOffset 0.600000 1 0
			End
		End

	End

	Condition
		On Cycle
		Time 15
		Chance 0.5000

		Event
			At generator2
			Type Local
			EName CylinderArcsUp
			Part WORLD\City\Science_Lab\Electric_CylinderGlow.part
			Part WORLD\City\Science_Lab\Electric_VerticalStream.part
			LifeSpan 5
		End

		Event
			At generator2
			Type Local
			EName CylinderArcsDown
			BhvrOverride
				PositionOffset 0 1.8 0
			End
			Part WORLD\City\Science_Lab\Electric_VerticalStream_Down.part
			LifeSpan 5
		End

	End

	Condition
		On Cycle
		Time 5
		Chance 0.2000
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	EncasedGenerator_Arcs1_01 15 15 .3
		Sound 	EncasedGenerator_Arcs1_02 15 15 .3
		Sound 	EncasedGenerator_Arcs1_03 15 15 .3
		Sound 	EncasedGenerator_Arcs1_04 15 15 .3
		Sound 	EncasedGenerator_Arcs1_05 15 15 .3
		lifespan 1
	End

		Event
			At generator
			Type Local
			EName SparkArcs
			BhvrOverride
				PositionOffset 0.6 3.9 0.370000
			End
			Part WORLD\City\Science_Lab\electric_Arc_XSmall.part
			LifeSpan 15
			LifeSpanJitter 5.0000
		End

	End

	Condition
		On Cycle
		Time 5
		Chance 0.2000
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	EncasedGenerator_Arcs2_01 15 15 .3
		Sound 	EncasedGenerator_Arcs2_02 15 15 .3
		Sound 	EncasedGenerator_Arcs2_03 15 15 .3
		Sound 	EncasedGenerator_Arcs2_04 15 15 .3
		Sound 	EncasedGenerator_Arcs2_05 15 15 .3
		lifespan 1
	End

		Event
			At generator
			Type Local
			EName SparkArcs2
			BhvrOverride
				PositionOffset 0.6 3.9 -0.3700000
			End
			Part WORLD\City\Science_Lab\electric_Arc_XSmall.part
			LifeSpan 15
			LifeSpanJitter 5.0000
		End

	End

End
