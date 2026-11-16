#############################################################
## SuppressiveFire_Attack.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		sound	DW_SuppressiveFire_01 120.0 100.0 1.0
		Lifespan 30                                          
	End
End
	
Condition
	On	Time
	Time	14

Event
		Ename	PrimeRight
		Type	Start
		At		WepR
		BhvrOverride
			TrackRate		7.0
		End
		POther	Target
		Magnet	Target
		LookAt  Target
End
		
Event
		Ename	PrimeLeft
		Type	Start
		At		WepL
		BhvrOverride
			TrackRate		7.0
		End
		POther	Target
		Magnet	Target
		LookAt  Target
End


	Event
		EName	PistolShot_Right
		Type	Local
		At		PrimeRight
		Until	SHOOT LEFT
		Geom	Bullet01
		ChildFX WEAPONS\DualPistols\SuppressiveFire_Right.fx 
	End

	Event
		EName	PistolShot_Left
		Type	Local
		At		PrimeLeft
		Until	SHOOT RIGHT
		Geom	Bullet01
		ChildFX WEAPONS\DualPistols\SuppressiveFire_Left.fx 

	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Ename	Prime
		Type	Start
		At	Origin
		BhvrOverride
			TrackRate		7.0
		End
		LookAt	Target
		Magnet	Target
	End
End

Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	ALL
	End
End

#############################################################

End