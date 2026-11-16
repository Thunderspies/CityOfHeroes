#############################################################
## Celestial_Aura_BootsAndEyes_Female.fx
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
		EName	BootsAnchor
		Type	Local
		At	Origin
		ChildFX	:Celestial_Aura_Boots.fx
	End
End

#############################################################

End