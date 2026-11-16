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
			ChildFx :Custom_Regrowth_Activation.fx
		End

		Event
			At Chest
			Type PositOnly
			Part :Tintable_Heal_pollen.part
			Part :Tintable_Heal_Stars.part
			LifeSpan 35
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
				PyrRotate 8 -90 0
			End
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Regrowth_LeavesA.part
			#Part CustomizeablePowers\PlantControl\NatureAffinity\Regrowth_LeavesB.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Regrowth_Stars.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Regrowth_Pollen.part
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
			Part :Tintable_Regrowth_Glow_Core.part
			Part :Tintable_Regrowth_GlowTrails.part
			Part :WindStream.part
			LifeSpan 35
		End

	End

End
