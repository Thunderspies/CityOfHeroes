#########################################################
##	
##

FxInfo

LifeSpan  190

##################################
Condition
	On 	Time
	Time 	45

	
	Event
		EName 	x
		Type	Local
		At	WepR
		Geom	Pistol
				
	End
	
	Event
		EName 	Prime
		Type	start
		At	x
		 Altpiv	1
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