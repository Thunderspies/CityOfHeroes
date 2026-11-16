#########################################################
##	template

FxInfo

Input  
	Inpname	WepR
End

Input  
	Inpname	hair
End

Input  
	Inpname	origin
End


Flags    InheritAlpha
LifeSpan	51

Condition
	On	Time
	Time	4

	Event
		Type	Local
		At	Origin
		Sound Nemesisblade2 55 55 .7
	End

End



Condition
	On 	Time
	Time 	16

	Event
		EName	Blade
		Type	Local
		At	WepR
		Bhvr	Behaviors/JagerWeapon_Fade_UPBlades.bhvr
		Geom	GEO_WepR_Nemesis_saw
		
	End
End

Condition
	On 	Time
	Time 	51

	Event
		EName	Blade
		Type	Destroy
		
	End
End

End		