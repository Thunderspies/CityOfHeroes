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


Condition
	Event
		EName 	Prime
		Type	Posit
		At	Root       
		bhvr	behaviors/_Jeremy/splat.bhvr
		Splat	Graphic_Star_01 Steam  
		#Splat	mistring01	woomersphere2
	End
End



End			