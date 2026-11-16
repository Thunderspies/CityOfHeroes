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
		Sound 	SP_PressureRelease_01 100 100 .8
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
	TIme	0
	Repeat	5

	Event
		Type	start
		At	Root
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset		0.0 0.5 0.0
			StartJitter		2.0 0.0 2.0
			Scale			1.0 1.0 1.0
			InitialVelocity		0.0 1.5 0.0
			InitialVelocityJitter	0.5 0.5 0.5
			StartColor		255 240 192
			PyrRotateJitter		180 180 180
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End
		Geom	Earth_FXDebris01A

		CEvent	:CHILD_RockBreak.fx
		CDestroy	1
		Lifespan 500
		LifespanJitter	90
	End
End

Condition
	On	Time
	Time	5

	Event
		Ename 	Crack
		Type	StartPositOnly
		At	Root
		Bhvr	:Splat_FootStomp.bhvr
		BhvrOverride
			PyrRotateJitter 0 0 0
			StartColor 100 95 90
			Scale 15 0.0 15
		End
		Splat	Crater_Cracks2.tga
		Lifespan 120
	End

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




























