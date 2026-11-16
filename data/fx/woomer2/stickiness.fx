#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 10000

Input  
	InpName	Hair
End
Input  
	InpName	SpadR
End
Input  
	InpName	SpadL
End
Input  
	InpName	UarmL
End
Input  
	InpName	UarmR
End
Input  
	InpName	LarmL
End
Input  
	InpName	LArmR
End
Input  
	InpName	FootL
End
Input  
	InpName	FootR
End
Input  
	InpName	LLegR
End
Input  
	InpName	LLegL
End
Input  
	InpName	ULegR
End
Input  
	InpName	ULegL
End
Input  
	InpName	hips
End
Input  
	InpName	HandR
End
Input  
	InpName	HandL
End

Condition

	Event
		EName   sword
		Type	posit 
		At	LarmL
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	LArmR
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	HandR
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	HandL
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	UArmL
		##Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		##Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	UArmR
		##Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		##Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	FootR
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	FootL
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	LlegL
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	LLegR
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	ULegR
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
	Event
		EName   sword
		Type	posit 
		At	ULegL
		Part	:TorchflameA.part
		#Part	:TorchflameStop.part
		Part	:TorchBase.part
		#Geom	FX_FireWeapon

	End
End

End			