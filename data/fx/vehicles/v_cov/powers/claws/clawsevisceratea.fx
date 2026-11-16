#########################################################
##	Fireweapon
########################################################
FxInfo


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
	Time 	10

	Event
		EName	Streak1
		Type Local
		At	Root
		Geom	RootToChestAdjustment01
		Sound Eviscerate5 100.0 100.0 .8
	End
End

#####################################################################################
##Scale up and down of the blade fx
##########################################

Condition
	On 	Time
	Time 	35

	Event
		EName	ClawSpikeR
		Type	local
		At	WepR

		Geom	Arachnos_Blade_R
		BHVR	:ClawWeapon_Fade_UP.bhvr
		LifeSpan	25
	End
	
	Event
		EName	ClawSpikeL
		Type	local
		At	WepL
		
		Geom	Arachnos_Blade
		BHVR	:ClawWeapon_Fade_UP.bhvr
		LifeSpan	25

	End

End

Condition
	On 	Time
	Time 	60

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
	Time 	49

	Event
		EName	Streak
		Type	start
		At	Streak1
		Altpiv	1
		Part	Powers/Claws/EviscerateStreak.part
	End

	Event
		EName	Streak9
		Type	start
		At	wepL
		Part	Powers/Claws/ClawEmbersBroadEvicerate.part
		#Part	Powers/Claws/ClawEmbersEvicerate.part
		LifeSpan	3
	End

	Event
		EName	Streak8
		Type	start
		At	wepR
		
		Part	Powers/Claws/ClawEmbersBroadEvicerate.part
		#Part	Powers/Claws/ClawEmbersEvicerate.part
		LifeSpan	3
	End
End


Condition
	On 	Time
	Time 	51

	Event
		EName	streak
		Type	Destroy
		
	End
	

End

#########################################################
##2 hit
###############
Condition
	On Time 
	Time 49
	
	Event
		EName	Pow
		Type 	Local
		At	T_Chest
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		Sound HitReact3 100.0 100.0 .5
	End

End

Condition
	On Time 
	Time 51
	
	Event
		EName	Pow
		Type 	Destroy
			
	End

End


End
##################################
