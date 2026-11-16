#############################################################
## RuneCircle_Demonlings.fx
#############################################################

FxInfo
Lifespan 200

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
		Part	:Demonlings_Rune_Smoke.part
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
		Part	:Demonlings_Rune.part
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
			StartColor	150 255 50
			Scale		15.0 10.0 15.0
			SplatFlags	ADDITIVE
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
			StartColor	255 240 20
			Scale		15.0 10.0 15.0
			SplatFlags	ADDITIVE
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
			StartColor	255 230 100
			Scale		3.5 2.0 3.5
			SplatFlags	ADDITIVE
		End
		Splat	RuneCircle_Rune1.tga
		Lifespan 60
	End
End

Condition
	On	Time
	Time	96

	Event
		Type StartPositOnly
		At T_Root
		Sound Demon_SpawnFX_Loop 80 80 .75
		bhvr	behaviors/soundFade1.bhvr
		Lifespan 60
	End


	Event
		EName	Steam
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0 .1 0
		End
		Part	:Lava.part
		Part	:Lava_Fill.part
		Lifespan 140
	End

End

Condition
	On	Time
	Time	98

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
			StartColor 	200 255 25
			Scale 32 10.0 32
		End
		Splat	StoneCracks.tga
		Lifespan 80

	End

	Event
		Type	start
		At	T_Root
		bhvr	behaviors\Demon_Quake2.bhvr
		BhvrOverride
			PositionOffset 	0 -1.5 0
			InitialVelocity	0 .75 0
			Drag		.4
			StartColor 	90 70 50
 			EndScale	0.65 1.75 0.65
		End
		Geom	Tintable_Fault_Lava_01
		Lifespan 140
	End

	Event
		Type	start
		At	T_Root
		bhvr	behaviors\Demon_Quake2.bhvr
		BhvrOverride
			PositionOffset 	0 -1.5 0
			InitialVelocity	0 .75 0
			Drag		.4
			StartColor 	200 255 25
			FadeOutLength	0
 			EndScale	0.65 1.75 0.65
		End
		Geom	Tintable_Fault_Lava_01_AddGlow
		Lifespan 140
	End

	Event
		Type	start
		At	T_Root
		bhvr	behaviors\Demon_Quake.bhvr
		BhvrOverride
			PositionOffset 	0 -1.5 0
			InitialVelocity	0 .5 0
			Drag		.3
			StartColor 	90 70 50
 			EndScale	0.65 1.75 0.65
		End
		Geom	Tintable_Fault_Lava_02
		Lifespan 140
	End

	Event
		Type	start
		At	T_Root
		bhvr	behaviors\Demon_Quake.bhvr
		BhvrOverride
			PositionOffset 	0 -1.5 0
			InitialVelocity	0 .5 0
			Drag		.3
			StartColor 	200 255 25
			FadeOutLength	0
 			EndScale	0.65 1.75 0.65
		End
		Geom	Tintable_Fault_Lava_02_AddGlow
		Lifespan 140
	End

	Event
		Type	start
		At	T_Root
		bhvr	behaviors\Demon_Quake2.bhvr
		BhvrOverride
			PositionOffset 0 -1.5 0
			InitialVelocity	0 .75 0
			Drag		.4
			StartColor 	90 70 50
 			EndScale	0.65 1.75 0.65
		End
		Geom	Tintable_Fault_Lava_03
		Lifespan 140
	End

	Event
		Type	start
		At	T_Root
		bhvr	behaviors\Demon_Quake2.bhvr
		BhvrOverride
			PositionOffset 0 -1.5 0
			InitialVelocity	0 .75 0
			Drag		.4
			StartColor 	200 255 25
			FadeOutLength	0
 			EndScale	0.65 1.75 0.65
		End
		Geom	Tintable_Fault_Lava_03_AddGlow
		Lifespan 140
	End

	Event
		Type	start
		At	T_Root
		bhvr	behaviors\Demon_Quake.bhvr
		BhvrOverride
			PositionOffset 0 -1.5 0
			InitialVelocity	0 .5 0
			Drag		.3
			StartColor 	90 70 50
 			EndScale	0.65 1.75 0.65
		End
		Geom	Tintable_Fault_Lava_04
		Lifespan 140
	End

	Event
		Type	start
		At	T_Root
		bhvr	behaviors\Demon_Quake.bhvr
		BhvrOverride
			PositionOffset 0 -1.5 0
			InitialVelocity	0 .5 0
			Drag		.3
			StartColor 	200 255 25
			FadeOutLength	0
 			EndScale	0.65 1.75 0.65
		End
		Geom	Tintable_Fault_Lava_04_AddGlow
		Lifespan 140
	End

	Event
		Type	start
		At	T_Root
		bhvr	behaviors\Demon_Quake2.bhvr
		BhvrOverride
			PositionOffset 0 -1.5 0
			InitialVelocity	0 .75 0
			Drag		.4
			StartColor 	90 70 50
 			EndScale	0.65 1.75 0.65
		End
		Geom	Tintable_Fault_Lava_05
		Lifespan 140
	End

	Event
		Type	start
		At	T_Root
		bhvr	behaviors\Demon_Quake2.bhvr
		BhvrOverride
			PositionOffset 0 -1.5 0
			InitialVelocity	0 .75 0
			Drag		.4
			StartColor 	200 255 25
			FadeOutLength	0
 			EndScale	0.65 1.75 0.65
		End
		Geom	Tintable_Fault_Lava_05_AddGlow
		Lifespan 140
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
		Ename	GeyserCore
		Type	StartPositOnly
		At	T_Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			Scale		1.25 1.25 1.25
		End
		Part	:Fire_Ash_Light.part
		Part	:Fire_Ash_Individual.part
		Part	:Fire_Ash_Origin.part
		Lifespan 60
	End
End


#########################################################

End