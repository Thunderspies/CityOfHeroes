#############################################################
## Pistols_Attack.fx
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	Root
		sound	DW_Pistols_01 120.0 100.0 1.0
	End
End

Condition
	On 	Time
	Time 	9 
	
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
		ChildFX WEAPONS\DualPistols\Pistols_Right.fx 
	End

	Event
		EName	PistolShot_Left
		Type	Local
		At		PrimeLeft
		Until	SHOOT RIGHT
		Geom	Bullet01
		ChildFX WEAPONS\DualPistols\Pistols_Left.fx 

	End
End

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		Ename	Prime
		Type	Start
		At	Origin
		BhvrOverride
			TrackRate		5.0
		End
		LookAt	T_Chest
		Magnet	T_Chest
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