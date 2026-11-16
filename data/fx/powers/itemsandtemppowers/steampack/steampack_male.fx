#############################################################
## SteamPack_MALE.fx
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Posit
		At	FootR
		Sound Superleap_loop 50.0 50.0 0.5
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 100
	End

	Event
		EName	PackGeo
		Type	Local
		At	Chest
		BhvrOverride
			Scale		1.0 1.0 1.0
			PositionOffset	0 0.5 -0.5
		End
		GEOM	GEO_Jet_Pack_Steampunk_01
	End


End

#############################################################

End