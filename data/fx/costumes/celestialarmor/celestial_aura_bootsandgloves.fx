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
		ChildFX	:Celestial_Aura_Boots.fx
	End

	Event
		EName	BootsAnchor
		Type	Local
		At	Origin
		ChildFX	:Celestial_Aura_Gloves.fx
	End
End

#############################################################

End