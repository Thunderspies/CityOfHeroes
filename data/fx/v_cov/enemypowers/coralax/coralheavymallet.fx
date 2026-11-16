#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	WepR
End

Input  
	Inpname	Mystic
End

#LifeSpan	100

Condition
	On 	Time
	Time 	15

	Event
		EName 	mallet
		Type	Local 
		At	WepR
		Bhvr	behaviors/EarthWeapon.bhvr
		Geom	FX_GreaterCoralMallet	
	End

End


End