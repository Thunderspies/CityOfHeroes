#############################################################
## RuneCircle_Demons.fx
#############################################################

FxInfo
Lifespan 400

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type Local
		At Origin
		Sound Demon_SummonFX_01 80 80 1
	End


	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Enchant_Hands_Fire.part
		Part	:Enchant_Hands_Fire_Trail.part
		Part	:Enchant_Hands_Steam.part
		Part	:Enchant_Hands_Steam_Trail.part
		Lifespan 110
	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Enchant_Hands_Fire.part
		Part	:Enchant_Hands_Fire_Trail.part
		Part	:Enchant_Hands_Steam.part
		Part	:Enchant_Hands_Steam_Trail.part
		Lifespan 115
	End

End

Condition
	On	Time
	Time	20

	Event
		Ename	ConeOffset
		Type	Start
		At	Root
		GEOM	FX_ConeOffsets
	End

	Event
		EName 	RuneAnchor
		Type	Local
		At	ConeOffset
		BhvrOverride
			PositionOffset	0 5 5
		End
		AltPiv	1
	End

	Event
		Type	Local
		At	RuneAnchor
		BhvrOverride
			PyrRotate 	-90 180 0
		End
		Part	:Demons_Rune_Smoke.part
	End

End

Condition
	On	Time
	Time	30
	Event
		Type	Local
		At	RuneAnchor
		BhvrOverride
			PyrRotate 	-90 180 0
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Demons_Rune.part
	End
End

Condition
	On	Time
	Time	38

	Event
		EName	Sigil
		Type	Start
		At	T_Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			StartColor	255 255 255
			PrimaryTint	80
			SecondaryTint	20
			Scale		15.0 10.0 15.0
		End
		Splat	RuneCircle_OuterCircle.tga
		Lifespan 60
	End

	Event
		EName	Sigil
		Type	Start
		At	T_Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			StartColor	255 255 255
			PrimaryTint	40
			SecondaryTint	60
			Scale		15.0 10.0 15.0
		End
		Splat	RuneCircle_Dial.tga
		Lifespan 60
	End

	Event
		EName	Sigil
		Type	Start
		At	T_Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			StartColor	255 255 255
			PrimaryTint	50
			SecondaryTint	50
			PyrRotate	0 270 0
			Scale		3.5 3.0 3.5
		End
		Splat	RuneCircle_Rune2.tga
		Lifespan 60
	End
End

#########################################################

Condition
	On	Time
	Time	98

	Event
		Type StartPositOnly
		At T_Root
		Sound Demon_SpawnFX_Loop 80 80 1
		bhvr	behaviors/soundFade1.bhvr
		Lifespan 45
	End

	Event
		Ename 	CameraShake
		Type	StartPositOnly
		At	T_Root
		Bhvr	behaviors\CameraShake02Subtle.bhvr
		Lifespan 3
	End

	Event
		Ename 	Crack
		Type	StartPositOnly
		At	T_Root
		Bhvr	Emotes\CostumeChanges\Stomp\Splat_FootStomp.bhvr
		BhvrOverride
			Alpha		255
			PyrRotateJitter 0 180 0
			Scale 		28 10.0 28
		End
		Splat	StoneCracks.tga
		Lifespan 220

	End

End

Condition
	On	Time
	Time	96

	Event
		EName	Steam
		Type	Start
		At	T_Root
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
	Time	98

	Event
		Type	Start
		At	T_Root
		bhvr	behaviors\Demon_Quake2.bhvr
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			EndScale	1.0 1.0 1.0
		End
		Geom	FX_HellFissure
		Lifespan 220

	End
End
#########################################################

Condition
	On	Time
	Time	96

	Event
		Ename	InitialSteam
		Type	StartPositOnly
		At	T_Root

		Part	:Shockwave.part
		Part	:GasRadiusRocksBig.part
		Lifespan 1
	End

	Event
		Ename	GeyserHighlights
		Type	StartPositOnly
		At	T_Root
		BhvrOverride
			PositionOffset	0.0 -13.0 0.0
		End
		Part	:GeyserHighlightsArea.part
		Lifespan 40
	End


	Event
		Ename 	Flash
		Type	StartPositOnly
		At	T_Root
		Part	:LavaDrops_Continuous.part
		Lifespan 30

	End

	Event
		Ename	GroundSmoke
		Type	StartPositOnly
		At	T_Root
		Part	:VolcanicDustBig.part
		Part	:VolcanicDustBig2.part
		Lifespan 58
	End
End

Condition
	On	Time
	Time	102

	Event
		EName	FirePillar
		Type	StartPositOnly
		At	T_Root
		Part	:ViolentSteamCoreAdd_Light.part
		Part	:Fire_Burst.part
		Part	:Fire_Burst_Detached.part
		Lifespan 50
	End

	Event
		Ename	GeyserCore
		Type	StartPositOnly
		At	T_Root
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