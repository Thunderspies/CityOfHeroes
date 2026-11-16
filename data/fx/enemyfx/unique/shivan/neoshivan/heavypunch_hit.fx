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
		At	Chest
		Sound 	EndGame_Nuke 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	MushroomAnchor
		Type	Start
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			InitialVelocity	0 1.2 0
			Drag		.065
		End
		Part	:Nuke_Flash.part
		Part	:Nuke_Flash_Rays.part
		Part	:Nuke_Smoke_Head.part
		Part	:Nuke_Smoke_Head_Continuing.part
		Part	:Nuke_Fire_Head.part
		Part	:Nuke_Boils_Head.part
		Lifespan	20
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PositionOffset	0 2 0
			ShakeFallOff	0.005
		End
		Part	:Nuke_Fire_Rising_Column.part
		Part	:Nuke_Shockwave.part
		Lifespan	10
	End

	Event
		EName	RingAnchor
		Type	Local
		At	Root
		Bhvr	:Ground_Dust_Expand.bhvr
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		Part	:Nuke_Smoke_Ring_Expanding.part
		Part	:Nuke_Fire_Ring_Expanding.part
		Part	:Nuke_Shockwave_Flat.part

		Lifespan	50
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	StartPositOnly
		At	Chest
		BhvrOverride
			PositionOffset	0 -3.0 0
		End
		Part	:Nuke_Smoke_Column.part
		Lifespan	10
	End

End

#############################################################

End
