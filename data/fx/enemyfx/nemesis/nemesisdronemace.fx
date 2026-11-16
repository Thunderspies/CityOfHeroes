#########################################################
##	template

FxInfo

Input  
	Inpname	WepR
End

Input  
	Inpname	mystic
End

Input  
	Inpname	Origin
End

Flags    InheritAlpha

LifeSpan	30

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Sound nemesismace1 55 55 .7
	End
	
End

Condition
	On 	Time
	Time 	10

	Event
		EName	Blade
		Type	Local
		At	WepR
		Bhvr	Behaviors/JagerWeapon_Fade_UP.bhvr
		Geom	GEO_WepR_Nemesis_mace
		Sound whoosh2 55 55 .5
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	Blade
		Type	Destroy
		
	End
End

End		