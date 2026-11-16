#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	
	Inpname Origin
End

Input

	Inpname	Mystic

End

lifespan	200

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
	Time 	29

	Event
		Type Local
		At origin
		Sound Eviscerate5 100.0 100.0 .8
	End
End


#####################################################################################
##Scale up and down of the blade fx
##########################################

Condition
	On 	Time
	Time 	34

	Event
		EName	ClawSpikeR
		Type	local
		At	WepR

		Geom	Arachnos_Blade_R
		BHVR	:ClawWeapon_Fade_UP.bhvr
		LifeSpan	50
	End
	
	Event
		EName	ClawSpikeL
		Type	local
		At	WepL
		
		Geom	Arachnos_Blade
		BHVR	:ClawWeapon_Fade_UP.bhvr
		LifeSpan	50

	End

End

Condition
	On 	Time
	Time 	84

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
	Time 	65

	Event
		EName	Streak
		Type	start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		lifespan	69
	End

End


Condition
	On 	Time
	Time 	67

	Event
		EName	streak
		Type	Destroy
		
	End
	

End


End
##################################
