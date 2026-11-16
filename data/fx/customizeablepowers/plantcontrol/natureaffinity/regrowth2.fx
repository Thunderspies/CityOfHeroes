#
FxInfo

	LifeSpan 400


###########################################################

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Local
			Sound NA_Regrowth_02 100 100 0.8
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
			At Chest
			Type Positonly
			Part :Heal_pollen.part
			Part :Heal_Stars.part
			Lifespan 35
		End

	End

	Condition
		On Time
		Time 10

		Event
			At WepL
			Type Local
			EName Anchor
			BhvrOverride
				#PositionOffset -0.1 4.2 2
				PyrRotate 8 -100 0
			End
			Part CustomizeablePowers\PlantControl\NatureAffinity\Regrowth_LeavesA.part
			#Part CustomizeablePowers\PlantControl\NatureAffinity\Regrowth_LeavesB.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Regrowth_Stars.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Regrowth_Pollen.part
			#Geom geo_Fx_testsphere
			LifeSpan 45
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Anchor
			Type Local
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part :Regrowth_Glow_Core.part
			Part :Regrowth_GlowTrails.part
			Part :WindStream.part
			LifeSpan 35
		End

	End

End
