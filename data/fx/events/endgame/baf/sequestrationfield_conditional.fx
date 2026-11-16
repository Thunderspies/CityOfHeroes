#############################################################
## ForceCage_Conditional.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.0 0.05
			Scale		0.9 1.1 0.625
			StartColor	255 112 16
		End
		Geom	GEO_ForceCage_Shackle
	End

	Event
		Type	local
		At	LArmL
		BhvrOverride
			PYRRotate	0 0 90
			PositionOffset	-0.65 0 0
			Scale		0.3 0.2 0.2
			StartColor	255 112 16
		End
		Geom	GEO_ForceCage_Shackle_Small
	End

	Event
		Type	local
		At	LArmR
		BhvrOverride
			PYRRotate	0 0 -90
			PositionOffset	0.65 0 0
			Scale		0.3 0.2 0.2
			StartColor	255 112 16
		End
		Geom	GEO_ForceCage_Shackle_Small
	End

	Event
		Type	local
		At	LLegR
		BhvrOverride
			PYRRotate	0 0 0
			PositionOffset	0.0 -1.35 -0.35
			Scale		0.3 0.3 0.3
			StartColor	255 112 16
		End
		Geom	GEO_ForceCage_Shackle_Small
	End

	Event
		Type	Local
		At	LLegL
		BhvrOverride
			PYRRotate	0 0 0
			PositionOffset	0.0 -1.35 -0.35
			Scale		0.3 0.3 0.3
			StartColor	255 112 16
		End
		Geom	GEO_ForceCage_Shackle_Small
	End
End

#############################################################

End