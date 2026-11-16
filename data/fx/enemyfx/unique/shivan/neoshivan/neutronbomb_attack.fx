#########################################################

FxInfo
LifeSpan	300

#########################################################

Input
	InpName	Hips
End

#########################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	HandAnchorR
		Type	PositOnly
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Hands_Core.part
		Part	:Hands_Rays_Heavy.part
		Part	:Hands_Rings_Heavy.part
		Lifespan 70
	End

	Event
		EName	HandAnchorL
		Type	PositOnly
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Hands_Core.part
		Part	:Hands_Rays_Heavy.part
		Part	:Hands_Rings_Heavy.part
		Lifespan 70
	End

End

Condition
	On 	Cycle
	Time 	8

	Event
		Type	PositOnly
		At	HandAnchorR
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.35 0.0
		End
		part	:Electron_Trail_Stationary_Hand.part
		Lifespan 15
	End

	Event
		Type	PositOnly
		At	HandAnchorL
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.35 0.0
		End
		part	:Electron_Trail_Stationary_Hand.part
		Lifespan 15
	End

End

Condition
	On	Time
	Time	22

	Event
		EName	BallAnchor
		At 	WepR
		Type 	Local
		BhvrOverride
			PositionOffset	1.0 -0.8 0.0
		End

		LifeSpan 70
	End

	Event
		EName	Glows
		At 	BallAnchor
		Type 	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:NeutronBomb_Core.part
		Part	:NeutronBomb_Rays.part
		Part	:NeutronBomb_Border.part
		LifeSpan 29
	End
End

Condition
	On 	Time
	Time 	30

	Event
		At	BallAnchor
		Type	PositOnly
		Bhvr	:NeutronBomb_Ball_Scale.bhvr
		Geom 	Tintable_ShadowField
		LifeSpan 20
	End

	Event
		EName 	CrushingHateGeo
		At	BallAnchor
		Type 	PositOnly
		Bhvr 	:NeutronBomb_Ball_Additive.bhvr
		Geom 	FX_Singularity_Highlights
		LifeSpan 20
	End
End


Condition
	On 	Time
	Time 	63

	Event
		EName 	Prime
		Type	Start
		At	Chest
		Bhvr	:NeutronBomb_Projectile_Alpha.bhvr
		Geom 	Tintable_ShadowField
		Part	:NeutronBomb_Border.part
		Part	:NeutronBomb_Core.part
		Part	:NeutronBomb_Rays.part
		Part	:NeutronBomb_Smoke.part
		Magnet	T_Chest
		LookAt	T_Chest

	End

	Event
		EName 	Prime2
		Type	PositOnly
		At	Prime
		Bhvr	:NeutronBomb_Ball_Additive.bhvr
		BhvrOverride
			Spin	0.0 0.2 0.0
		End
		Geom 	FX_Singularity_Highlights
	End

End

Condition
	On	PrimeHit

	Event
		Ename	Explosion
		Type	Local
		At	T_Chest
		ChildFx :HeavyPunch_Hit.fx
	End

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Prime2
		Type	Destroy
	End
End

#########################################################

End





