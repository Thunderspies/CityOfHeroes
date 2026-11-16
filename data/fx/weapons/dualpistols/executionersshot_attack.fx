#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo

LifeSpan 300

#############################################################

Input
	Inpname T_Head
End

#############################################################

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	Root
		sound	DW_ExecutionersShot_01 120.0 100.0 1.0
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	30
		
Event
		Ename	Prime
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
		EName	PistolShot_Right
		Type	Local
		At		Prime
		Geom	Bullet01
		ChildFX WEAPONS\DualPistols\SuppressiveFire_Right.fx 
End 
	
	Event
		Type	Start
		At	MuzzleFlashOffset
		ChildFX	WEAPONS\DualPistols\Child_SingleShot.fx
		Lifespan 39
	End

	Event
		Type	Start
		At	WepR
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		ChildFX	WEAPONS\DualPistols\Child_ShellEject.fx
		Lifespan 1
	End
End

##############################################################


Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	ALL
	End
End

#############################################################

End