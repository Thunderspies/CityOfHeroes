#########################################################
##	template

FxInfo


Flags InheritAlpha
Flags InheritAnimScale

######## arrow projectile  ##########################
Condition
	On 	Time
	Time 	3

	
	Event
		EName 	Projectile
		Type	start
		At	WepL
		Bhvr	Behaviors\projectile2.bhvr
		Geom	CrossBowBolt
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