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
		ChildFX	:Celestial_Aura_Eyes_Female.fx
	End

	Event
		EName	ShouldersAnchor
		Type	Local
		At	Origin
		ChildFX	:Celestial_Aura_Shoulders.fx
	End

	Event
		EName	GlovesAnchor
		Type	Local
		At	Origin
		ChildFX	:Celestial_Aura_Gloves_Female.fx
	End
End

#############################################################

End