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



LifeSpan 0

Condition
	Event
		Type	Local
		At	WepR
		Geom	FX_IceWeapon
		Part	:vistest.part
	End

End


End	


			