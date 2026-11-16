#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepR
		BHVR	Weapons/Bow/Archery/HandRotateNet.bhvr
		Geom	StartNetArrow
		LifeSpan 19
	End
End

#############################################################

Condition
	On 	Time
	Time 	24

	Event
		EName 	Prime
		Type	start
		At	BendMystic  ##WepL
		Bhvr	Weapons/Bow/Archery/Netprojectile.bhvr
		Geom	aNetArrow
		Sound Crossbow1 100.0 100.0 .7
		Magnet	T_hips
		LookAt	T_hips
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End
End

#############################################################

End