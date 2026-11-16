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
		Geom	FX_WolfPack
		Sound Wolfpackmech_loop 80 80 .35
	
	End
	
	Event
		EName 	light
		Type	Local 
		At	LarmR
		Geom	FX_WolfPackLight
	
	End

	Event
		EName 	Eyez1
		Type	local 
		At	Prime
		 AltPiv 1
		Part1	:WolfPackEyesMotion2.part
		Part2	:WolfPackPupil2.part
		PMagnet	Eyez2
		
	End
	
	Event
		EName 	Eyez2
		Type	Local 
		At	Prime
		 AltPiv 2
		#Part1	:WolfPackEyes.part
		#Part2	:StationaryPupil.part
		
	End

	Event
		EName 	Eyez3
		Type	Local 
		At	Prime
		 AltPiv 3
		Part1	:WolfPackEyesMotion.part
		Part2	:WolfPackPupil.part
		PMagnet	Eyez4
		
	End
	
	Event
		EName 	Eyez4
		Type	Local 
		At	Prime
		 AltPiv 4
		#Part1	:WolfPackEyes.part
		#Part2	:StationaryPupil.part
		
	End

	Event
		EName 	Gun
		Type	Local 
		At	light
		 AltPiv 1
		Part1	:LazerLight.part
		Part2	:GunLightPulsing.part
		
	End

End

End
##################################

