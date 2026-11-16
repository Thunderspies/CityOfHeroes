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
		Part	:Tattoo_Glow_Arm_Female.part
		POther	UArmL
		Lifespan 0
	End

	Event
		Type	Local
		At	LArmR
		Part	:Tattoo_Glow_Arm_Female.part
		POther	UArmR
		Lifespan 0
	End


	Event
		Type	Local
		At	Col_L
		Geom	FX_Yakuza_ChestL
		BhvrOverride
			Scale		0.95 0.95 0.95
			PositionOffset	0.1 -0.1 0.10
			PYRRotate	30 0 0
		#	StartColor	255 150 50
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	UArmL
		Geom	FX_Yakuza_UArmL
		BhvrOverride
			Scale		1.0 1.0 0.825
			PositionOffset	0.0 -0.05 0.0
			PYRRotate	-30 0 5
		#	StartColor	255 100 50
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	Col_R
		Geom	FX_Yakuza_ChestR
		BhvrOverride
			Scale		0.95 0.95 0.95
			PositionOffset	-0.1 -0.1 0.10
			PYRRotate	30 0 0
		#	StartColor	255 150 50
			TintGeom	1

		End
		Lifespan 0
	End

	Event
		Type	Local
		At	UArmR
		Geom	FX_Yakuza_UArmR
		BhvrOverride
			Scale		1.0 1.0 0.825
			PositionOffset	0.0 -0.05 0.0
			PYRRotate	-30 0 -5
	#		StartColor	255 100 50
			TintGeom	1
		End
		Lifespan 0
	End


	Event
		Type	Local
		At	Back
		BhvrOverride
			Scale		1.0 1.0 1.0
			PositionOffset	0.0 0.0 0.0
		#	StartColor	255 150 50
			TintGeom	1
		End
		Geom	FX_Yakuza_Spine
		Lifespan 0
	End
End

#############################################################

End