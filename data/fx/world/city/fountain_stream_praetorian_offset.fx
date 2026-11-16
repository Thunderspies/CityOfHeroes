#########################################################
##	SStreetSteeam01
##
FxInfo

LifeSpan 0

Input
	InpName	root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	FountainAnchor
		Type	start
		At	root
		Part	:Water_Stream.part
		Part	:Water_Stream_Additive.part
		Part	:Water_Stream_Column.part
	End

	Event
		EName 	SplashAnchor
		Type	Local
		At	FountainAnchor
		BhvrOverride
			PositionOffset	0.0 1.35 10.0
		End
		Part	:Bottom_Splash_Small.part
	End
	Event
		EName 	SplashAnchor
		Type	Local
		At	FountainAnchor
		BhvrOverride
			PositionOffset	0.0 1.6 10.0
		End
		Part	:Bottom_Splash_Flat_Small.part
	End

End
End