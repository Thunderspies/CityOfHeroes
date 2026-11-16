#############################################################
## MachineGunHitLong.fx
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BR_Overcharge_Hit_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	HipsAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.5 0.0 0.0
		End
	End

	Event
		EName	HipsAnchor2
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-0.5 0.0 0.0
		End
	End

	Event
		EName	HipsAnchor3
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-1.0 0.0 0.0
		End
	End

	Event
		Type	StartPositOnly
		At	HipsAnchor
		Part	:Explosion_Flash_Huge.part
		Part	:SelfDestruct_Glow_Huge.part
		Part	:SelfDestruct_Shockwave_Huge.part
		Part	:SelfDestruct_Glow_Lingering_Huge.part
		Part	:SelfDestruct_Streaks_Huge.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	1

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
		Part	:Explosion_Fire.part
		Lifespan 10
	End

	Event
		Type	StartPositOnly
		At	HipsAnchor3
		BhvrOverride
			PositionOffset		-1.0 0.0 0
		End
		Part	:Explosion_Fire_Flat_Huge.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	4

	Event
		EName	Sigil
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		150
			FadeInLength	5
			FadeOutLength	120
			StartColor	100 100 100
			Scale		80.0 10.0 80.0
		End
		Splat	ExplosiveResidue.tga
		Lifespan 190
	End
End

#############################################################

End