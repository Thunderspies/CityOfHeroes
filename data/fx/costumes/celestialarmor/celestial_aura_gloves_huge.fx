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
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.4 0.25 0.0
			PYRRotate	180 0 -35
		End
		Part	:Light_Rays.part
		Part	:Light_Rays_Large_Steady.part
		Part	:Light_Glow.part

	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.4 0.25 0.0
			PYRRotate	180 0 35
		End
		Part	:Light_Rays.part
		Part	:Light_Rays_Large_Steady.part
		Part	:Light_Glow.part
	End

	Event
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.2 0.0
			PYRRotate	180 0 -35
		End
		Part	:Light_Rays_Huge.part
		Part	:Light_Rays_Steady_Huge.part
	End

	Event
		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.2 0.0
			PYRRotate	180 0 35
		End
		Part	:Light_Rays_Huge.part
		Part	:Light_Rays_Steady_Huge.part
	End

#############################################################

End