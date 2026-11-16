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
		EName	EyeAnchor
		Type	Local
		At	Origin
		ChildFX	:Celestial_Aura_Eyes_Male.fx
	End

	Event
		Type	Local
		At	LArmL
		BhvrOverride
			PositionOffset	-0.4 0.3 0.0
			PYRRotate	180 0 -35
		End
		Part	:Light_Rays.part
		Part	:Light_Rays_Steady.part
		Part	:Light_Glow.part

	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	0.4 0.3 0.0
			PYRRotate	180 0 35
		End
		Part	:Light_Rays.part
		Part	:Light_Rays_Steady.part
		Part	:Light_Glow.part
	End

	Event
		Type	Local
		At	HandL
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
			PYRRotate	180 0 -35
		End
		Part	:Light_Rays_Small.part
		Part	:Light_Rays_Small_Steady.part
	End

	Event
		Type	Local
		At	HandR
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
			PYRRotate	180 0 35
		End
		Part	:Light_Rays_Small.part
		Part	:Light_Rays_Small_Steady.part
	End

#############################################################

End