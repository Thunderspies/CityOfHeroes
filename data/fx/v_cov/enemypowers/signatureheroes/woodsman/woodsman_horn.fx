#########################################################
##	jack
##
FxInfo
	
LifeSpan 100

Flags    InheritAlpha




Condition
	On	Time
	Time	15
	

	Event
		Type	Local
		At 	Origin
		Sound rams_horn 100 100 .96
	End
End



Condition
	On 	Time
	Time 	5

	Event
		EName FadeDown_Horn
		Type	Local 
		At	WepL
		Geom	WepL_Woodsman_Horn
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End			