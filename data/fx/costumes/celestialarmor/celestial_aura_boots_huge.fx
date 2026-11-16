#############################################################
## Wolf Spider Helmet - Glow
#############################################################

FxInfo
Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	LegHighL
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.5 -0.2 -0.25
			PYRRotate	0 -40 130
		End
		Part	:Light_Rays_Large_Steady2_Huge.part
		Part	:Light_Rays_Large2_Huge.part
		Part	:Light_Glow_Large_Sparse.part
	End

	Event
		EName	LegLowL
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.45 -0.45 -0.1
			PYRRotate	0 -40 130
		End
		Part	:Light_Rays_Steady.part
		Part	:Light_Rays.part
	End

	Event
		EName	FootHighL
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.45 -1.15 -0.35
			PYRRotate	0 -45 140
		End
		Part	:Light_Rays.part
		Part	:Light_Rays_Steady.part
		Part	:Light_Glow.part
	End

	Event
		EName	FootLowL
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.45 -1.3 -0.35
			PYRRotate	0 -45 140
		End
		Part	:Light_Rays_Small_Steady.part
	End

#############################################################

	Event
		EName	LegHighR
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.5 -0.2 -0.25
			PYRRotate	0 40 -130
		End
		Part	:Light_Rays_Large_Steady2.part
		Part	:Light_Rays_Large2.part
		Part	:Light_Glow_Large_Sparse.part
	End

	Event
		EName	LegLowR
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.45 -0.45 -0.1
			PYRRotate	0 40 -130
		End
		Part	:Light_Rays_Steady.part
		Part	:Light_Rays.part
	End

	Event
		EName	FootHighR
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.45 -1.15 -0.35
			PYRRotate	0 45 -140
		End
		Part	:Light_Glow.part
		Part	:Light_Rays.part
		Part	:Light_Rays_Steady.part
	End

	Event
		EName	FootLowR
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.45 -1.3 -0.35
			PYRRotate	0 45 -140
		End
		Part	:Light_Rays_Small_Steady.part
	End

#############################################################

End