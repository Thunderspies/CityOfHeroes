#
FxInfo

Lifespan 1000


###########################################################

	Condition
		On Cycle
		Time 40
		Repeat 1

		Event
			At Head
			Type PositOnly
			ChildFx :SorrowSong_Continuing_Child.fx
			Bhvr Behaviors/FadeInOut.bhvr
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			LifeSpan 120
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName Anchor
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_FallingC.part
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_FallingD.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_Falling.part
		End

	End

End
