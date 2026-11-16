#############################################################
## Streetlight_Praetorian_Flicker.fx
#############################################################

FxInfo
Lifespan 20

#############################################################

Condition
	On 	Cycle
	Time 	4
	Chance 0.6

	Event

		Type	start
		At	Head
		BhvrOverride
			Alpha		200
			Scale 		15 15 15
			StartColor	230 220 200
		End
	#	Geom	FX_Streetlamp
		Lifespan 2
	End

	Event

		Type	PositOnly
		At	Head
		BhvrOverride
			PyrRotate	90 -90 0
			Alpha		200
			PositionOffset	0 -20 0
			Scale 		20 20 20
			StartColor	230 220 200
		End
		Geom	FX_Streetlamp_Base
		Lifespan 2
	End
End

##############################################################
End