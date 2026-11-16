#########################################################
##	Warburg Bomb - Nuke
##

FxInfo
	
lifespan	600

Condition
	On	Time
	Time	20

	Event
		Type	local
		At	Origin
		Sound incoming3 90 90 .6

	End
End


Condition
	On	Time
	Time	70

	Event
		Type	local
		At	Origin
		Sound atomicblast2 90 90 .88

	End
End

Condition
	On	Time
	Time	75
	Event
		Type	local
		At	Origin
		Sound firesword_loop 90 90 .48
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 5

	End
End





Condition

	On Time 
	Time 0
	
	Event
		Ename	Start
		Type	PositOnly
		At	T_Root
		Sound Nova4 100.0 100.0 .98
	End


	Event
		EName 	TargetLock1
		Type	Local 
		At	Start
		BhvrOverride
			StartJitter	5 0 5
		End
		Part1   :MissileLock.part

	End

	Event
		EName 	Prime1
		Type	Start 
		At	Start
		Bhvr	:MissileStrike.bhvr
		BhvrOverride
			PositionOffset	0 180 0
		End
		Magnet	TargetLock1
		LookAt	TargetLock1
		Geom	_5th_missile
		Part1	:MissileStrike_smoke.part
		Part2	:MissileStrike_fire.part
	End

End

Condition

	On Time 
	Time 5

	Event
		EName 	TargetLock2
		Type	Local 
		At	TargetLock1
		BhvrOverride
			PositionOffset	15 0 0
			StartJitter	5 0 15
		End
		Part1   :MissileLock.part
	
	End

	Event
		EName 	Prime2
		Type	Start 
		At	Start
		Bhvr	:MissileStrike.bhvr
		BhvrOverride
			PositionOffset	0 220 0
		End
		Magnet	TargetLock2
		LookAt	TargetLock2
		Geom	_5th_missile
		Part1	:MissileStrike_smoke.part
		Part2	:MissileStrike_fire.part
	End

End

Condition

	On Time 
	Time 10

	Event
		EName 	TargetLock3
		Type	Local 
		At	TargetLock1
		BhvrOverride
			PositionOffset	-15 0 0
			StartJitter	5 0 15
		End
		Part1   :MissileLock.part

	End

	Event
		EName 	Prime3
		Type	Start 
		At	Start
		Bhvr	:MissileStrike.bhvr
		BhvrOverride
			PositionOffset	0 260 0
		End
		Magnet	TargetLock3
		LookAt	TargetLock3
		Geom	_5th_missile
		Part1	:MissileStrike_smoke.part
		Part2	:MissileStrike_fire.part
	End

End

Condition
	On Time
	Time	40

	Event
		Ename	Blast1
		Type	Local
		At	TargetLock1
		ChildFX	:NukeBomb.fx

	End

	Event
		EName	Prime1
		Type	Destroy
	End

End

Condition
	On Time
	Time	55

	Event
		Ename	Blast2
		Type	Local
		At	TargetLock2
		ChildFX	:NukeBomb.fx

	End

	Event
		EName	Prime2
		Type	Destroy
	End

End

Condition
	On Time
	Time	70

	Event
		Ename	Blast3
		Type	Local
		At	TargetLock3
		ChildFX	:NukeBomb.fx

	End

	Event
		EName	Prime3
		Type	Destroy
	End

End

End