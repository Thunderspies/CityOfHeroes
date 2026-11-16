#############################################################
## Test.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time	25

	Event
		EName	HoloSight_Right
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Pistols\Maelstrom\Maelstrom_HoloSights_Create.bhvr
		Geom	GEO_WepR_Pistol_Maelstrom_Overlay
		Lifespan 10
	End

	Event
		EName	HoloSight_Left
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Pistols\Maelstrom\Maelstrom_HoloSights_Create.bhvr
		BhvrOverride
			PYRRotate		0 0 180
		End
		Geom	GEO_WepR_Pistol_Maelstrom_Overlay
		Lifespan 10
	End
End

Condition
	On 	Time
	Time	30

	Event
		EName	HoloSight_Right
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Pistols\Maelstrom\Maelstrom_HoloSights_Fade.bhvr
		Geom	GEO_WepR_Pistol_Maelstrom_Overlay
	End

	Event
		EName	HoloSight_Left
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Pistols\Maelstrom\Maelstrom_HoloSights_Fade.bhvr
		BhvrOverride
			PYRRotate		0 0 180
		End
		Geom	GEO_WepR_Pistol_Maelstrom_Overlay
	End
End

#############################################################

End