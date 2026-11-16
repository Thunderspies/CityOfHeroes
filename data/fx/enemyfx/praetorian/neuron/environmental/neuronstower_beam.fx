#############################################################
## NeuronsTower_Beam.fx
#############################################################

FxInfo
Flags DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MidAnchor
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -165.0 0.0
		End
		Sound NeuronsBeam_Loop 700 500 .5
	End

	Event
		EName	PortalAnchor
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 -350.0 0.0
		End
		ChildFX	:NeuronsTower_Portal.fx
		Sound NeuronsBeamDist_Loop 1000 1000 .5
	End

	Event
		EName	Anchor
		Type	StartPositOnly
		At	Root
	End

	Event
		EName	TopAnchor
		Type	Start
		At	Anchor
		BhvrOverride
			PositionOffset	0.0 170.0 0.0
		End
		Part	:Projectile_Base_Top.part
		Part	:Projectile_Base_Flat.part
		POther	Anchor
		LookAt	Anchor
	End

	Event
		EName	BottomAnchor
		Type	Start
		At	Anchor
		BhvrOverride
			PositionOffset	0.0 -320 0.0
		End
		Sound NeuronsBeamECU_Loop 200 200 .3
	End

	Event
		EName	BottomAnchorC
		Type	Start
		At	Anchor
		BhvrOverride
			PositionOffset	0.0 -380.0 0.0
		End
		POther	MidAnchor
		LookAt	MidAnchor
	End

	Event
		EName	BottomAnchorB
		Type	Start
		At	Anchor
		BhvrOverride
			PositionOffset	0.0 -430.0 0.0
		End
		Part	:Projectile_Trail_Electricity_Blur.part
		Part	:Projectile_Trail_Electricity_Blur2.part
		Part	:Projectile_Trail_Electricity.part
		Part	:Projectile_Trail_Electricity2.part
		Part	:Projectile_Trail_Core.part
		Part	:Projectile_Base_Flat.part
		POther	BottomAnchorC
		LookAt	BottomAnchorC
	End

	Event
		Type	StartPositOnly
		At	Anchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Projectile_Trail_Electricity_Blur.part
		Part	:Projectile_Trail_Electricity_Blur2.part
		Part	:Projectile_Trail_Electricity.part
		Part	:Projectile_Trail_Electricity2.part
		Part	:Projectile_Trail_Core.part
		Part	:Projectile_Base.part
		POther	TopAnchor
		LookAt	TopAnchor
	End

	Event
		Type	StartPositOnly
		At	MidAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Projectile_Trail_Electricity_Blur.part
		Part	:Projectile_Trail_Electricity_Blur2.part
		Part	:Projectile_Trail_Electricity.part
		Part	:Projectile_Trail_Electricity2.part
		Part	:Projectile_Trail_Core.part
		Part	:Projectile_Base_Top.part
		Part	:Projectile_Base_Flat.part
		POther	BottomAnchor
		LookAt	BottomAnchor
	End
End

Condition
	On	Cycle
	Time	6
	Chance	0.3

	Event
		Type	StartPositOnly
		At	Anchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Projectile_Trail_Electricity_Large.part
		POther	TopAnchor
		LookAt	TopAnchor
		Lifespan 	5
		LifespanJitter	2
	End

	Event
		Type	StartPositOnly
		At	MidAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Projectile_Trail_Electricity_Large.part
		POther	BottomAnchor
		LookAt	BottomAnchor
		Lifespan 	5
		LifespanJitter	2
	End
End

#############################################################

End