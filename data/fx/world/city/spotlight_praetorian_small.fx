#############################################################
## Lightning_Red.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event

		Type	start
		At	Root
		BhvrOverride
			Alpha		100
			PositionOffset	0 -5 0
			Scale 		24 350 24
			StartColor	200 190 150
		End
		Geom	FX_P_Spotlight
	End

	Event

		Type	start
		At	Root
		BhvrOverride
			Alpha		150
			PositionOffset	0 -5 0
			Scale 		24 75 24
			StartColor	200 190 150
		End
		Geom	FX_P_Spotlight
	End
End

##############################################################
End