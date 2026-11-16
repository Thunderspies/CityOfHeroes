#############################################################
## Geysers_Damage.fx
#############################################################

FxInfo
Lifespan 150

#########################################################
#################### GEYSER ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Ename	GeyserHighlights
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -13.0 0.0
		End
		Part	:GeyserHighlightsArea_Damage.part
		Sound Sigil_Geyser_Damage 200 200 1
		Lifespan 40
	End


	Event
		Ename 	Flash
		Type	StartPositOnly
		At	Origin

		Part	:GasRadiusRocksBig.part
		Part	:LavaSpray2_Damage.part
		Lifespan 50

	End

	Event
		Ename	GroundSmoke
		Type	StartPositOnly
		At	Origin

		Part	:VolcanicDustBig_Damage.part
		Part	:VolcanicDustBig2_Damage.part

		Lifespan 58
	End



	Event
		Ename 	Crack
		Type	StartPositOnly
		At	Origin
		Bhvr	:Splat_FootStomp.bhvr
		BhvrOverride
			PyrRotateJitter 0 0 0
			StartColor 255 150 50
			SplatFlags ADDITIVE
			Scale 22 0.0 22
		End
		Splat	crackedEarth.tga
		Lifespan 60

	End

End


Condition
	On	Time
	Time	5




	Event
		Ename	Steam
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			Scale		1.25 1.25 1.25
		End
		Part	:SteamAlpha_Damage.part
		Lifespan 45
	End

	Event
		Ename	GeyserCore
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			Scale		1.25 1.25 1.25

		End
		Part	:ViolentSteamCoreAdd_Damage.part

		Lifespan 20
	End

		Event
		Ename	GeyserCore2
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			Scale		1.25 1.25 1.25

		End
		Part	:ViolentSteamCoreBlue_Damage.part

		Lifespan 25
	End
End

#########################################################

End






























