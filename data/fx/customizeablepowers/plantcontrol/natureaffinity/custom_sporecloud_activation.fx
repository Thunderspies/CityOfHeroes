#                                                          
FxInfo

	LifeSpan 38


###########################################################
Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	NA_SporeCloudActivation_01 100 100 .8
	End
End

	Condition
		On Cycle
		Time 8

		Event
			At WepL
			Type PositOnly
			Bhvr behaviors\Splat_RuneCircle.bhvr
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Spore_DebuffGlow_Sm.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Spore_DebuffSpores_Sm.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 0
		Repeat 8

		Event
			At T_Root
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				StartJitter 2.5 0 2.5
				Alpha 255
				StartColor 255 255 255
				EndScale 1.2 1.2 1.2
				PyrRotate 0 0 0
				PyrRotateJitter 0 180 0
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
		On Time
		Time 0
		Repeat 3

		Event
			At T_Root
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				StartJitter 2 0 2
				Alpha 255
				StartColor 255 255 255
				EndScale 2.2 2.2 2.2
				PyrRotate 0 0 0
				PyrRotateJitter 0 180 0
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

End
