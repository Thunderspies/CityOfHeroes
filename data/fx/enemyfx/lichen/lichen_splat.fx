#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Root
End


Flags    InheritAlpha

Condition
	Event
		EName 	Prime
		Type	Posit
		At	Root       
		bhvr	_ChristopherStuff/fungussplat/splat.bhvr
		Splat	Mold_02.tga white  
		#Splat	mistring01	woomersphere2
	End
End



End			