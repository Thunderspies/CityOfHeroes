#                                                          
FxInfo


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	NA_SporeCloud_01 100 100 .8
	End
End
	Condition
		On Time
		Time 0

		Event
			At UArmL
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				PositionOffset -0.15 0.05 0
				PyrRotate 0 0 0
				PyrRotateJitter 0 0 30
				EndScale 0.7 0.9 0.7
				TintGeom 1
				Alpha 255
				StartColor 255 255 255
			End
			#Geom FX_Mushroom_01
			#Geom FX_Mushroom_02
			#Geom FX_Mushroom_03
			#Geom FX_Mushroom_04
			#Geom FX_Mushroom_05
			Geom FX_Tintable_Mushroom_01
			Geom FX_Tintable_Mushroom_02
			Geom FX_Tintable_Mushroom_03
			Geom FX_Tintable_Mushroom_04
			Geom FX_Tintable_Mushroom_05
		End

		Event
			At UArmL
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset -0.6 -0.05 0
				Alpha 255
				StartColor 255 255 255
				EndScale .6 .8 .6
				PyrRotate 0 90 30
			End
			#Geom FX_Mushroom_01
			#Geom FX_Mushroom_02
			#Geom FX_Mushroom_03
			#Geom FX_Mushroom_04
			#Geom FX_Mushroom_05
			Geom FX_Tintable_Mushroom_01
			Geom FX_Tintable_Mushroom_02
			Geom FX_Tintable_Mushroom_03
			Geom FX_Tintable_Mushroom_04
			Geom FX_Tintable_Mushroom_05
		End

		Event
			At UArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0.2 0.05 -0.1
				Alpha 255
				StartColor 255 255 255
				EndScale .7 .9 .7
				PyrRotate 0 90 30
			End
			#Geom FX_Mushroom_01
			#Geom FX_Mushroom_02
			#Geom FX_Mushroom_03
			#Geom FX_Mushroom_04
			#Geom FX_Mushroom_05
			Geom FX_Tintable_Mushroom_01
			Geom FX_Tintable_Mushroom_02
			Geom FX_Tintable_Mushroom_03
			Geom FX_Tintable_Mushroom_04
			Geom FX_Tintable_Mushroom_05
		End

		Event
			At UArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				PositionOffset 0.8 0.05 -0.1
				Alpha 255
				StartColor 255 255 255
				EndScale .6 .8 .6
				PyrRotate 0 90 30
			End
			#Geom FX_Mushroom_01
			#Geom FX_Mushroom_02
			#Geom FX_Mushroom_03
			#Geom FX_Mushroom_04
			#Geom FX_Mushroom_05
			Geom FX_Tintable_Mushroom_01
			Geom FX_Tintable_Mushroom_02
			Geom FX_Tintable_Mushroom_03
			Geom FX_Tintable_Mushroom_04
			Geom FX_Tintable_Mushroom_05
		End

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0 0.25 0.1
				Alpha 255
				StartColor 255 255 255
				EndScale .6 .8 .6
				PyrRotateJitter 20 90 20
			End
			#Geom FX_Mushroom_01
			#Geom FX_Mushroom_02
			#Geom FX_Mushroom_03
			#Geom FX_Mushroom_04
			#Geom FX_Mushroom_05
			Geom FX_Tintable_Mushroom_01
			Geom FX_Tintable_Mushroom_02
			Geom FX_Tintable_Mushroom_03
			Geom FX_Tintable_Mushroom_04
			Geom FX_Tintable_Mushroom_05
		End

		Event
			At Chest
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0 0.5 -0.15
				Alpha 255
				StartColor 255 255 255
				EndScale 1.5 1.5 1.5
				PyrRotate -30 180 0
				PyrRotateJitter 10 30 15
			End
			#Geom FX_Mushroom_01
			#Geom FX_Mushroom_02
			#Geom FX_Mushroom_03
			#Geom FX_Mushroom_04
			#Geom FX_Mushroom_05
			Geom FX_Tintable_Mushroom_01
			Geom FX_Tintable_Mushroom_02
			Geom FX_Tintable_Mushroom_03
			Geom FX_Tintable_Mushroom_04
			Geom FX_Tintable_Mushroom_05
		End

	End

	Condition
		On Cycle
		Time 10

		Event
			At Chest
			Type Local
			EName RINGS
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.2 0
			End
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Spore_DebuffGlow_Big.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Spore_DebuffSpores_Big.part
			LifeSpan 20
		End

	End

End
