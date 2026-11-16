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
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.2 0.65 -0.2
			PYRRotate	200 0 15
		End

		Part	:Light_Rays.part
		Part	:Light_Rays.part

		Part	:Light_Rays_Large_Steady2_Huge.part
		Part	:Light_Rays_Large_Steady2_Huge.part

		Part	:Light_Glow_Large.part
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.2 0.65 -0.2
			PYRRotate	200 0 -15
		End

		Part	:Light_Rays.part
		Part	:Light_Rays.part

		Part	:Light_Rays_Large_Steady2_Huge.part
		Part	:Light_Rays_Large_Steady2_Huge.part

		Part	:Light_Glow_Large.part
	End

#############################################################

End