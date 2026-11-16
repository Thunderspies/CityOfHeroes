#
FxInfo

	LifeSpan 300


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DD_ImpaleCrystal_Pet_01 300 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type StartPositOnly
			EName IcicleCentralAnchor
			Bhvr behaviors\CameraShakeStomp.bhvr
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 1
		Repeat 1

		Event
			At IcicleCentralAnchor
			Type StartPositOnly
			ChildFx :CHILD_Impale_DirtSputter_Delay.fx
			EName IcicleCentral
			Bhvr :Impale_Icicle_Large.bhvr
			BhvrOverride
				StartJitter 1 0 1
				StartColor 1 30 50
			End
			Geom FX_Impale_Icicle01
			Geom FX_Impale_Icicle02
			Geom FX_Impale_Icicle03
			Geom FX_Impale_Icicle04
			Geom FX_Impale_Icicle05
			LifeSpan 90
		End

	End

	Condition
		On Time
		Time 5
		Repeat 12
		Random 1

		Event
			At Root
			Type StartPositOnly
			ChildFx :CHILD_Impale_DirtSputter_Delay.fx
			EName IcicleTall
			Bhvr :Impale_Icicle.bhvr
			BhvrOverride
				StartJitter 16 0 16
				StartColor 0 30 50
			End
			Geom FX_Impale_Icicle01
			Geom FX_Impale_Icicle02
			Geom FX_Impale_Icicle03
			Geom FX_Impale_Icicle04
			Geom FX_Impale_Icicle05
			LifeSpan 90
		End

		Event
			At Root
			Type StartPositOnly
			ChildFx :CHILD_Impale_DirtSputter_Delay.fx
			EName IcicleShort
			Bhvr :Impale_Icicle_Small.bhvr
			BhvrOverride
				StartJitter 16 0 16
				StartColor 10 30 50
			End
			Geom FX_Impale_Icicle01
			Geom FX_Impale_Icicle02
			Geom FX_Impale_Icicle03
			Geom FX_Impale_Icicle04
			Geom FX_Impale_Icicle05
			LifeSpan 90
		End

	End

	Condition
		On Time
		Time 5
		Repeat 15
		Random 1

		Event
			At Root
			Type StartPositOnly
			ChildFx :CHILD_Impale_DirtSputter.fx
			EName IcicleTall
			Bhvr :Impale_Icicle.bhvr
			BhvrOverride
				StartJitter 16 0 16
				StartColor 0 30 50
			End
			Geom FX_Impale_Icicle01
			Geom FX_Impale_Icicle02
			Geom FX_Impale_Icicle03
			Geom FX_Impale_Icicle04
			Geom FX_Impale_Icicle05
			LifeSpan 85
		End

		Event
			At Root
			Type StartPositOnly
			ChildFx :CHILD_Impale_DirtSputter.fx
			EName IcicleShort
			Bhvr :Impale_Icicle_Small.bhvr
			BhvrOverride
				StartJitter 16 0 16
				StartColor 10 30 50
			End
			Geom FX_Impale_Icicle01
			Geom FX_Impale_Icicle02
			Geom FX_Impale_Icicle03
			Geom FX_Impale_Icicle04
			Geom FX_Impale_Icicle05
			LifeSpan 85
		End

	End

	Condition
		On Time
		Time 84

		Event
			At Root
			Type StartPositOnly
			EName FlashAnchor
			LifeSpan 5
		End

	End

	Condition
		On Cycle
		Time 1
		Repeat 1

		Event
			At FlashAnchor
			Type StartPositOnly
			EName FlashAnchorCycle
			BhvrOverride
				PositionOffset 0 2 0
				StartJitter 13 1 13
			End
			LifeSpan 5
		End

		Event
			At FlashAnchorCycle
			Type StartPositOnly
			Bhvr :IceChunk_Hit.bhvr
			BhvrOverride
				StartColor 255 255 255
			End
			Part :Impale_Flash_Core.part
			Part :Impale_Streaks.part
			Part :Impale_Sparkles.part
			LifeSpan 3
		End

	End

End
