#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	Origin
End
Input
	InpName Root
End

LifeSpan 160

Condition
	Time 0
	Event
		Type	Start
		At	Root
		Geom	FX_IceWeapon__Ice
		#Part1	:smoke.part
		#Part3	:Flash.part
		#Part5	:FlatFlash.part
	End

End

End	


			