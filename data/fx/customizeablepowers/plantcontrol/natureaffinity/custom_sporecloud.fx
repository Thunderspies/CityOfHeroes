#                                                          
FxInfo

	Flags InheritAlpha InheritGeoScale

	LifeSpan 45


###########################################################

	#Condition
		#On Time
		#Time 0

		#Event
			#At Root
			#Type Local
			#Sound GeneticContamination_01 100 100 0.8
		#End

	#End

	Condition
		On Time
		Time 0

		Event
			At T_Chest
			Type PositOnly
			EName RingNode
			Bhvr behaviors/genericparticlefade.bhvr
			BhvrOverride
				#PositionOffset 0 2 0
			End
		End

	End

	Condition
		On Time
		Time 0

		Event
			At RingNode
			Type Local
			EName FogRing1
			Bhvr behaviors/genericparticlefade.bhvr
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Spore_DebuffSpores_RadOut.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Spore_RingSmall.part
		End

		Event
			At RingNode
			Type Local
			EName FogRing2
			Bhvr behaviors/genericparticlefade.bhvr
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Spore_DebuffGlow_Big.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Spore_DebuffSpores_Big.part
		End

		Event
			At RingNode
			Type Local
			EName SwirlForce
			BhvrOverride
				physForceType CWSwirl
				physForceRadius 30
				physForceCentripetal .6
				physForcePower 50
				physForcePowerJitter 5
			End
		End

		Event
			At Root
			Type Local
			EName LevitationForce
			BhvrOverride
				physForceType Up
				physForceRadius 16
				physForcePower 45
				physForcePowerJitter 5
			End
		End

		Event
			At RingNode
			Type Local
			EName LevitationForce
			BhvrOverride
				PositionOffset 0 6 0
				physForceType Up
				physForceRadius 10
				physForcePower -15
				physForcePowerJitter 5
			End
		End

		Event
			At RingNode
			Type Local
			EName Hurricane
			Bhvr behaviors/genericparticlefade.bhvr
			#Part CustomizeablePowers\BioArmor\Genetic_Ring02.part
		End

	End

	Condition
		On Time
		Time 0
		Chance 0.5

		Event
			At T_UArmL
			Type Local
			EName Prime1
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset -0.15 0.05 0
				Alpha 255
				#StartColor 255 255 255
				Scale .01 .01 .01
				EndScale .7 .9 .7
				PyrRotate 0 0 0
				PyrRotateJitter 0 0 30
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
			LifeSpan 120
		End

	End

	Condition
		On Time
		Time 0
		Chance 0.5

		Event
			At T_UArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0.2 0.05 -0.1
				Alpha 255
				#StartColor 255 255 255
				Scale .01 .01 .01
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

	End

	Condition
		On Time
		Time 0
		Chance 0.5

		Event
			At T_Head
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0 0.25 0.1
				Alpha 255
				#StartColor 255 255 255
				Scale .01 .01 .01
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

	End

	Condition
		On Time
		Time 0
		Chance 0.7

		Event
			At T_Chest
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0 0.5 -0.15
				Alpha 255
				#StartColor 255 255 255
				EndScale 3 3 3
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

End
