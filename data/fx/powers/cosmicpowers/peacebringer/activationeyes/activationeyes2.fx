#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Eyes
End

Flags    InheritAlpha

LifeSpan	30
########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		sound	peaceeyes1 60 50 .73
		LifeSpan	30
	End

	Event
		Type	Local 
		At	Origin
		Sound peacelightform1 80 80 .8
	End
	

End
##################################

