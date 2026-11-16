#############################################################
## SteamEmote.fx
#############################################################

FxInfo
Lifespan 200

#########################################################
#################### GEYSER ########################
#########################################################

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SP_PressureReleaseFly_01 100 100 .8
	End

	Event
		Ename	Steam
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PositionOffset	0.0 -7.5 0.0
			Scale		1.25 1.25 1.25
		End
		Part	:ViolentSteamCoreAdd_Initial.part
		Part	:ViolentSteamCoreAdd_Initial2.part
		Lifespan 5
	End

	Event
		Type	StartPositOnly
		At	Root
		Bhvr	:Dust_Expanding.bhvr
		Part	:Dust_Expanding.part
		Part	:Dust_Expanding_Lingering.part
		Part	:Dust_Shockwave.part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	5


	Event
		Ename	Steam
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -7.5 0.0
			Scale		1.25 1.25 1.25
		End
		Part	:SteamAlpha.part
		Part	:ViolentSteamCoreAdd.part

		Lifespan 30
	End
End

Condition
	On	Time
	Time	10

	Event
		Ename	GeyserCore2
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			Scale		1.25 1.25 1.25
		End
		Part	:ViolentSteamCoreBlue.part

		Lifespan 30
	End

	Event
		Ename	Steam
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -7.5 0.0
		End
		Part	:GeyserHighlightsArea.part
		Lifespan 30
	End
End




























