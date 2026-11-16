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
		EName 	Prime
		Type	Local 
		At	Head
			
		Sound Wolfpackmech_loop 45 45 .27
	
	End
	
	Event
		EName 	light
		Type	Local 
		At	LarmR
		Bhvr	Behaviors/MekManScale.bhvr
		Geom	FX_WolfPackLight
	
	End

	Event
		EName 	Gun
		Type	Local 
		At	light
		 AltPiv 1
		#Part1	:LazerLight.part
		Part2	:GunLightPulsing.part
		
	End

End

End
##################################

