#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 190

######## arrow in hand   ###########################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepR
		Geom	GEO_Arrow_test
		LifeSpan 17
	End
End

######## arrow projectile  ##########################
Condition
	On 	Time
	Time 	23

	
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

		