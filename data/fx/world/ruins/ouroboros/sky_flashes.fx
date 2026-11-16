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
	On	Time
	Time	0

	Event
		Type	StartPositOnly
		At	Root
		Part	:Clouds_Rolling.part
		Part	:Clouds_Rolling_Additive.part
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.3

	Event
		Type	StartPositOnly
		At	Root
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
		Type	Local
		At	Root
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