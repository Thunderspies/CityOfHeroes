#############################################################
## Celestial_Aura_Body_Female.fx
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
		EName	GlovesAnchor
		Type	Local
		At	Origin
		ChildFX	:Celestial_Aura_Gloves_Female.fx
	End

	Event
		EName	ShouldersAnchor
		Type	Local
		At	Origin
		ChildFX	:Celestial_Aura_Shoulders_Female.fx
	End
End

#############################################################

End