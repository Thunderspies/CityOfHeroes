#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 340

#############################################################

Input
	InpName Hips
End

#############################################################
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\CameraShake01Subtle.bhvr

		BhvrOverride
			PositionOffset	0.0 0.2 0.0
		End
		Part	:Toxic_Splash_Initial.part
		Part	:Toxic_Splash_Initial_Lingering.part

		Lifespan 4
	End
End


Condition
	On 	Time
	Time 	2


	Event
		Type	Local
		At	Root
		Bhvr	behaviors\CameraShake01Subtle.bhvr

		BhvrOverride
			PositionOffset	0.0 0.2 0.0
		End
		Part	:Explosion_Flash_Large.part
		Part	:Explosion_Toxic_Shockwave_Small.part
		Part	:Explosion_Toxic_Fire_Rings_Large.part
		Part	:Explosion_Flash_Large_Flat.part
		Part	:Explosion_Toxic_Glow_Lingering_Flat.part
		Part	:Explosion_Toxic_Shockwave_Flat.part
		Part	:Explosion_Toxic_Glow_Flat.part
		Part	:Toxic_Splash_Initial.part
		Lifespan 4
	End


	Event
		Type	PositOnly
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.3 0
			Scale			0.01 0.01 0.01
			ScaleRate		1.5 1.5 1.5
 			EndScale		2.25 2.25 2.25
		End
		Part	:Toxic_Steam_Ring.part
		Lifespan 10
	End

	Event
		EName	PuddleAnchor
		Type	Local
		At	Root
		Part	:Toxic_Burn_Flat.part
		Part	:Toxic_Rings_Flat.part
		Lifespan 250
	End


	Event
		Type	Local
		At	PuddleAnchor
		BhvrOverride
			PositionOffset	0.0 -2.0 0.0
		End
		Part	:Toxic_Steam.part
		Lifespan 250
	End
End

Condition
	On	Cycle
	Time	6

	Event
		Type	Local
		At	PuddleAnchor
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			StartJitter	5.0 0.0 5.0
		End
		ChildFX	:Child_Toxic_Bubbles.fx
		Lifespan 30
	End
End

Condition
	On	Time
	Time	8



	Event
		Type	Start
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	10
			FadeOutLength	40
			StartColor	25 30 20
			Scale		40.0 5.0 40.0
		End
		Splat	BlurredOilSplatDark.tga
		Lifespan 320
	End

	Event
		Type	Start
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		200
			FadeInLength	10
			FadeOutLength	40
			StartColor	23 25 19
			Scale		40.0 5.0 40.0
		End
		Splat	Energy_ConeB3.tga
		Lifespan 340
	End
End

#############################################################

End