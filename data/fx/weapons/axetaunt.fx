#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress


Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime
		Type	Local
		At	WepR
		##Geom	GEO_WepR_Axe_3
		Sound AxeTaunt_01 70 70 .6
		Sound AxeTaunt_02 70 70 .6
		Sound AxeTaunt_03 70 70 .6
		Sound AxeTaunt_04 70 70 .6
		Sound AxeTaunt_05 70 70 .6
		##Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End