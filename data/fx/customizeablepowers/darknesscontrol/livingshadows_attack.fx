#
FxInfo

	LifeSpan 300


###########################################################
Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	DC_LivingShadows_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 15

		Event
			EName	OffsetAnchor
			Type 	Local
			At 	Chest
			Geom	FX_ConeOffsets
			BhvrOverride
				Scale		4.0 4.0 4.0
				PYRRotate	-90 0 0
			End
			LifeSpan 32
		End

		Event
			EName	HandsAnchor
			Type	Local
			At 	OffsetAnchor
			AltPiv	1
			BhvrOverride
				Scale	0.2 0.2 0.2
			End
			LifeSpan 32
		End

		Event
			At HandsAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 0 1
				PYRRotate	50 0 0

			End
			Part CustomizeablePowers\DarknessControl\LivingShadows_Core_Flat.part
			LifeSpan 22
		End

		Event
			At HandsAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 0.5 -1
				PYRRotate	50 0 0

			End
			Part :Shadows_Base.part
			LifeSpan 32
		End



	End

	Condition
		On Time
		Time 25
		Random 1

		Event
			EName ShadowGeo
			At HandsAnchor
			Type Start
			Bhvr :LivingShadows_Scale.bhvr
			Geom Tintable_LivingShadows
			LifeSpan 20
		End

		Event
			EName ShadowGeo_Inverted
			At HandsAnchor
			Type Start
			Bhvr :LivingShadows_Scale.bhvr
			Geom Tintable_LivingShadows_Inverted
			LifeSpan 20
		End

	End

End
