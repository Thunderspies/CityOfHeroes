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
		EName	BootsAnchor
		Type	Local
		At	Origin
		ChildFX	:Celestial_Aura_Boots_Huge.fx
	End

	Event
		EName	GlovesAnchor
		Type	Local
		At	Origin
		ChildFX	:Celestial_Aura_Gloves_Huge.fx
	End
End

#############################################################

End