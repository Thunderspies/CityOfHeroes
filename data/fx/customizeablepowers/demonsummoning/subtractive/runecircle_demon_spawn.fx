#############################################################
## RuneCircle_Demons.fx
#############################################################

FxInfo
Lifespan 400

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type 	StartPositOnly
		At 	Origin
		Sound 	Demon_SpawnFX_Loop 80 80 1
		bhvr	behaviors/soundFade1.bhvr
		Lifespan 45
	End

	Event
		Ename 	CameraShake
		Type	StartPositOnly
		At	Origin
		Bhvr	behaviors\CameraShake02Subtle.bhvr
		Lifespan 3
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	Steam
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 .1 0
		End
		Part	:Lava.part
		Part	:Lava_Fill.part
		Lifespan 230
	End

End

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Origin
		bhvr	behaviors\Demon_Quake2.bhvr
		BhvrOverride
			PositionOFfset	0.0 1.0 0.0
			EndScale	1.0 1.0 1.0
		End
		Geom	FX_HellFissure
		Lifespan 240
	End
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Ename	InitialSteam
		Type	StartPositOnly
		At	Origin

		Part	:Shockwave.part
		Part	:GasRadiusRocksBig.part
		Lifespan 1
	End

	Event
		Ename	GeyserHighlights
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -13.0 0.0
		End
		Part	:GeyserHighlightsArea.part
		Lifespan 40
	End


	Event
		Ename 	Flash
		Type	StartPositOnly
		At	Origin
		Part	:LavaDrops_Continuous.part
		Lifespan 30

	End

	Event
		Ename	GroundSmoke
		Type	StartPositOnly
		At	Origin
		Part	:VolcanicDustBig.part
		Part	:VolcanicDustBig2.part
		Lifespan 58
	End
End

Condition
	On	Time
	Time	6

	Event
		EName	FirePillar
		Type	StartPositOnly
		At	Origin
		Part	:ViolentSteamCoreAdd_Light.part
		Part	:Fire_Burst.part
		Part	:Fire_Burst_Detached.part
		Lifespan 50
	End

	Event
		Ename	GeyserCore
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Fire_Ash.part
		Part	:Fire_Ash_Individual.part
		Part	:Fire_Ash_Origin.part
		Lifespan 65
	End


End


#########################################################

End