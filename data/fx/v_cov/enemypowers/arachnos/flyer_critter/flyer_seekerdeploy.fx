#########################################################
##	
##

FxInfo
LifeSpan  180

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	origin	
		Sound ArachnosSeekerDrone_deploy 180 180 .7
		
	End
End

end
