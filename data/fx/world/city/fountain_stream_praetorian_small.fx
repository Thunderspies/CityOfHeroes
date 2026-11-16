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
		Part	:Water_Stream_Small.part
		Part	:Water_Stream_Additive_Small.part
		Part	:Water_Stream_Column_Small.part
	End

	Event
		EName 	SplashAnchor
		Type	Local
		At	FountainAnchor
		BhvrOverride
			PositionOffset	0.0 0.45 7.0
		End
		Part	:Bottom_Splash_Small.part
	End
	Event
		EName 	SplashAnchor
		Type	Local
		At	FountainAnchor
		BhvrOverride
			PositionOffset	0.0 0.7 7.0
		End
		Part	:Bottom_Splash_Flat_Small.part
	End

End
End