#
FxInfo


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Emote_InnerWill_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 0.15 0.5
			End
			Part Emotes\CostumeChanges\InnerWill\particles_Fast.part
			Part Emotes\CostumeChanges\InnerWill\Particles_Fast_Short.part
			LifeSpan 60
		End

		Event
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 0.15 0.9
				PyrRotate 0 90 90
			End
			Part Emotes\CostumeChanges\InnerWill\ChestGlow_Lg.part
			Part Emotes\CostumeChanges\InnerWill\ChestGlow.part
			LifeSpan 70
		End

		Event
			At Chest
			Type Local
			EName BG
			BhvrOverride
				PositionOffset 0 0.15 0
			End
			Part Emotes\CostumeChanges\InnerWill\BackGlow.part
			Part Emotes\CostumeChanges\InnerWill\BackGlow_Add.part
			Part Emotes\CostumeChanges\InnerWill\Streaks.part
			Part Emotes\CostumeChanges\InnerWill\StreakGlow_Add.part
			LifeSpan 70
		End

	End

	Condition
		On Time
		Time 70

		Event
			At Chest
			Type Local
			ChildFx Emotes\CostumeChanges\Howl\BodyGlow.fx
			EName Burst
			Part :EmberBurst.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 72

		Event
			At Chest
			Type Local
			EName Flash
			Part :Blast.part
			Part :CircleBlast.part
			LifeSpan 30
		End

	End

End
