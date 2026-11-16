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
		At	Root
		BhvrOverride
			PositionOffset	0.25 -0.25 -0.25
			Scale		0.875 1.0 0.875
			Spin		0.0 0.02 0.0
		End
		Geom	GEO_ForceCage_Alert
	End

#	Event
#		Type	local
#		At	Root
#		BhvrOverride
#			PositionOffset	0.25 0 -0.25
#			StartColor	0 96 255
#			Alpha		16
#		End
#		Geom	GEO_ForceCage
#	End
#
#	Event
#		Type	local
#		At	Root
#		BhvrOverride
#			PositionOffset	0.25 0 -0.25
#			StartColor	64 128 255
#			Alpha		255
#		End
#		Geom	GEO_ForceCage_Pulse
#	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.0 -0.05
			Scale		1.5 1.5 0.9
			StartColor	32 112 255
		End
		Geom	GEO_ForceCage_Shackle
	End

	Event
		Type	local
		At	LArmL
		BhvrOverride
			PYRRotate	0 0 90
			PositionOffset	-0.7 0 0
			Scale		0.35 0.3 0.3
			StartColor	32 112 255
		End
		Geom	GEO_ForceCage_Shackle_Small
	End

	Event
		Type	local
		At	LArmR
		BhvrOverride
			PYRRotate	0 0 -90
			PositionOffset	0.7 0 0
			Scale		0.35 0.3 0.3
			StartColor	32 112 255
		End
		Geom	GEO_ForceCage_Shackle_Small
	End

	Event
		Type	local
		At	LLegR
		BhvrOverride
			PYRRotate	-15 0 0
			PositionOffset	0.0 -1.0 -0.5
			Scale		0.5 0.5 0.5
			StartColor	32 112 255
		End
		Geom	GEO_ForceCage_Shackle_Small
	End

	Event
		Type	Local
		At	LLegL
		BhvrOverride
			PYRRotate	-15 0 0
			PositionOffset	0.0 -1.0 -0.5
			Scale		0.5 0.5 0.5
			StartColor	32 112 255
		End
		Geom	GEO_ForceCage_Shackle_Small
	End
End

## CAGE EFFECT #######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Hookup
		Type	Local
		At	root
		bhvr	behaviors\Invulnerability_Spin.bhvr
		BhvrOverride
			PositionOffset	0.25 -0.25 -0.25
			Scale		0.25 0.25 0.25
		End
		geom	FX_RuneStreak
	End

	Event
		Type	Local
		At	Hookup
		altpiv  1
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceCage_VerticalStreaks01.part
	End

	Event
		Type	Local
		At	Hookup
		altpiv  2
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceCage_VerticalStreaks01.part
	End
End

#############################################################

End