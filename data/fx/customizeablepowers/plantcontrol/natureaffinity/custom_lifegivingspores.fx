#
FxInfo

	LifeSpan 170


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 		NA_LifegivingSpores_01 100 100 .8
	End
End






	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			Part :LifeSpore_Tintable_GrassRing.part
			Part :LifeSpore_Tintable_GrassOuterRing.part
			Part :LifeSpore_Tintable_Grass.part
			LifeSpan 170
		End

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part :LifeSpore_Tintable_Glow_Dark.part
			Part :LifeSpore_Tintable_Glow.part
			LifeSpan 170
		End

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0 2 0
			End
			Part :WildGrowth_Specks.part
			LifeSpan 170
		End

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part :LifeSpore_Tintable_Stars.part
			Part :LifeSpore_Tintable_Spores.part
			LifeSpan 170
		End

		Event
			At Root
			Type Local
			EName Anchor
			BhvrOverride
				PyrRotateJitter 0 180 0
				Spin 0 0.2 0
			End
			LifeSpan 30
		End

		Event
			At Anchor
			Type Local
			EName SpinAnchor
			BhvrOverride
				PositionOffset 12 0 0
			End
			Part :LifeSpore_Tintable_Mush02.part
			Part :LifeSpore_Tintable_Mush01.part
		End

	End

	Condition
		On Time
		Time 0
		Repeat 2

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0 0 0
				#PyrRotateJitter 0 180 0
				StartJitter 6 0 6
				Scale 2 0.2 2
				EndScale 3 0.5 3
				Alpha 255
				FadeInLength 120
				PrimaryTint 80
				SecondaryTint 20
			End
			Geom FX_Tintable_Bush_AzXLgAFol
			LifeSpan 170
		End

	End

	Condition
		On Cycle
		Time 2
		Random 1

		Event
			At SpinAnchor
			Type Start
			EName Prime1
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				StartJitter 3 0.5 3
				Alpha 255
				#StartColor 255 255 255
				Scale .5 .5 .5
				EndScale 3 3 3
				PyrRotate 0 0 0
				PyrRotateJitter 10 180 10
			End
#			Geom FX_Mushroom_01
#			Geom FX_Mushroom_02
#			Geom FX_Mushroom_03
#			Geom FX_Mushroom_04
#			Geom FX_Mushroom_05
			Geom FX_Tintable_Mushroom_01
			Geom FX_Tintable_Mushroom_02
			Geom FX_Tintable_Mushroom_03
			Geom FX_Tintable_Mushroom_04
			Geom FX_Tintable_Mushroom_05
			LifeSpan 170
		End

		Event
			At SpinAnchor
			Type Start
			EName Prime3
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				StartJitter 3 0.5 3
				Alpha 255
				#StartColor 255 255 255
				Scale .5 .5 .5
				EndScale 2.3 2.3 2.3
				PyrRotate 0 0 0
				PyrRotateJitter 10 180 10
			End
#			Geom FX_Mushroom_01
#			Geom FX_Mushroom_02
#			Geom FX_Mushroom_03
#			Geom FX_Mushroom_04
#			Geom FX_Mushroom_05
			Geom FX_Tintable_Mushroom_01
			Geom FX_Tintable_Mushroom_02
			Geom FX_Tintable_Mushroom_03
			Geom FX_Tintable_Mushroom_04
			Geom FX_Tintable_Mushroom_05
			LifeSpan 170
		End

	End

End
