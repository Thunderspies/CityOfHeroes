#############################################################
## DualWield_Attack.fx
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 30

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Root
		sound	DW_DualWield_01 120.0 100.0 1.0
	End
End

#############################################################

Condition
	On 	Time
	Time 	19

Event
		Ename	PrimeRight
		Type	Start
		At		WepR
		BhvrOverride
			PositionOffset	0 0 -0.5
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
		Geom	Bullet01
		BhvrOverride
			PositionOffset	0 0.25 0
		End
		ChildFX WEAPONS\DualPistols\Pistols_Right.fx 
	End

End

Condition
	On 	Time
	Time 	20


		
Event
		Ename	PrimeLeft
		Type	Start
		At		WepL
		BhvrOverride
			PositionOffset	0 0 0.5
			TrackRate		7.0
		End
		POther	Target
		Magnet	Target
		LookAt  Target
End

Event
		EName	PistolShot_Left
		Type	Local
		At		PrimeLeft
		Geom	Bullet01
		ChildFX WEAPONS\DualPistols\Pistols_Left.fx 

	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename	Prime
		Type	Start
		At	Origin
		BhvrOverride
			TrackRate		5.0
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