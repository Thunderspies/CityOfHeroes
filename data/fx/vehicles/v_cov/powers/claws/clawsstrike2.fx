#########################################################
##	Claws Strike
########################################################
FxInfo

Input  
	Inpname	Mystic
End

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
	Time 	15

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound Claw2b 100.0 100.0 .6
		LifeSpan	3
	End

End


#####################################################################################
##Scale up and down of the blade fx
##########################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ClawSpikeR
		Type	local
		At	WepR

		Geom	Arachnos_Blade_R
		BHVR	:ClawWeapon_Fade_UP.bhvr
		LifeSpan	19
	End
	
	Event
		EName	ClawSpikeL
		Type	local
		At	WepL
		
		Geom	Arachnos_Blade
		BHVR	:ClawWeapon_Fade_UP.bhvr
		LifeSpan	19

	End

End

Condition
	On 	Time
	Time 	19

	Event
		EName	ClawSpikeR
		Type	local
		At	WepR

		Geom	Arachnos_Blade_R
		BHVR	:ClawWeapon_Fade_Down.bhvr
		LifeSpan	50
	End
	
	Event
		EName	ClawSpikeL
		Type	local
		At	WepL
		
		Geom	Arachnos_Blade
		BHVR	:ClawWeapon_Fade_Down.bhvr
		LifeSpan	50

	End

End

#############################################################################################

Condition
	On 	Time
	Time 	200

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################

