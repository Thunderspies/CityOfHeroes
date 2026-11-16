#############################################################
## AntimatterPulse.fx
#############################################################

FxInfo
Lifespan 300

#############################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	EndGame_Nuke 100 100 .8
	End
End

Condition
	On	Time
	Time	0

#	Event
#		Type	Start
#		At	Root
#		BhvrOverride
#			InitialVelocity	0 1.4 0
#			Drag		.09
#		End
#
#		Lifespan	45
#	End

	Event
		EName	MushroomAnchor
		Type	Start
		At	Root
		BhvrOverride
			InitialVelocity	0 1.4 0
			Drag		.065
		End
		Part	:Nuke_Glow_Rising.part
		Part	:Nuke_Flash.part
		Part	:Nuke_Smoke_Head.part
		Part	:Nuke_Smoke_Head_Continuing.part
		Part	:Nuke_Fire_Head.part
		Part	:Nuke_Flare_Rising.part
		Lifespan	45
	End


End

Condition
	On	Time
	Time	5

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PositionOffset	0 -7.5 0
			ShakeFallOff	0.005
		End
		Part	:Nuke_Fire_Rising_Column.part
	#	Part	:Nuke_Shockwave.part
		Lifespan	10
	End

	Event
		EName	RingAnchor
		Type	Local
		At	Root
		Bhvr	:Ground_Dust_Expand.bhvr
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		Part	:Nuke_Smoke_Ring_Expanding.part
		Part	:Nuke_Fire_Ring_Expanding.part
		Part	:Nuke_Shockwave_Flat.part

		Lifespan	50
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 -0.35 0
		End
		Part	:Nuke_Smoke_Ring_Central.part
		Lifespan	15
	End

	Event
		EName	Sigil
		Type	Start
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	5
			FadeOutLength	120
			StartColor	50 50 50
			Scale		20.0 15.0 20.0
		End
		Splat	Energy_ConeB3.tga
		Lifespan 220
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0 -12 0
			ShakeFallOff	0.005
		End
		Part	:Nuke_Smoke_Column.part
		Lifespan	50
	End

End

#############################################################

End
