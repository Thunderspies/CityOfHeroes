#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Head
End

Input  
	Inpname	LarmR
End

Flags    InheritAlpha

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Gun
		Type	Local 
		At	chest
		Part1	:LazerLight.part
		Part2	:GunLightPulsing.part
		
	End

End

End
##################################

