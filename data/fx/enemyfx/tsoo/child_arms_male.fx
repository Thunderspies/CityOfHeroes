#############################################################
## SingleLargePowerPunch.fx
#############################################################

FxInfo
Flags	InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Part	:Tattoo_Glow_Arm.part
		POther	UArmL
		Lifespan 0
	End

	Event
		Type	Local
		At	LArmR
		Part	:Tattoo_Glow_Arm.part
		POther	UArmR
		Lifespan 0
	End


	Event
		Type	Local
		At	UArmL
		Geom	FX_Yakuza_UArmL
		BhvrOverride

			Scale		1.5 1.5 1.5
			PositionOffset	0.0 -0.05 0.0
			StartColor	255 150 50
			PYRRotate	-45 0 0
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	Col_L
		BhvrOverride

			Scale		1.5 1.5 1.5
			PositionOffset	0.3 -0.2 0.3
			StartColor	255 150 50
		End
		Geom	FX_Yakuza_ChestL
		Lifespan 0
	End

	Event
		Type	Local
		At	UArmR
		BhvrOverride

			Scale		1.5 1.5 1.5
			PositionOffset	0.0 -0.05 0.0
			StartColor	255 150 50
			PYRRotate	-45 0 0
		End
		Geom	FX_Yakuza_UArmR
		Lifespan 0
	End

	Event
		Type	Local
		At	Col_R
		BhvrOverride

			Scale		1.5 1.5 1.5
			PositionOffset	-0.3 -0.2 0.3
			StartColor	255 150 50
		End
		Geom	FX_Yakuza_ChestR
		Lifespan 0
	End

	Event
		Type	Local
		At	Col_R
		BhvrOverride

			Scale		1.5 1.5 1.5
			PositionOffset	-0.3 -0.2 0.3
			StartColor	255 150 50
		End
		Geom	FX_Yakuza_Spine
		Lifespan 0
	End

	Event
		Type	Local
		At	Back
		BhvrOverride

			Scale		0.8 0.8 0.8
			PositionOffset	0.0 0.0 0.0
			StartColor	255 150 50
		End
		Geom	FX_Yakuza_Spine
		Lifespan 0
	End
End

#############################################################

End