#########################################################
##	
##

FxInfo

LifeSpan  190

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	WepR
End


##################################
Condition
	On 	Time
	Time 	64
	
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
		Ename 	All
		Type	Destroy
	End
End


End				