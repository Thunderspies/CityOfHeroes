#########################################################
##	template

FxInfo


Flags InheritAnimScale InheritAlpha

LifeSpan 300

######## arrow in hand   ###########################

Condition
	On	Time
	Time	28

	Event	
		Type	Local
		At	WepR
		Sound arrowout2 50 50 .25
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	WepR
		BHVR	Weapons/Bow/Archery/HandRotateNet.bhvr
		Geom	StartNetArrow
		LifeSpan 31
	End
End

######## arrow projectile  ##########################
Condition
	On 	Time
	Time 	61

	
	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	Weapons/Bow/Archery/Netprojectile.bhvr
		Geom	aNetArrow
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

		