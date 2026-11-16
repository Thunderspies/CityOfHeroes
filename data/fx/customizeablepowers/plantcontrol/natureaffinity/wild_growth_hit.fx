#
FxInfo
Lifespan 300

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type PositOnly
			BhvrOverride
				PyrRotate 0 0 15
			End
			Part CustomizeablePowers\PlantControl\NatureAffinity\Wild_GlowStreaks.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Wild_Glow.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Wild_Leaves.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Wild_Pollen.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Wild_Stars.part
			LifeSpan 150
		End
	End

	Condition
		On Time
		Time 10

		Event
			Ename ButtAnchor
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 0 0
			End
			ChildFx Auras\Male\Butterflies\Butterflies_Light_Idle_Child02.fx
			LifeSpan 150
		End
	End

	Condition
		On Cycle
		Time 42

		Event
			At ButtAnchor
			Type Local
			ChildFx Auras\Male\Butterflies\Butterflies_Light_Idle_Child02.fx
			LifeSpan 150
		End
	End

End
