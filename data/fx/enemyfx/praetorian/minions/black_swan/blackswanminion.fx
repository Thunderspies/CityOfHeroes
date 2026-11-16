############################################################
##	Fireweapon
###########################################################
FxInfo



########################################################################
##arms
########################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	Back
		Part1	:FireElementalBack.part	
	End
	Event
		EName	Chest
		Type	Local
		At	UarmR
		Part1	:FireElementalFireLegs2.part
		Pmagnet LArmR
	
	End
	Event
		EName	Head
		Type	Local
		At	LArmR
		Part1	:FireElementalFirearm2.part
		Pmagnet WepR
	
	End
	Event
		EName	Chest
		Type	Local
		At	UarmL
		Part1	:FireElementalFireLegs2.part
		Pmagnet LArmL
	
	End
	Event
		EName	Head
		Type	Local
		At	LArmL
		Part1	:FireElementalFirearm2.part
		Pmagnet WepL
	
	End
	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireElementalHandz.part
		
	
	End
	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireElementalHandz.part
		
	
	End
	Event
		EName	Chest
		Type	Local
		At	hips
		Part1	:FireElementalFire2.part
		
	
	End
	Event
		EName	Head
		Type	Local
		At	Head
		Part1	:FireElementalFire.part
		
	
	End
	Event
		EName	Neck
		Type	Local
		At	Neck
		#Part1	:FireElementalFire.part
		
	
	End

####################################################################################
##Legs
#################################################################################


	Event
		EName	Chest
		Type	Local
		At	FootL
		Part1	:FireElementalFireLegs.part
		Pmagnet LlegL
	
	End
	Event
		EName	Head
		Type	Local
		At	FootR
		Part1	:FireElementalFireLegs.part
		Pmagnet LlegR
	
	End
	Event
		EName	Chest
		Type	Local
		At	LlegL
		#Part1	:FireElementalFireLegsTop.part
		Pmagnet UlegL
	
	End
	Event
		EName	Head
		Type	Local
		At	LlegR
		#Part1	:FireElementalFireLegsTop.part
		Pmagnet UlegR
	
	End
	Event
		EName	hipmovement
		Type	Local
		At	UlegL
		Part1	:FireElementalFireLegsTop2.part
		Pmagnet	hips
	
	End
	Event
		EName	hipmovement2
		Type	Local
		At	UlegR
		Part1	:FireElementalFireLegsTop2.part
		Pmagnet	hips
	
	End


End

Condition
	On 	Time
	Time 	10

	Event
		EName	spawnFire
		Type	Destroy 
			
	
	End

	End

End

##############################################################

