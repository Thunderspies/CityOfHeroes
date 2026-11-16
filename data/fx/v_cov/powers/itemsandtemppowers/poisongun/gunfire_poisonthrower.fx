#############################################################
## Gunfire_PoisonThrower.fx
#############################################################

FxInfo

LifeSpan 110

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		ENAME   p2
		Type	Local
		At	WepR
		Geom	FlameThrower
	End

	Event
		Type	local
		At	p2
		Altpiv	1
		Part1	:PoisonThrowerLight.part
		Part	:PoisonThrowerDark.part
		Sound gasgunhiss 90.0 90.0 .8
		LifeSpan  105
	End

	Event
		Type	local
		At	p2
		Altpiv	1
		Part	:PoisonSpecksFalling.part
		Part	:BlackPoisonSpecksFalling.part
		LifeSpan  110
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	start
		At	C_Neck
		Bhvr	Behaviors\Fastprojectile.bhvr
		BhvrOverride
			TrackRate	0.75
		End
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End