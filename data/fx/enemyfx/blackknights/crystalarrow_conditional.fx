#
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Cycle
		Time 25

		Event
			At Hips
			Type Local
			EName Swirls
			Bhvr CustomizeablePowers\DarknessControl\DarkGrasp_Scale.bhvr
			BhvrOverride
				Spin 0 0.2 0
				#TintGeom 1
				StartColor 255 255 255
			End
			Geom Tintable_DarkGrasp
			LifeSpan 35
		End

		Event
			At Root
			Type Local
			BhvrOverride
				Spin 0 0.1000 0
			End
			LifeSpan 50
		End

		Event
			At Hips
			Type Local
			EName SwirlsSmall
			Bhvr CustomizeablePowers\DarknessControl\DarkGrasp_Scale_Small.bhvr
			BhvrOverride
				#TintGeom 1
				StartColor 255 255 255
			End
			Geom Tintable_DarkGrasp
			LifeSpan 35
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type PositOnly
			EName Core
			Part ENEMYFX\BlackKnights\ColdCore.part
			Part ENEMYFX\BlackKnights\ColdSmoke.part
		End

	End

	Condition
		On Death

		Event
			Type Destroy
			EName CrushingHateGeo
		End

	End

End
