#############################################################
## Celestial_Aura_BootsAndShoulders_Huge.fx
#############################################################

FxInfo
Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ShouldersAnchor
		Type	Local
		At	Origin
		ChildFX	:Celestial_Aura_Shoulders_Huge.fx
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