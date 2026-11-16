#########################################################
##	
##

FxInfo

LifeSpan  390

####################################################################

Condition
	On 	Time
	Time 	10

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

##################################
Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	start
		At	WepL

		Bhvr	Behaviors\projectile3.bhvr
		Geom	Arachnos_Blade_Shooting
		Sound Crossbow1 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
	End

End

##################################


Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End


End				