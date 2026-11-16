#
FxInfo

	LifeSpan 600


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	NA_Rebirth_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			ChildFx :Regrowth_Activation.fx
		End

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 -0.3 0
			End
			Part :Regrowth_Grass_Single.part
		End

		Event
			At Chest
			Type PositOnly
			BhvrOverride
				PyrRotate 0 0 15
			End
			Part CustomizeablePowers\PlantControl\NatureAffinity\Rebirth_Pollen.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Rebirth_Leaves.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Rebirth_Stars.part
			LifeSpan 165
		End
	End

	Condition
		On Time
		Time 0
		#Chance 0.8

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	2 1.5 2
				PositionOffset	2 0 2
				StartJitter	1 0 1
				PyrRotateJitter 0 180 0
				FadeInLength	120
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			geom FX_Bush_AzMedA
			geom FX_Bush_AzSmBFol
			geom FX_Bush_AzLgAFol
			geom FX_Bush_AzSmA
			Lifespan 75
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	1.5 1 1.5
				PositionOffset	-2 0 -2
				StartJitter	1 0 1
				PyrRotateJitter 0 180 0
				FadeInLength	120
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			geom FX_Bush_AzMedA
			geom FX_Bush_AzSmBFol
			geom FX_Bush_AzLgAFol
			geom FX_Bush_AzSmA
			Lifespan 75
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	1.5 1 1.5
				PositionOffset	2 0 -2
				StartJitter	1 0 1
				PyrRotateJitter 0 180 0
				FadeInLength	120
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			geom FX_Bush_AzMedA
			geom FX_Bush_AzSmBFol
			geom FX_Bush_AzLgAFol
			geom FX_Bush_AzSmA
			Lifespan 75
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	1 0.75 1
				PositionOffset	-2 0 2
				StartJitter	1 0 1
				PyrRotateJitter 0 180 0
				FadeInLength	120
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			geom FX_Bush_AzMedA
			geom FX_Bush_AzSmBFol
			geom FX_Bush_AzLgAFol
			geom FX_Bush_AzSmA
			Lifespan 75
		End


	End

End
