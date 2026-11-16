#############################################################
## Death_Demon.fx
#############################################################

FxInfo
Lifespan 220

#############################################################

Condition
	On	Time
	Time	15
	Event
		Ename 	CameraShake
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors\CameraShakeSubtle.bhvr
		Part	:Shockwave.part

		Lifespan 1
	End

	Event
		Ename	GeyserHighlights
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -13.0 0.0
		End
		Part	:GeyserHighlightsArea_Wide.part
		Lifespan 20
	End

	Event
		EName	Sigil
		Type	Start
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			StartColor	255 255 255
			PrimaryTint	80
			SecondaryTint	20
			Scale		25.0 25.0 25.0
			SplatFlags	ADDITIVE
			PyrRotate	0 180 0
		End
		Splat	crackedEarth.tga
		Lifespan 90
	End
End

#########################################################

Condition
	On	Time
	Time	30

	Event
		Ename 	CameraShake
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors\CameraShake02Subtle.bhvr
		BhvrOverride
			PositionOffset	0 -2 0
		End
		Part	:Rocks_Additive.part
		Part	:Rocks_Alpha.part
		Lifespan 1
	End
End
#########################################################

Condition
	On	Time
	Time	40

	Event
		Ename 	Flash
		Type	StartPositOnly
		At	Root
		Part	:LavaDrops_Continuous.part
		Lifespan 30

	End

	Event
		Ename	GroundSmoke
		Type	StartPositOnly
		At	Root
		Part	:Fire_Burst.part
		Part	:VolcanicDustBig.part
		Part	:VolcanicDustBig2.part
		Lifespan 58
	End
End

Condition
	On	Time
	Time	35

	Event
		Ename	GeyserCore
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Fire_Ash.part
		Part	:Fire_Ash_Additive.part
		Part	:Fire_Ash_Individual.part
		Part	:Fire_Ash_Origin.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	31

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Lava_Splash.part
		Part	:Lava_Splash_Bright.part
		Lifespan 10
	End

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Death_FirePillar.part
		Part	:Lava_Splash_Consistant.part
		Lifespan 60
	End

End

Condition
	On	Time
	Time	99

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Lava_Splash.part
		Part	:Lava_Splash_Bright.part
		Part	:Fire_Ash_Additive.part
		Part	:Fire_Ash_Individual.part
		Lifespan 10
	End

End
#########################################################

End