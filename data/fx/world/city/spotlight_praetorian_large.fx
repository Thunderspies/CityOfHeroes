#############################################################
## Spotlight_Praetorian_Large.fx
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
			Scale 		30 500 30
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
			Scale 		30 100 30
			StartColor	230 220 200
		End
		Geom	FX_P_Spotlight
	End

#	Event
#
#		Type	start
#		At	Root
#		BhvrOverride
#			Alpha		255
#			PositionOffset	0 2 0
#			Scale 		33 0 33
#			StartColor	255 200 100
#
#		End
#		Geom	FX_P_Spotlight_Base
#	End
#
#	Event
#
#		Type	start
#		At	Root
#		BhvrOverride
#			Alpha		255
#			PositionOffset	0 2 0
#			Scale 		30 0 30
#			StartColor	255 200 200
#
#		End
#		Geom	FX_P_Spotlight_Base
#	End
End

##############################################################
End