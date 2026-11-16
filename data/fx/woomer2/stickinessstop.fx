#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 10000

Input  
	InpName	Origin
End

Condition

	Event
		EName   sword
		Type	Posit 
		At	Origin
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
End

End			