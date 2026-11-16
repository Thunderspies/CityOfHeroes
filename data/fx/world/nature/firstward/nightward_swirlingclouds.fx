#############################################################
## FirstWard_SwirlingClouds.fx
#############################################################

FxInfo

#########################################################

Input
	InpName	Root
End

#########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FirstWard_Vortex1_Loop 4500 1000 .20
	End
End



Condition
	On 	Time
	Time 	0

	Event
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			Alpha		150
			PositionOffset	0 -2500 0
			Scale		150 110 150
			StartColor	70 230 150
			Spin		0 -0.00125 0
		End
		GEOM	FX_FirstWard_Clouds3
	End


	Event
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			Alpha		150
			PositionOffset	0 -2000 0
			Scale		80 80 80
			StartColor	150 250 190
			Spin		0 -0.0035 0
			PyrRotate	0 180 0
		End
		GEOM	FX_FirstWard_Clouds4
	End

	Event
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			Alpha		150
			PositionOffset	0 -1500 0
			Scale		75 75 75
			StartColor	205 250 150
			PyrRotate	0 180 0
			Spin		0 -0.002 0

			PyrRotate	0 90 0

		End
		GEOM	FX_FirstWard_Clouds3
	End
End

Condition
	On		Cycle
	Time		100
#	DayEnd		4
#	DoMany		1

	Event
		EName	Flashes
		Type	StartPositOnly
		At	Origin
		Flags	OneAtATime
		ChildFX	:Sky_Flashes.fx
		Lifespan	100
	End
End

#########################################################

End