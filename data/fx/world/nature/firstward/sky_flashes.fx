#############################################################
## Sky_Flashes.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On		TimeOfDay
	DayStart 	19
	DayEnd		4
	DoMany		1

	Event
		Ename	FlashAnchor
		At	Root
		BhvrOverride
			PositionOffset	0 2500 0
		End
		Flags	OneAtATime
		Lifespan 100
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.5

	Event
		EName	SkyFlashes
		Type	StartPositOnly
		At	FlashAnchor
		BhvrOverride
			StartJitter	1000 0 1000
		End
		Part	:Flashes_Cloud.part
		Part	:Flashes_Cloud_Highlights.part
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	6
	Chance	0.25
	Event
		EName	Lightning
		Type	Local
		At	FlashAnchor
		BhvrOverride
			StartJitter	700 0 700
		End
		ChildFX	:Child_Lightning.fx
		SoundNoRepeat 3
		Sound Thunder_01 1000 1000 .7
		Sound Thunder_02 1000 1000 .7
		Sound Thunder_03 1000 1000 .7
		Sound Thunder_04 1000 1000 .7
		Sound Thunder_05 1000 1000 .7
		Sound Thunder_06 1000 1000 .7
	Lifespan 1
	End
End

#############################################################

End