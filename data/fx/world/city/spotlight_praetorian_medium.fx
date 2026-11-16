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
			Alpha		150
			PositionOffset	0 -30 0
			Scale 		30 350 30
			StartColor	255 180 0
		End
		Geom	FX_P_Spotlight
	End

	Event

		Type	start
		At	Root
		BhvrOverride
			Alpha		200
			PositionOffset	0 -30 0
			Scale 		15 75 15
			StartColor	255 220 200
		End
		Geom	FX_P_Spotlight
	End
End

##############################################################
End