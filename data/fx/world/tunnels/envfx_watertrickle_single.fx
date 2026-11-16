#############################################################
## EnvFX_WaterTrickle_Long.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	15
	Chance 	.2

	Event
		EName	DropletAnchor2
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	1.0 0.0 1.0
		End
		Geom	FX_WaterDroplet_Short
		Lifespan 60
	End

	Event
		Type	Start
		At	DropletAnchor2
		AltPiv	2
		ChildFX	:ChildFX_Droplet_Group_Falling.fx
		Lifespan 60

	End

End

#########################################################

End