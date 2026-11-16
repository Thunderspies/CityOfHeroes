#############################################################
## Child_OrbitalLance_Attack.fx
#############################################################

FxInfo
LifeSpan 30

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	21

	Event
		EName	HipsAnchor
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0.5 0.0 0.0
		End
	End

	Event
		EName	HipsAnchor2
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	-0.5 0.0 0.0
		End
	End

	Event
		EName	HipsAnchor3
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	-1.0 0.0 0.0
		End
	End

	Event
		EName 	Fade
		Type	Local
		At	Chest
		Bhvr	behaviors/WarpOut_Fade.bhvr
		Flags 	AdoptParentEntity
		Lifespan 3
	End

	Event
		Type	StartPositOnly
		At	HipsAnchor
		Bhvr	behaviors\CameraShake02.bhvr
		Part	:Explosion_Flash_Huge.part
		Part	:SelfDestruct_Glow_Huge.part
		Part	:SelfDestruct_Shockwave_Huge.part
		Part	:SelfDestruct_Glow_Lingering_Huge.part
		Part	:SelfDestruct_Streaks_Huge.part
		Lifespan 3
	End


	Event
		Type	StartPositOnly
		At	HipsAnchor2
		Bhvr	behaviors\CameraShake01.bhvr
	#	Part	:SelfDestruct_Shockwave.part
	#	Part	:SelfDestruct_Glow_Flat.part
		Lifespan 3
	End

End

Condition
	On	Time
	Time	22

	Event
		Type	PositOnly
		At	HipsAnchor2
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.3 0
			Scale			0.01 0.01 0.01
			ScaleRate		1.5 1.5 1.5
 			EndScale		3.0 3.0 3.0
		End
		Part	:FireRing_Small.part
		Part	:FireRing_Highlight.part
		Lifespan 60
	End

	Event
		Type	PositOnly
		At	HipsAnchor
		Part	:Explosion_Fire_Blue.part
		Part	:Explosion_Fire_Yellow.part
		Lifespan 10
	End

	Event
		Type	StartPositOnly
		At	HipsAnchor3

		BhvrOverride
			PositionOffset		-1.0 0.0 0
		#	PyrRotate		0.0 0.0 -40.0
		End
		Part	:Explosion_Fire_Yellow_Flat_Huge.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	24

	Event
		EName	Sigil
		Type	StartPositOnly
		At	Hips
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	5
			FadeOutLength	40
			StartColor	20 20 20
			Scale		80.0 10.0 80.0
		End
		Splat	Energy_ConeB3.tga
		Lifespan 190
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0.0 5.0 0.0
		End
		Part	:SelfDestruct_Smoke_Lingering.part
		Part	:SelfDestruct_Smoke.part
		Lifespan 1
	End

End

Condition
	On	Time
	Time	27

	Event
		Type	Start
		At	HipsAnchor
		BhvrOverride
			PositionOffset	0.0 5.0 0.0
		End
		Part	:Globe_Sparks.part
		Part	:Globe_Cinders.part
		Lifespan 1
	End

End

#############################################################

End