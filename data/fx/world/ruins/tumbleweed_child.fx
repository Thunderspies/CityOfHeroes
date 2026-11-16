#
FxInfo
Lifespan 40

###########################################################

	Condition
		On Time
		Time 0
		Chance 0.5000

		Event
			At Root
			Type Local
			EName DustUp
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part WORLD\Ruins\Dust_KickUp.part
			LifeSpan 30
			LifeSpanJitter 10.0000
		End

	End

Condition
	On	Time
	Time	0

	Event
		Ename	SoundGoesHere
		At	Root
		Type	Local
		SoundNoRepeat 3
		Sound Tumbleweed_Bounce_01 50 50 .1
		Sound Tumbleweed_Bounce_02 50 50 .1
		Sound Tumbleweed_Bounce_03 50 50 .1
		Sound Tumbleweed_Bounce_04 50 50 .1
		Sound Tumbleweed_Bounce_05 50 50 .1
		Sound Tumbleweed_Bounce_06 50 50 .1
		Sound Tumbleweed_Bounce_07 50 50 .1
	End
End

##################################################
End
