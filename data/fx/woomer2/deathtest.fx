#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	WepR
End

LifeSpan 200

Condition
	Event
		EName	Ed
		Type	Local
		At	WepR
		Geom	FX_IceWeapon
	End

	Event
		EName	Ed
		Type	Start
		At	WepR
		Geom	FX_FireWeapon
	End
End


End	


			