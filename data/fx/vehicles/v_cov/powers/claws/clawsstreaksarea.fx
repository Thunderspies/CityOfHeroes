#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	root
End

Input  
	Inpname	mystic
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

##############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	P1
		Type	Local 
		At	WepR
		Geom	Arachnos_WristRocket

		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
	
	Event
		EName 	P2
		Type	Local 
		At	WepL
		Geom	Arachnos_WristRocket_L

		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName	ClawSpikeR
		Type	local
		At	WepR

		Geom	Arachnos_Blade_R
		BHVR	:ClawWeapon_Fade_UP.bhvr
		LifeSpan	75
	End
	
	Event
		EName	ClawSpikeL
		Type	local
		At	WepL
		
		Geom	Arachnos_Blade
		BHVR	:ClawWeapon_Fade_UP.bhvr
		LifeSpan	75

	End

End

Condition
	On 	Time
	Time 	75

	Event
		EName	ClawSpikeR
		Type	local
		At	WepR

		Geom	Arachnos_Blade_R
		BHVR	:ClawWeapon_Fade_Down.bhvr
		LifeSpan	20
	End
	
	Event
		EName	ClawSpikeL
		Type	local
		At	WepL
		
		Geom	Arachnos_Blade
		BHVR	:ClawWeapon_Fade_Down.bhvr
		LifeSpan	20

	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName	p
		Type	local	#start
		At	WepR
		Part1	Powers/Claws/ClawStreak.part
		Sound Clawspin 100 100 1.0
	End

End

Condition
	On 	Time
	Time 	34

	Event
		EName	Streakburst
		Type	local	#start
		At	WepR
		part1	Powers/Claws/ClawEmbers.part
		part2	Powers/Claws/ClawEmbersBroad.part
	End

End

Condition
	On 	Time
	Time 	35

	Event
		EName	Ring
		Type	start
		At	root
		Part5	Powers/Claws/ClawsRing.part
	End

End


Condition
	On Time 
	Time 50
	
	Event
		EName	Streakburst
		Type 	Destroy
				
	End

	Event
		EName	Ring
		Type 	Destroy
				
	End

End


Condition
	On Time 
	Time 38
	Event
		EName	p
		Type 	Destroy
				
	End
End


Condition
	On Time 
	Time 200
	
	Event
		EName	all
		Type 	Destroy
				
	End

End

End
##################################

